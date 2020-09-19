#include <cmath>
#include "remote_control_service.h"

RemoteControlService::RemoteControlService(const string& device_name) :
	nrt_thread_(),
	nrt_message_queue_(Config::kRemoteControlServerName),
	hid_device_(device_name),
	AbstractService(nrt_thread_, nrt_message_queue_) {
	Start();
}

RemoteControlService::~RemoteControlService() {

}

void RemoteControlService::Work() {
	unsigned int result = 0;
	unsigned int rx, ry;
	hid_device_.Get(HIDDevice::kKeyUpdated, result);
	hid_device_.Get(hid_device_.FindAbsName(ABS_HAT0X), rx);
	hid_device_.Get(hid_device_.FindAbsName(ABS_HAT0Y), ry);
	int x = static_cast<int>(rx);
	int y = static_cast<int>(-ry);
	int angle;
	int speed = 100;
	if (x == 0 && y == 0) {
		angle = 0;
		speed = 0;
	}
	else if (x == 0)
		angle = y > 0 ? 90 : -90;
	else if (x > 0)
		angle = atan(y / x) / M_PI * 180;
	else if (x < 0) {
		angle = atan(y / x) / M_PI * 180 + 180;
		angle = angle > 180 ? -(360 - angle) : angle;
	}
	Message message;
	message.SetSource(Config::kRemoteControlServerName);
	message.SetDestination(Config::kMotionServiceName);
	message.SetType(Message::kRequest);
	message.SetItem("angle", angle);
	message.SetItem("speed", speed);
	SendMessage(message);
}
