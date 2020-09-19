#include "rservice.h"
#include "message_switch.h"
#include "motion_service.h"
#include "remote_control_service.h"

int main(int argc, char* argv[])
{
	MessageSwitch message_switch;
	MotionService motion_service;
	RemoteControlService remote_control_service("/dev/input/event5");

	message_switch.RegisterChannel(motion_service.GetMessageQueueName());
	message_switch.RegisterChannel(remote_control_service.GetMessageQueueName());

	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1000));
	}

	return 0;
}
