#include "remote_control_service.h"

RemoteControlService::RemoteControlService(const string& device_file_name) :
	nrt_thread_(),
	nrt_message_queue_(Config::kRemoteControlServerName),
	hid_device_(device_file_name),
	AbstractService(nrt_thread_, nrt_message_queue_) {
	Start();
}

RemoteControlService::~RemoteControlService() {

}

void RemoteControlService::Work() {
	Message message;
	message.SetSource(Config::kRemoteControlServerName);
	message.SetDestination(Config::kMotionServiceName);
	message.SetType(Message::kRequest);
	message.SetItem("key", 100);
	SendMessage(message);
	this_thread::sleep_for(chrono::milliseconds(10));
}
