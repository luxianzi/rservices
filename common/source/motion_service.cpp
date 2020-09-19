#include "motion_service.h"
#include "logger.h"

MotionService::MotionService() :
	nrt_thread_(),
	nrt_message_queue_(Config::kMotionServiceName),
	AbstractService(nrt_thread_, nrt_message_queue_) {
	Start();
}

MotionService::~MotionService() {

}

void MotionService::Work() {
	Message message;
	ReceiveMessage(message);
	int value;
	message.QueryItem("key", value);
	Logger logger;
	logger.Log(Logger::kInfo, "New message: " +  to_string(value), kCodeLocation);
}
