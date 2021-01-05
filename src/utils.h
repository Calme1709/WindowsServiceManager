#pragma once

#include <napi.h>
#include <string>
#include "ServiceController.h"

using namespace std;
using namespace Napi;

class Utils {
public:
	static string GetLastErrorString();
	static ServiceStartType StringToServiceStartType(string startType);
	static void PushToNapiArray(Array array, Value value);
	static const char *c_str(const string *str);
};