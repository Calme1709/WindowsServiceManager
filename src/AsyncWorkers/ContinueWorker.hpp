#pragma once

#include <napi.h>

#include "../ServiceController.cpp"

using namespace std;
using namespace Napi;

class ContinueWorker: public AsyncWorker {
	public:
		ContinueWorker(Napi::Env env, Promise::Deferred deferred, ServiceController* service):
	        AsyncWorker(env), deferred(deferred), service(service) {}
		void Execute();
		void OnOK();
		void OnError(Error const &error);

	private:
        ServiceController* service;
		Promise::Deferred deferred;
		vector<string> serviceNames;
};
