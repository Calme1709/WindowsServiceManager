#pragma once

#include "utils.hpp"

void Utils::PushToNapiArray(Array array, const Value value) {
    array.Set(array.Length(), value);
}

const char * Utils::c_str(const string *str) {
    return str == nullptr ? nullptr : str->c_str();
}

ServiceStartType Utils::StringToServiceStartType(const string startType) {
	if(startType == "Boot") return ServiceStartType::Boot;
	if(startType == "System") return ServiceStartType::System;
	if(startType == "Auto") return ServiceStartType::Auto;
	if(startType == "Demand") return ServiceStartType::Demand;
	
	return ServiceStartType::Disabled;
}

string Utils::GetLastErrorString() {
	//Get the error message, if any.
    DWORD errorMessageID = GetLastError();
    if(errorMessageID == 0){
        return std::string(); //No error message has been recorded
	}

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

std::vector<string> Utils::SplitDoubleNullTerminatedString(const LPSTR text) {
	std::vector<string> texts;
	LPSTR ptr = text;
	do {
		texts.push_back(ptr);
		ptr += texts.back().size() + 1;
	} while (*ptr != '\0');

	return texts;
}