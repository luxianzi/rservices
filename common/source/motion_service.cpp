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
	int angle, speed;
	message.QueryItem("angle", angle);
	message.QueryItem("speed", speed);
	Logger logger;
	logger.Log(Logger::kInfo, "Angle: " + to_string(angle) + ", Speed: " + \
			to_string(speed));
}
