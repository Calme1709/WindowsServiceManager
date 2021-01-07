#pragma once

#include <napi.h>

using namespace std;
using namespace Napi;

class GetServicesWorker: public AsyncWorker {
	public:
		GetServicesWorker(Napi::Env env, Promise::Deferred deferred, FunctionReference* serviceControllerConstructor): 
            AsyncWorker(env), deferred(deferred), serviceControllerConstructor(serviceControllerConstructor) {}
		void Execute();
		void OnOK();
		void OnError(Error const &error);

	private:
		FunctionReference* serviceControllerConstructor;
		Promise::Deferred deferred;
		vector<string> serviceNames;
};
