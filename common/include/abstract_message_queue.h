#ifndef ABSTRACTMESSAGEQUEUE_H_
#define ABSTRACTMESSAGEQUEUE_H_

#include <stdint.h>
#include <system_error>
#include <vector>
#include "message.h"

using namespace std;

class AbstractMessageQueue {
public:
	static const constexpr char* kDownStreamSuffix = "_in";
	static const constexpr char* kUpStreamSuffix = "_out";
	static const unsigned int kMaxMessageNumber = 16;
	static const unsigned int kMaxMessageSize = 4096;

	AbstractMessageQueue(const string& name) :
		name_(name) {}
	virtual ~AbstractMessageQueue() {}

	virtual error_condition Send(const Message& message) = 0;
	virtual error_condition Receive(Message& message) = 0;
	virtual error_condition EmptyDownStream() = 0;
	virtual string GetUpStreamName() {
		return name_ + kUpStreamSuffix;
	}
	virtual string GetDownStreamName() {
		return name_ + kDownStreamSuffix;
	}

private:
	string name_;

};

#endif /* ABSTRACTMESSAGEQUEUE_H_ */
