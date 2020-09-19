#ifndef MOTION_SERVICE_H_
#define MOTION_SERVICE_H_

#include "rservice.h"
#include "abstract_service.h"
#include "nrt_thread.h"
#include "nrt_message_queue.h"
#include "robo_master_c620_device.h"

class MotionService: public AbstractService {
public:
	MotionService();
	virtual ~MotionService();
	virtual void Work();

private:
	NRTThread nrt_thread_;
	NRTMessageQueue nrt_message_queue_;
	RoboMasterC620Device robo_master_c620_device_;
};

#endif /* MOTION_SERVICE_H_ */
