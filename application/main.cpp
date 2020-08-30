#include <iostream>
#include <chrono>
#include "message.h"
#include "nrt_thread.h"
#include "wheel_driver_service.h"

using namespace std;

int main(int argc, char* argv[])
{
	// TODO: Replace the following temporary code
//	Message m;
//	vector<uint8_t> buffer;
//
//	buffer = m.GetRawMessage();
//	m.ParseRawMessage(buffer);

	WheelDriverService wds;

	wds.Start();
	this_thread::sleep_for(chrono::seconds(1));
	wds.Stop();

	return 0;
}
