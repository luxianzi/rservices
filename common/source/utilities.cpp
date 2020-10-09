#include <ctype.h>
#include "utilities.h"

string to_lower(const string& str) {
	string result;
	for (auto c : str)
		result.push_back(tolower(c));
	return result;
}

