#ifndef ABSTRACTSOCKET_H_
#define ABSTRACTSOCKET_H_

#include <string>
#include <map>
#include <vector>
#include <atomic>
#include "rservice.h"
#include "error_category.h"

class AbstractSocket {
public:
	AbstractSocket() :
		connected_(false) {}
	AbstractSocket(CommonKeyPairs& config) {}
	virtual ~AbstractSocket() {};

	virtual error_condition	ReConfig(const CommonKeyPairs& config) = 0;
	virtual error_condition DeConfig() = 0;
	virtual error_condition Read(vector<uint8_t>& buffer) = 0;
	virtual error_condition Write(const vector<uint8_t>& buffer) = 0;
	virtual bool IsConnected() { return connected_.load(); }
	virtual void SetConnected() { connected_.store(true); }
	virtual void ClearConnected() { connected_.store(false); }

private:
	atomic_bool connected_;
};

#endif /* ABSTRACTSOCKET_H_ */
