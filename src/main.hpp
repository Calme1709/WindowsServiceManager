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
	Napi::Value CanPauseAndContinue(const CallbackInfo& info);
	Napi::Value CanShutdown(const CallbackInfo& info);
	Napi::Value CanStop(const CallbackInfo& info);
	Napi::Value DependentServices(const CallbackInfo& info);
	Napi::Value DisplayName(const CallbackInfo& info);
	Napi::Value MachineName(const CallbackInfo& info);
	Napi::Value ServiceName(const CallbackInfo& info);
	Napi::Value ServicesDependedOn(const CallbackInfo& info);
	Napi::Value ServiceType(const CallbackInfo& info);
	Napi::Value StartType(const CallbackInfo& info);
	Napi::Value Status(const CallbackInfo& info);

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