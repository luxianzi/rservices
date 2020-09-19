#ifndef ABSTRACTMESSAGEQUEUE_H_
#define ABSTRACTMESSAGEQUEUE_H_

#include <stdint.h>
#include <system_error>
#include <vector>
#include "rservice.h"
#include "message.h"

class AbstractMessageQueue {
public:
	static const constexpr char* kDownStreamSuffix = "_in";
	static const constexpr char* kUpStreamSuffix = "_out";
	static const unsigned int kMaxMessageNumber = 8;
	static const unsigned int kMaxMessageSize = 4096;

	AbstractMessageQueue(const string& name, const bool on_switch = false) :
		name_(name),
		on_switch_(on_switch),
		ready_(false) {}
	virtual ~AbstractMessageQueue() {}

	virtual bool IsReady() { return ready_; }
	virtual error_condition Send(const Message& message) = 0;
	virtual error_condition Receive(Message& message) = 0;
	virtual error_condition EmptyDownStream() = 0;
	virtual error_condition GetReceivingMessageCount(unsigned int& count) = 0;
	virtual string GetName() const {
		return name_;
	}
	virtual string GetUpStreamName() {
		return name_ + (on_switch_ ? kDownStreamSuffix : kUpStreamSuffix);
	}
	virtual string GetDownStreamName() {
		return name_ + (on_switch_ ? kUpStreamSuffix : kDownStreamSuffix);
	}

protected:
	bool on_switch_;
	bool ready_;

private:
	string name_;

};

#endif /* ABSTRACTMESSAGEQUEUE_H_ */
