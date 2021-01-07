// Copyright (c) 2016 Marius Bancila
// License: CPOL http://www.codeproject.com/info/cpol10.aspx

#pragma once

#include <string>
#include <vector>
#include "ServiceConstants.hpp"

class ServiceConfig {
	SC_HANDLE srvHandle;
	ServiceType type;
	ServiceStartType startType;
	ServiceErrorControl errorControl;
	string pathName;
	string loadOrderGroup;
	DWORD tagId;
	std::vector<string> dependencies;
	string startName;
	string displayName;
	string description;

public:
	ServiceType GetType() const { return type; }
	ServiceStartType GetStartType() const { return startType; }
	ServiceErrorControl GetErrorControl() const { return errorControl; }
	string GetBinaryPathName() const { return pathName; }
	string GetLoadOrderingGroup() const { return loadOrderGroup; }
	DWORD GetTagId() const { return tagId; }
	std::vector<string> const GetDependencies() const { return dependencies; }
	string GetStartName() const { return startName; }
	string GetDisplayName() const { return displayName; }
	string GetDescription() const { return description; }

	static ServiceConfig Create(SC_HANDLE const handle);
	bool ChangeStartType(ServiceStartType const type);
	bool ChangeStartErrorControl(ServiceErrorControl const control);
	bool ChangeDescription(string newDescription);
	void Refresh();
};