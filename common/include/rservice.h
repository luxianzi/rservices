#ifndef RSERVICE_H_
#define RSERVICE_H_

#include <map>
#include <string>
#include "utilities.h"

using namespace std;
using CommonKeyPairs = map<string, string>;

namespace Config {
	static const constexpr char* kMainServiceName = "main";
	static const constexpr char* kMotionServiceName = "motion";
	static const constexpr char* kRemoteControlServerName = "remote_control";
};

#endif /* RSERVICE_H_ */
