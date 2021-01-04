#include "main.h"

#include <iostream>

Napi::Object WinSvcManager::Init(Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass(env, "WinSvcManager", {
		StaticMethod("getServices", &WinSvcManager::GetServices),
		InstanceAccessor("canPauseAndContinue", &WinSvcManager::CanPauseAndContinue, nullptr, napi_default),
		
		InstanceMethod("changeStartupType", &WinSvcManager::ChangeStartupType)
	});

	Napi::FunctionReference* constructor = new Napi::FunctionReference();
	*constructor = Napi::Persistent(func);
	env.SetInstanceData(constructor);

	exports.Set("default", func);
	return exports;
}

WinSvcManager::WinSvcManager(const Napi::CallbackInfo& info) : Napi::ObjectWrap<WinSvcManager>(info) {
	#ifndef _WIN32
		Napi::Error::New(info.Env(), "This library is only supported on windows machines").ThrowAsJavaScriptException();

		return;
	#endif

	if (info.Length() <= 0 || !info[0].IsString()) {
		Napi::TypeError::New(info.Env(), "Expected a string for service name").ThrowAsJavaScriptException();

		return;
	}

	this->service = new ServiceController(info[0].As<Napi::String>().Utf8Value());
}

Napi::Value WinSvcManager::GetServices(const Napi::CallbackInfo& info) {
	auto env = info.Env();
	auto promise = Napi::Promise::Deferred::New(env);
	auto array = Napi::Array::New(env);

	for(auto service : ServiceEnumerator::EnumerateServices()) {
		Napi::Object serviceStatus = Napi::Object::New(env);
		serviceStatus.Set("serviceType", Napi::Number::New(env, service.Status.dwServiceType));
		serviceStatus.Set("currentState", Napi::Number::New(env, service.Status.dwCurrentState));
		serviceStatus.Set("controlsAccepted", Napi::Number::New(env, service.Status.dwControlsAccepted));
		serviceStatus.Set("win32ExitCode", Napi::Number::New(env, service.Status.dwWin32ExitCode));
		serviceStatus.Set("serviceSpecificExitCode", Napi::Number::New(env, service.Status.dwServiceSpecificExitCode));
		serviceStatus.Set("checkPoint", Napi::Number::New(env, service.Status.dwCheckPoint));
		serviceStatus.Set("waitHint", Napi::Number::New(env, service.Status.dwWaitHint));

		Napi::Object serviceInfo = Napi::Object::New(env);
		serviceInfo.Set("displayName", Napi::String::New(env, service.DisplayName));
		serviceInfo.Set("serviceName", Napi::String::New(env, service.ServiceName));
		serviceInfo.Set("status", serviceStatus);

		PushToNapiArray(array, serviceInfo);
	}

	promise.Resolve(array);

	return promise.Promise();
}

Napi::Value WinSvcManager::CanPauseAndContinue(const Napi::CallbackInfo& info) {
	auto canPauseAndContinue = this->service->CanPauseContinue();

	cout << "Can pause and continue: " << canPauseAndContinue;

	return Napi::Boolean::New(info.Env(), canPauseAndContinue);
}

Napi::Promise WinSvcManager::Continue(const Napi::CallbackInfo& info) {
	auto env = info.Env();

	auto promise = Napi::Promise::Deferred::New(env);

	if(this->service->CanPauseContinue() && this->service->GetStatus() == ServiceStatus::Paused) {
		if(this->service->Continue()) {
			promise.Resolve(env.Undefined());
		} else {
			promise.Reject(Napi::Error::New(env, GetLastErrorString()).Value());
		}
	}

	return promise.Promise();
}

Napi::Value WinSvcManager::ChangeStartupType(const Napi::CallbackInfo& info) {
	auto env = info.Env();
	auto promise = Napi::Promise::Deferred::New(env);

	if(info.Length() <= 0 || !info[0].IsString()) {
		promise.Reject(Napi::TypeError::New(env, "Expected a string for the first argument").Value());
	} else {
		std::vector<string> startTypes { "Boot", "System", "Auto", "Demand", "Disabled" };

		auto startType = info[0].As<Napi::String>().Utf8Value();

		if(find(startTypes.begin(), startTypes.end(), startType) == startTypes.end()) {
			promise.Reject(Napi::TypeError::New(env, "Expected the first argument to be one of Auto, Boot, Disabled, Demand or System").Value());
		}

		ServiceStartType serviceStartType = StringToServiceStartType(startType);

		if(this->service->GetServiceConfig().ChangeStartType(serviceStartType)) {
			promise.Resolve(env.Undefined());
		} else {
			promise.Reject(Napi::Error::New(env, GetLastErrorString()).Value());
		}
	}

	return promise.Promise();
}

Napi::Object initAll(Napi::Env env, Napi::Object exports) {
	WinSvcManager::Init(env, exports);

	return exports;
}

NODE_API_MODULE(addon, initAll)