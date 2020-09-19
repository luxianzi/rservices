#include <mqueue.h>
#include <algorithm>
#include <typeinfo>
#include "message.h"
#include "message_switch.h"
#include "nrt_message_queue.h"
#include "abstract_message_queue.h"

MessageSwitch::MessageSwitch() :
	nrt_routine_([&](){ this->NRTRoutine(); }) {
	nrt_routine_.Start();
}

MessageSwitch::~MessageSwitch() {
	nrt_routine_.Stop();
}

error_condition MessageSwitch::RegisterChannel(const string& name) {
	lock_guard<mutex> lock(maintenance_);
	message_queues_.emplace_back(new NRTMessageQueue(name, true));
	if (!message_queues_.back()->IsReady()) {
		message_queues_.pop_back();
		return make_error_condition(errc::broken_pipe);
	}
	message_queues_.back()->EmptyDownStream();
	return kNoError;
}

void MessageSwitch::NRTRoutine() {
	// TODO: use select to replace sleep
	this_thread::sleep_for(chrono::milliseconds(10));
	lock_guard<mutex> lock(maintenance_);
	for (auto& source : message_queues_) {
		if (typeid(*source) != typeid(NRTMessageQueue))
			continue;
		unsigned int message_count = 0;
		if (source->GetReceivingMessageCount(message_count) != kNoError)
			continue;
		for (int i = 0; i < message_count; i++) {
			Message message;
			if (source->Receive(message) != kNoError)
				continue;
			if (message.IsBroadcasting()) {
				for (auto& target : message_queues_) {
					if (target->GetName() == source->GetName())
						continue;
					target->Send(message);
				}
			}
			else {
				// FIXME: direct compare of names fails, need to use c_str;
				auto target = find_if(message_queues_.begin(), \
						message_queues_.end(), \
						[&](const unique_ptr<AbstractMessageQueue>& mq) \
						{ return string(mq->GetName().c_str()) == \
								string(message.GetDestination().c_str()); });
				if (target != message_queues_.end())
					(*target)->Send(message);
			}
		}
	}
}
