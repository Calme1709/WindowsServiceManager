// Copyright (c) 2016 Marius Bancila
// License: CPOL http://www.codeproject.com/info/cpol10.aspx

#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <map>

#include "ServiceConfig.hpp"
#include "ServiceHandle.hpp"

using namespace std::literals::chrono_literals;

class ServiceController {
public:
	ServiceController(string name, DWORD access = SERVICE_ALL_ACCESS);

	ServiceStatus GetStatus();
	ServiceConfig GetServiceConfig();
	std::vector<LPSTR> GetDependentServices();
	bool Start();
	bool Stop();
	bool Pause();
	bool Continue();
	bool Delete();
	bool WaitForStatus(ServiceStatus desiredStatus, std::chrono::milliseconds const timeout);

	bool CanAcceptControl(ServiceControls const control) const {
		auto it = acceptedControls.find(control);
		return it != std::end(acceptedControls) ? it->second : false;
	}

	string GetServiceName() const { return srvName; }

private:
	ServiceHandle scHandle;
	ServiceHandle srvHandle;
	string srvName;

	std::map<ServiceControls, bool> acceptedControls = {
		{ ServiceControls::Stop, false},
		{ ServiceControls::PauseAndContinue, false },
		{ ServiceControls::ChangeParams, false },
		{ ServiceControls::ChangeBindings, false },
		{ ServiceControls::PreShutdown, false },
		{ ServiceControls::ShutdownNotification, false },
		{ ServiceControls::HardwareProfileChangedNotification, false },
		{ ServiceControls::PowerChangedNotification, false },
		{ ServiceControls::SessionChangedNotification, false },
		{ ServiceControls::TriggerEventNotification, false },
		{ ServiceControls::TimeChangeNotification, false },
		{ ServiceControls::UserModeNotification, false },
	};

	static std::chrono::milliseconds GetWaitTime(DWORD const waitHint);
	static bool ChangeServiceStatus(SC_HANDLE const handle, DWORD const controlCode, SERVICE_STATUS_PROCESS& ssp);
	static bool WaitForStatus(SC_HANDLE const handle, SERVICE_STATUS_PROCESS& ssp, ServiceStatus const desireStatus, std::chrono::milliseconds const timeout = -1ms);
	bool StopDependentServices();
};