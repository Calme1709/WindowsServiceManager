// Copyright (c) 2016 Marius Bancila
// License: CPOL http://www.codeproject.com/info/cpol10.aspx

#pragma once

#include "ServiceConstants.hpp"
#include "ServiceHandle.hpp"
#include "utils.cpp"

#include <vector>

struct ServiceStatusProcess {
	string ServiceName;
	string DisplayName;
	SERVICE_STATUS_PROCESS Status;
};

class ServiceEnumerator {
public:
	static std::vector<ServiceStatusProcess> EnumerateServices(ServiceType const type = ServiceType::All, ServiceState const state = ServiceState::All, string const * machine = nullptr, string const * dbname = nullptr, string const * groupName = nullptr);
};