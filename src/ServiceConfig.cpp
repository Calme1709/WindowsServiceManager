#include "ServiceConfig.hpp"

#include "Utils.hpp"

void ServiceConfig::Refresh() {
	auto bytesNeeded = DWORD{ 0 };

	if (!QueryServiceConfigA(srvHandle, nullptr, 0, &bytesNeeded)) {
		if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
			std::vector<unsigned char> buffer(bytesNeeded, 0);

			auto lpsc = reinterpret_cast<LPQUERY_SERVICE_CONFIGA>(buffer.data());
			if (QueryServiceConfigA(srvHandle, lpsc, bytesNeeded, &bytesNeeded)) {
				type = (ServiceType)lpsc->dwServiceType;
				startType = (ServiceStartType)lpsc->dwStartType;
				errorControl = (ServiceErrorControl)lpsc->dwErrorControl;
				pathName = lpsc->lpBinaryPathName;
				loadOrderGroup = lpsc->lpLoadOrderGroup;
				tagId = lpsc->dwTagId;
				dependencies = Utils::SplitDoubleNullTerminatedString(lpsc->lpDependencies);
				startName = lpsc->lpServiceStartName;
				displayName = lpsc->lpDisplayName;
			}
		}
	}

	bytesNeeded = 0;
	if (!QueryServiceConfig2A(srvHandle, SERVICE_CONFIG_DESCRIPTION, nullptr, 0, &bytesNeeded)) {
		if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
			std::vector<unsigned char> buffer(bytesNeeded, 0);

			if (QueryServiceConfig2A(srvHandle, SERVICE_CONFIG_DESCRIPTION, reinterpret_cast<LPBYTE>(buffer.data()), bytesNeeded, &bytesNeeded)) {
				auto lpsd = reinterpret_cast<LPSERVICE_DESCRIPTIONA>(buffer.data());

				description = lpsd->lpDescription;
			}
		}
	}
}

ServiceConfig ServiceConfig::Create(SC_HANDLE const handle) {
	auto config = ServiceConfig{};
	config.srvHandle = handle;

	config.Refresh();

	return config;
}

bool ServiceConfig::ChangeStartType(ServiceStartType const type) {
	auto result = false;

	if (ChangeServiceConfigA(
		srvHandle,
		SERVICE_NO_CHANGE,
		static_cast<DWORD>(type),
		SERVICE_NO_CHANGE,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr))
	{
		startType = type;
		result = true;
	}

	return result;
}

bool ServiceConfig::ChangeStartErrorControl(ServiceErrorControl const control) {
	auto result = false;

	if (ChangeServiceConfigA(
		srvHandle,
		SERVICE_NO_CHANGE,
		SERVICE_NO_CHANGE,
		static_cast<DWORD>(control),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr))
	{
		errorControl = control;
		result = true;
	}

	return result;
}

bool ServiceConfig::ChangeDescription(string newDescription) {
	auto result = false;

	auto sd = SERVICE_DESCRIPTIONA {};
	sd.lpDescription = const_cast<char *>(newDescription.c_str());

	if (ChangeServiceConfig2A(srvHandle, SERVICE_CONFIG_DESCRIPTION, &sd)) {
		description = newDescription;
		result = true;
	}

	return result;
}