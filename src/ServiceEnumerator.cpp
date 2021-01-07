#include "ServiceEnumerator.hpp"

std::vector<ServiceStatusProcess> ServiceEnumerator::EnumerateServices(ServiceType const type = ServiceType::All, ServiceState const state = ServiceState::All, string const * machine = nullptr, string const * dbname = nullptr, string const * groupName = nullptr) {
	std::vector<ServiceStatusProcess> ssps;

	auto scHandle = ServiceHandle { OpenSCManagerA(Utils::c_str(machine), Utils::c_str(dbname), SC_MANAGER_ENUMERATE_SERVICE) };

	auto bytesNeeded = DWORD{ 0 };
	auto servicesReturnedCount = DWORD{ 0 };
	auto resumeHandle = DWORD{ 0 };

	do {
		if (!EnumServicesStatusExA(scHandle, SC_ENUM_PROCESS_INFO, static_cast<DWORD>(type), static_cast<DWORD>(state), nullptr, 0, &bytesNeeded, &servicesReturnedCount, &resumeHandle, Utils::c_str(groupName))) {
			if (GetLastError() == ERROR_MORE_DATA) {
				std::vector<unsigned char> buffer(bytesNeeded, 0);

				if (EnumServicesStatusExA(scHandle, SC_ENUM_PROCESS_INFO, static_cast<DWORD>(type), static_cast<DWORD>(state), reinterpret_cast<LPBYTE>(buffer.data()), bytesNeeded, &bytesNeeded, &servicesReturnedCount, nullptr, Utils::c_str(groupName))) {
					auto essp = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESSA>(buffer.data());

					for (auto i = DWORD{ 0 }; i < servicesReturnedCount; ++i) {
						auto ssp = ServiceStatusProcess{};
						ssp.ServiceName = essp[i].lpServiceName;
						ssp.DisplayName = essp[i].lpDisplayName;
						ssp.Status = essp[i].ServiceStatusProcess;

						ssps.push_back(ssp);
					}
                }
				else break;
			}
			else break;
		}
	} while (resumeHandle != 0);

	return ssps;
}