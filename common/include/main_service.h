#ifndef MAIN_SERVICE_H_
#define MAIN_SERVICE_H_

#include "rservice.h"
#include "abstract_service.h"
#include "nrt_thread.h"
#include "nrt_message_queue.h"
//#include "main_service_fsm.h"

class MainService : public AbstractService {
public:
	MainService();
	virtual ~MainService();
	virtual void Work();

private:
	NRTThread nrt_thread_;
	NRTMessageQueue nrt_message_queue_;
};

#endif /* MAIN_SERVICE_H_ */
