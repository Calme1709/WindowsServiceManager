#pragma once

#include "./ContinueWorker.hpp"

#include "../utils.cpp"

void ContinueWorker::Execute() {
    if(!service->CanAcceptControl(ServiceControls::PauseAndContinue) || service->GetStatus() != ServiceStatus::Paused) {
        return;
    }

    if(!service->Continue()){
        SetError(Utils::GetLastErrorString());
    }
}

void ContinueWorker::OnError(Error const &error) {
	deferred.Reject(error.Value());
}

void ContinueWorker::OnOK() {
	deferred.Resolve(Env().Undefined());
}