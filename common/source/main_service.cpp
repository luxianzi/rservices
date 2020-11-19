#include "main_service.h"

MainService::MainService() :
	work_mode_(kManualMode),
	nrt_thread_(),
	nrt_message_queue_(Config::kMainServiceName),
	AbstractService(nrt_thread_, nrt_message_queue_) {
	work_fsm_.start();
	work_fsm_.process_event(EventInit(work_mode_, kManualMode));
	Start();

}

MainService::~MainService() {
	// TODO Auto-generated destructor stub
}


void MainService::Work() {
	int work_mode_recv = work_mode_;
	uint32_t count;
	Message message;

	// 1. Get message
	nrt_message_queue_.GetReceivingMessageCount(count);
	if (count > 0)
	{
		if (ReceiveMessage(message) != kNoError)
			message.QueryItem("work_mode", work_mode_recv);
	}

	// 2. call Event process
	work_fsm_.process_event(EventSwitchManual(work_mode_, static_cast<WorkMode>(work_mode_recv)));
	work_fsm_.process_event(EventSwitchAuto(work_mode_, static_cast<WorkMode>(work_mode_recv)));
	work_fsm_.process_event(EventAutoWork(work_mode_, static_cast<WorkMode>(work_mode_recv)));
}
