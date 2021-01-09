#pragma once

#include <napi.h>
#include <string>
#include "ServiceConstants.hpp"

using namespace std;
using namespace Napi;

class Utils {
public:
	static string GetLastErrorString();
	static ServiceStartType StringToServiceStartType(const string startType);
	static void PushToNapiArray(Array array, const Value value);
	static const char *c_str(const string *str);
	static std::vector<string> Utils::SplitDoubleNullTerminatedString(const LPSTR text);
};