#include "ServiceEnumerator.h"
#include "utils.cpp"
#include <napi.h>

using namespace std;

class WinSvcManager : public Napi::ObjectWrap<WinSvcManager> {
public:
	static Napi::Object Init(Napi::Env env, Napi::Object exports);
	static Napi::Value GetServices(const Napi::CallbackInfo& info);
	WinSvcManager(const Napi::CallbackInfo& info);

private:
	Napi::Value CanPauseAndContinue(const Napi::CallbackInfo& info);

	ServiceController* service;

	Napi::Value ChangeStartupType(const Napi::CallbackInfo& info);
	Napi::Promise Continue(const Napi::CallbackInfo& info);
	Napi::Value GetInfo(const Napi::CallbackInfo& info);
	Napi::Value Pause(const Napi::CallbackInfo& info);
	Napi::Value Start(const Napi::CallbackInfo& info);
	Napi::Value Stop(const Napi::CallbackInfo& info);
	Napi::Value WaitForStatus(const Napi::CallbackInfo& info);
};