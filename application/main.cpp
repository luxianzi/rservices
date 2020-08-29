#include <iostream>
#include <chrono>
#include "message.h"
#include "nrt_thread.h"

using namespace std;

int main(int argc, char* argv[])
{
	Message m;
	vector<uint8_t> buffer;

	m.ParseRawMessage(buffer);

	return 0;
}
