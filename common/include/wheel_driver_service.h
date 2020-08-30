#ifndef WHEEL_DRIVER_SERVICE_H_
#define WHEEL_DRIVER_SERVICE_H_

#include "abstract_service.h"
#include "nrt_thread.h"
#include "nrt_message_queue.h"
#include "can_socket.h"

using namespace std;

class WheelDriverService: public AbstractService {
public:
	static const constexpr char* kWheelDriverServiceName = "wheel_driver_service";

	WheelDriverService();
	virtual ~WheelDriverService();

	virtual void Work();

private:
	NRTThread thread_;
	NRTMessageQueue message_queue_;
	CANSocket can_socket_;

};

#endif /* WHEEL_DRIVER_SERVICE_H_ */
