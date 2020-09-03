#ifndef ABSTRACT_SOCKET_DEVICE_H_
#define ABSTRACT_SOCKET_DEVICE_H_

#include "rservice.h"
#include "abstract_device.h"
#include "abstract_socket.h"

class AbstractSocketDevice : public AbstractDevice {
public:
	AbstractSocketDevice(AbstractSocket& socket) :
		socket_(socket) { }
	virtual ~AbstractSocketDevice() { }

	virtual error_condition ReConfig(const CommonKeyPairs& config) {
		return socket_.ReConfig(config);
	}
	virtual error_condition DeConfig() {
		return socket_.DeConfig();
	}

private:
	AbstractSocket& socket_;
};

#endif /* ABSTRACT_SOCKET_DEVICE_H_ */
