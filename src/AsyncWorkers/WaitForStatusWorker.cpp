#pragma once

#include "WaitForStatusWorker.hpp"

#include "../utils.cpp"

void WaitForStatusWorker::Execute() {
	auto result = service->WaitForStatus(desiredStatus, timeout);

	if(!result) {
		SetError(Utils::GetLastErrorString());
	}
}

void WaitForStatusWorker::OnError(Error const &error) {
	deferred.Reject(error.Value());
}

void WaitForStatusWorker::OnOK() {
	deferred.Resolve(Env().Undefined());
}