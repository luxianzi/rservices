#include <iostream>
#include <string>
#include <chrono>
#include "wheel_driver_service.h"

WheelDriverService::WheelDriverService() :
	thread_(),
	message_queue_(kWheelDriverServiceName),
	can_socket_(),
	AbstractService(thread_, message_queue_) {
	// FIXME: Use definition of keys
	SocketConfig config = {
		{"interface", "can0"},
		{"bit_rate", "1000000"},
		{"control_mode", "triple-sampling on"}
	};
	can_socket_.ReConfig(config);
}

WheelDriverService::~WheelDriverService() {
	// TODO Auto-generated destructor stub
}

void WheelDriverService::Work() {
	// TODO: Replace the following temporary code
	can_socket_.SetWriteCANID(0x200);
	vector<uint8_t> current(8);
	current[0] = 0x80;
	can_socket_.Write(current);
	this_thread::sleep_for(chrono::milliseconds(5));
}
