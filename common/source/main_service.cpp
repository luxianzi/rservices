#include "main_service.h"

MainService::MainService() :
	nrt_thread_(),
	nrt_message_queue_(Config::kMainServiceName),
	AbstractService(nrt_thread_, nrt_message_queue_) {
	Start();

}

MainService::~MainService() {
	// TODO Auto-generated destructor stub
}


void MainService::Work() {

}
