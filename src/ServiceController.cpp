#pragma once

#include "ServiceController.hpp"

#include <thread>

ServiceController::ServiceController(string name, DWORD access) {
	srvName = name;
	scHandle = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
	if (scHandle) {
		srvHandle = OpenServiceA(scHandle, name.c_str(), access);

		if (!srvHandle) {
			scHandle = nullptr;
		} else {
			connected = true;
			auto bytesNeeded = DWORD{ 0 };
			auto ssp = SERVICE_STATUS_PROCESS{ 0 };

			auto result = QueryServiceStatusEx(srvHandle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp), &bytesNeeded);

			if (result != 0) {
				auto setControl = [ssp](std::map<ServiceControls, bool>& controls, ServiceControls const control) { 
					controls[control] = (ssp.dwControlsAccepted & static_cast<int>(control)) != 0; 
				};

				setControl(acceptedControls, ServiceControls::Stop);
				setControl(acceptedControls, ServiceControls::PauseAndContinue);
				setControl(acceptedControls, ServiceControls::ChangeParams);
				setControl(acceptedControls, ServiceControls::ChangeBindings);
				setControl(acceptedControls, ServiceControls::PreShutdown);
				setControl(acceptedControls, ServiceControls::ShutdownNotification);
				setControl(acceptedControls, ServiceControls::HardwareProfileChangedNotification);
				setControl(acceptedControls, ServiceControls::PowerChangedNotification);
				setControl(acceptedControls, ServiceControls::SessionChangedNotification);
				setControl(acceptedControls, ServiceControls::TriggerEventNotification);
				setControl(acceptedControls, ServiceControls::TimeChangeNotification);
				setControl(acceptedControls, ServiceControls::UserModeNotification);
			}
		}
	}
}

ServiceStatus ServiceController::GetStatus() {
	auto status = ServiceStatus::Unknown;

	if (srvHandle) {
		auto bytesNeeded = DWORD{ 0 };
		auto ssp = SERVICE_STATUS_PROCESS { 0 };

		auto result = QueryServiceStatusEx(srvHandle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp), &bytesNeeded);

		if (result != 0) {
			status = static_cast<ServiceStatus>(ssp.dwCurrentState);
		}
	}
    
    return status;
}

bool ServiceController::Start() {
	auto success = false;

	if (srvHandle) {
		auto result = StartServiceA(srvHandle, 0, nullptr);
		success = result != 0;
	}

	return success;
}

bool ServiceController::Stop() {
	auto success = false;

	if (srvHandle) {
		success = StopDependentServices();

		if (success) {
			auto ssp = SERVICE_STATUS_PROCESS{ 0 };
			success = ChangeServiceStatus(srvHandle, SERVICE_CONTROL_STOP, ssp);
		}
	}

	return success;
}

bool ServiceController::Pause() {
	auto success = false;

	if (srvHandle) {
		auto ssp = SERVICE_STATUS_PROCESS{ 0 };
		success = ChangeServiceStatus(srvHandle, SERVICE_CONTROL_PAUSE, ssp);
	}

	return success;
}

bool ServiceController::Continue() {
    auto success = false;

	if (srvHandle) {
		auto ssp = SERVICE_STATUS_PROCESS{ 0 };
		success = ChangeServiceStatus(srvHandle, SERVICE_CONTROL_CONTINUE, ssp);
	}

	return success;
}

bool ServiceController::Delete() {
	auto success = false;

	if (srvHandle) {
		success = DeleteService(srvHandle) != 0;

		if (success) {
			srvHandle = nullptr;
		}
	}

	return success;
}

bool ServiceController::WaitForStatus(ServiceStatus desiredStatus, std::chrono::milliseconds const timeout) {
	auto success = false;

	if (srvHandle) {
		auto ssp = SERVICE_STATUS_PROCESS{ 0 };

		auto bytesNeeded = DWORD{ 0 };

		if (QueryServiceStatusEx(srvHandle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp), &bytesNeeded)) {
			success = WaitForStatus(srvHandle, ssp, desiredStatus, timeout);
		}
	}

	return success;
}

ServiceConfig ServiceController::GetServiceConfig() {
	return ServiceConfig::Create(srvHandle);
}

std::vector<LPSTR> ServiceController::GetDependentServices() {
	auto bytesNeeded = DWORD{ 0 };
	auto count = DWORD{ 0 };
	auto services = vector<LPSTR>();

	if (!EnumDependentServicesA(srvHandle, SERVICE_ACTIVE, nullptr, 0, &bytesNeeded, &count)) {
		if (GetLastError() != ERROR_MORE_DATA) {
			throw ERROR_MORE_DATA;
		}

		std::vector<unsigned char> buffer(bytesNeeded, 0);

		auto result = EnumDependentServicesA(srvHandle, SERVICE_ACTIVE, reinterpret_cast<LPENUM_SERVICE_STATUSA>(buffer.data()), bytesNeeded, &bytesNeeded, &count);

		if (!result) {
			throw GetLastError();
		}

		for (auto i = DWORD{ 0 }; i < count; ++i) {
			auto ess = static_cast<ENUM_SERVICE_STATUSA>(*(reinterpret_cast<LPENUM_SERVICE_STATUSA>(buffer.data() + i)));
				
			services.push_back(ess.lpServiceName);
		}
	}

	return services;
}

std::chrono::milliseconds ServiceController::GetWaitTime(DWORD const waitHint) {
	auto waitTime = waitHint / 10;

	if (waitTime < 1000) {
		waitTime = 1000;
	} else if (waitTime > 10000) {
		waitTime = 10000;
	}

	return std::chrono::milliseconds(waitTime);
}

bool ServiceController::ChangeServiceStatus(SC_HANDLE const handle, DWORD const controlCode, SERVICE_STATUS_PROCESS& ssp) {
	auto success = false;

	if (handle) {
		auto result = ControlService(handle, controlCode, reinterpret_cast<LPSERVICE_STATUS>(&ssp));

		success = result != 0;
	}

	return success;
}

bool ServiceController::WaitForStatus(SC_HANDLE const handle, SERVICE_STATUS_PROCESS& ssp, ServiceStatus const desireStatus, std::chrono::milliseconds const timeout) {
	auto success = false;;

	if(ssp.dwCurrentState == static_cast<DWORD>(desireStatus)) {
		return true;
	}

	if(handle) {
		auto start = std::chrono::high_resolution_clock::now();
		auto waitTime = GetWaitTime(ssp.dwWaitHint);

		while (ssp.dwCurrentState != static_cast<DWORD>(desireStatus)) {
			std::this_thread::sleep_for(waitTime);

			auto bytesNeeded = DWORD{ 0 };

			if (!QueryServiceStatusEx(handle, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp), &bytesNeeded)) {
				break;
			}

			if (ssp.dwCurrentState == static_cast<DWORD>(desireStatus)) {
				success = true;
				break;
			}

			if (timeout != -1ms && std::chrono::high_resolution_clock::now() - start > timeout) {
				break;
			}
		}
	}

	return success;
}

bool ServiceController::StopDependentServices()  {
	auto services = GetDependentServices();

	for(auto service : services) {
		ServiceHandle handle = OpenServiceA(scHandle, service, SERVICE_STOP | SERVICE_QUERY_STATUS);

		if (!handle) {
			return false;
		}

		auto ssp = SERVICE_STATUS_PROCESS{ 0 };

		if (!ChangeServiceStatus(handle, SERVICE_CONTROL_STOP, ssp)) {
			return false;
		}

		if (!WaitForStatus(handle, ssp, ServiceStatus::Stopped)) {
			return false;
		}
	}

	return true;
}