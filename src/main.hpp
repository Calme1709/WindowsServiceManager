#pragma once

#include <napi.h>
#include "ServiceController.cpp"

using namespace std;
using namespace Napi;

class WinSvcManager : public ObjectWrap<WinSvcManager> {
public:
	static Object Init(Napi::Env env, Object exports);
	WinSvcManager(const CallbackInfo& info);

private:
	//Internal values
	ServiceController* service;

	//Static methods
	static Napi::Value GetServices(const CallbackInfo& info);
	static Napi::Value GetDevices(const CallbackInfo& info);

	//Accessors
	Napi::Value GetCanPauseAndContinue(const CallbackInfo& info);
	Napi::Value GetCanShutdown(const CallbackInfo& info);
	Napi::Value GetCanStop(const CallbackInfo& info);
	Napi::Value GetDependentServices(const CallbackInfo& info);
	Napi::Value GetDisplayName(const CallbackInfo& info);
	Napi::Value GetMachineName(const CallbackInfo& info);
	Napi::Value GetServiceName(const CallbackInfo& info);
	Napi::Value GetServicesDependedOn(const CallbackInfo& info);
	Napi::Value GetServiceType(const CallbackInfo& info);
	Napi::Value GetStartType(const CallbackInfo& info);
	Napi::Value GetStatus(const CallbackInfo& info);

	//Methods
	Napi::Value Close(const CallbackInfo& info);
	Napi::Value Continue(const CallbackInfo& info);
	Napi::Value Dispose(const CallbackInfo& info);
	Napi::Value ExecuteCommand(const CallbackInfo& info);
	Napi::Value Pause(const CallbackInfo& info);
	Napi::Value Start(const CallbackInfo& info);
	Napi::Value Stop(const CallbackInfo& info);
	Napi::Value WaitForStatus(const CallbackInfo& info);
};