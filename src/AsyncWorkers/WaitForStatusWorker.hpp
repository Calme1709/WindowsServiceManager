#pragma once

#include <napi.h>

#include "../ServiceController.cpp"

using namespace std;
using namespace Napi;

class WaitForStatusWorker: public AsyncWorker {
	public:
		WaitForStatusWorker(Napi::Env env, ServiceController* service, Promise::Deferred deferred, ServiceStatus desiredStatus, chrono::milliseconds timeout): 
            AsyncWorker(env), service(service), deferred(deferred), desiredStatus(desiredStatus), timeout(timeout) {}
		void Execute();
		void OnOK();
		void OnError(Error const &error);

	private:
		ServiceController* service;
		Promise::Deferred deferred;
        ServiceStatus desiredStatus;
		chrono::milliseconds timeout;
};
