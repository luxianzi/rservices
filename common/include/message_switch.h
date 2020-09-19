#ifndef MESSAGE_SWITCH_H_
#define MESSAGE_SWITCH_H_

#include <mqueue.h>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include "error_category.h"
#include "nrt_thread.h"
#include "nrt_message_queue.h"

using namespace std;

class MessageSwitch {
public:
	MessageSwitch();
	virtual ~MessageSwitch();

	virtual error_condition RegisterChannel(const string& name);
	virtual void NRTRoutine();

private:
	vector<unique_ptr<AbstractMessageQueue>> message_queues_;
	NRTThread nrt_routine_;
	mutex maintenance_;
};

#endif /* MESSAGE_SWITCH_H_ */
