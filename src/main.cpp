#pragma once

#include "main.hpp"

#include "utils.cpp"
#include <napi.h>

#include "AsyncWorkers/GetServicesWorker.cpp"

#include <iostream>

Object WinSvcManager::Init(Napi::Env env, Object exports) {
	Function func = DefineClass(env, "WinSvcManager", {
		StaticMethod("GetServices", &WinSvcManager::GetServices),
		//GetDevices
		InstanceAccessor("CanPauseAndContinue", &WinSvcManager::CanPauseAndContinue, nullptr, napi_default),
		InstanceAccessor("CanShutdown", &WinSvcManager::CanShutdown, nullptr, napi_default),
		InstanceAccessor("CanStop", &WinSvcManager::CanStop, nullptr, napi_default),
		InstanceAccessor("DependentServices", &WinSvcManager::DependentServices, nullptr, napi_default),
		InstanceAccessor("DisplayName", &WinSvcManager::DisplayName, nullptr, napi_default),
		//MachineName
		InstanceAccessor("ServiceName", &WinSvcManager::ServiceName, nullptr, napi_default),
		InstanceAccessor("ServicesDependedOn", &WinSvcManager::ServicesDependedOn, nullptr, napi_default),
		InstanceAccessor("ServiceType", &WinSvcManager::ServiceType, nullptr, napi_default),
		//StartType
		//Status

		//Close
		//Continue
		//Dispose
		//ExecuteCommand
		//Pause
		//Start
		//Stop
		//WaitForStatus

	});

	FunctionReference* constructor = new FunctionReference();
	*constructor = Persistent(func);
	env.SetInstanceData(constructor);

	exports.Set("default", func);
	return exports;
}

WinSvcManager::WinSvcManager(const CallbackInfo& info) : ObjectWrap<WinSvcManager>(info) {
	#ifndef _WIN32
		Error::New(info.Env(), "This library is only supported on windows machines").ThrowAsJavaScriptException();

		return;
	#endif

	if (info.Length() <= 0 || !info[0].IsString()) {
		TypeError::New(info.Env(), "Expected a string for service name").ThrowAsJavaScriptException();

		return;
	}

	auto serviceAccess = SC_MANAGER_ALL_ACCESS;

	service = new ServiceController(info[0].As<String>().Utf8Value(), serviceAccess);

	if(!service->connected) {
		Error::New(info.Env(), Utils::GetLastErrorString()).ThrowAsJavaScriptException();
	}
}

Value WinSvcManager::GetServices(const CallbackInfo& info) {
	//TODO: Support remote machines
	auto deferred = Promise::Deferred::New(info.Env());

	auto asyncWorker = new GetServicesWorker(
		info.Env(),
		deferred,
		info.Env().GetInstanceData<FunctionReference>()
	);

	asyncWorker->Queue();

	return deferred.Promise();
}

Value WinSvcManager::CanPauseAndContinue(const CallbackInfo& info) {
	return Boolean::New(info.Env(), service->CanAcceptControl(ServiceControls::PauseAndContinue));
}

Value WinSvcManager::CanShutdown(const CallbackInfo& info) {
	return Boolean::New(info.Env(), service->CanAcceptControl(ServiceControls::ShutdownNotification));
}

Value WinSvcManager::CanStop(const CallbackInfo& info) {
	return Boolean::New(info.Env(), service->CanAcceptControl(ServiceControls::Stop));
}

Value WinSvcManager::DependentServices(const CallbackInfo& info) {
	auto array = Array::New(info.Env());

	for(auto service : service->GetDependentServices()) {
		Utils::PushToNapiArray(array, info.Env().GetInstanceData<FunctionReference>()->New({ String::New(info.Env(), service) }));
	}

	return array;
}

Value WinSvcManager::ServiceName(const CallbackInfo& info) {
	return String::New(info.Env(), service->GetServiceName());
}

Value WinSvcManager::DisplayName(const CallbackInfo& info) {
	return String::New(info.Env(), service->GetServiceConfig().GetDisplayName());
}

Value WinSvcManager::Continue(const CallbackInfo& info) {
	if(service->CanAcceptControl(ServiceControls::PauseAndContinue) && service->GetStatus() == ServiceStatus::Paused && !service->Continue()) {
    	Error::New(info.Env(), String::New(info.Env(), Utils::GetLastErrorString())).ThrowAsJavaScriptException();
    }

	return info.Env().Undefined();
}

Value WinSvcManager::ServiceType(const CallbackInfo& info) {
	return Napi::Number::New(info.Env(), (double)service->GetServiceConfig().GetType());
}

Value WinSvcManager::ServicesDependedOn(const CallbackInfo& info) {
	auto array = Array::New(info.Env());

	for(auto dependency : service->GetServiceConfig().GetDependencies()) {
		FunctionReference* constructor = info.Env().GetInstanceData<FunctionReference>();

		Utils::PushToNapiArray(array, constructor->New({ String::New(info.Env(), dependency) }));
	}

	return array;
}

Object InitAll(Env env, Object exports) {
	WinSvcManager::Init(env, exports);

	return exports;
}

NODE_API_MODULE(addon, InitAll)