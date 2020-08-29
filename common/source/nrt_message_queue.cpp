/*
 * NRTMessageQueue.cpp
 *
 *  Created on: 2020��8��23��
 *      Author: Luxianzi
 */

#include "nrt_message_queue.h"
#include "error_category.h"

NRTMessageQueue::NRTMessageQueue(string& name) :
	AbstractMessageQueue(name),
	ready_(false) {
	string down_stream_queue_name = GetName() + kDownStreamSuffix;
	string up_stream_queue_name = GetName() + kDownStreamSuffix;
	struct mq_attr attribute;
	attribute.mq_flags = 0;
	attribute.mq_maxmsg = kMaxMessageNumber;
	attribute.mq_msgsize = kMaxMessageSize;
	attribute.mq_curmsgs = 0;

	down_stream_queue_descriptor_ = mq_open(down_stream_queue_name.c_str(),
		O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attribute);
	up_stream_queue_descriptor_ = mq_open(up_stream_queue_name.c_str(),
		O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &attribute);
	if (down_stream_queue_descriptor_ < 0 || up_stream_queue_descriptor_ < 0)
		return;
	ready_ = true;
}

NRTMessageQueue::~NRTMessageQueue() {
	mq_close(down_stream_queue_descriptor_);
	mq_close(up_stream_queue_descriptor_);
}

error_condition NRTMessageQueue::Send(Message& message) {
	if (!ready_)
		return make_error_condition(errc::broken_pipe);
	vector<uint8_t> buffer = message.GetRawMessage();
	if (mq_send(up_stream_queue_descriptor_, \
			reinterpret_cast<const char*>(buffer.data()), buffer.size(), \
			kNormalPriority) < 0)
		return make_error_condition(errc(errno));
	return make_error_condition(errc(0));
}

error_condition NRTMessageQueue::Receive(Message& message) {
	if (!ready_)
		return make_error_condition(errc::broken_pipe);
	vector<uint8_t> buffer(kMaxMessageSize);
	if (mq_receive(down_stream_queue_descriptor_, \
			reinterpret_cast<char*>(buffer.data()), buffer.size(), \
			nullptr) < 0)
		return make_error_condition(errc(errno));
	return message.ParseRawMessage(buffer);
}

error_condition NRTMessageQueue::EmptyDownStream() {
	if (!ready_)
		return make_error_condition(errc::broken_pipe);
	return make_error_condition(errc(0));
}
