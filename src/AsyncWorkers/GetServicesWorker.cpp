#pragma once

#include "GetServicesWorker.hpp"
#include "../ServiceEnumerator.cpp"

#include "../utils.cpp"

void GetServicesWorker::Execute() {
	for(auto service : ServiceEnumerator::EnumerateServices()) {
		serviceNames.push_back(service.ServiceName);
	}
}

void GetServicesWorker::OnError(Error const &error) {
	deferred.Reject(error.Value());
}

void GetServicesWorker::OnOK() {
	auto array = Array::New(Env());

	for(auto serviceName : serviceNames) {
		Utils::PushToNapiArray(array, serviceControllerConstructor->New({ String::New(Env(), serviceName) }));
	}

	deferred.Resolve(array);
}