#ifndef REMOTE_CONTROL_SERVICE_H_
#define REMOTE_CONTROL_SERVICE_H_

#include "abstract_service.h"
#include "nrt_thread.h"
#include "nrt_message_queue.h"
#include "hid_device.h"

class RemoteControlService: public AbstractService {
public:
	RemoteControlService(const string& device_name);
	virtual ~RemoteControlService();
	virtual void Work();

private:
	NRTThread nrt_thread_;
	NRTMessageQueue nrt_message_queue_;
	HIDDevice hid_device_;
};

#endif /* REMOTE_CONTROL_SERVICE_H_ */
