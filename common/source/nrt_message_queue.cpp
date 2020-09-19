#include "nrt_message_queue.h"
#include "error_category.h"

NRTMessageQueue::NRTMessageQueue(const string& name, const bool on_switch) :
	AbstractMessageQueue(name, on_switch) {
	string down_stream_queue_name = "/" + GetDownStreamName();
	string up_stream_queue_name = "/" + GetUpStreamName();
	struct mq_attr attribute = {};
	attribute.mq_flags = 0;
	attribute.mq_maxmsg = kMaxMessageNumber;
	attribute.mq_msgsize = kMaxMessageSize;
	attribute.mq_curmsgs = 0;

	down_stream_queue_descriptor_ = mq_open(down_stream_queue_name.c_str(), \
			O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, \
			&attribute);
	up_stream_queue_descriptor_ = mq_open(up_stream_queue_name.c_str(), \
			O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, \
			&attribute);
	if (down_stream_queue_descriptor_ < 0 || up_stream_queue_descriptor_ < 0)
		return;
	ready_ = true;
}

NRTMessageQueue::~NRTMessageQueue() {
	mq_close(down_stream_queue_descriptor_);
	mq_close(up_stream_queue_descriptor_);
}

error_condition NRTMessageQueue::Send(const Message& message) {
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
	int bytes = mq_receive(down_stream_queue_descriptor_, \
			reinterpret_cast<char*>(buffer.data()), buffer.size(), \
			nullptr);
	if (bytes < 0)
		return make_error_condition(errc(errno));
	buffer.resize(bytes);
	return message.ParseRawMessage(buffer);
}

error_condition NRTMessageQueue::GetReceivingMessageCount(unsigned int& count) {
	struct mq_attr attr;
	int result = mq_getattr(down_stream_queue_descriptor_, &attr);
	if (result < 0)
		return make_error_condition(errc(errno));
	count = attr.mq_curmsgs;
	return kNoError;
}

error_condition NRTMessageQueue::EmptyDownStream() {
	if (!ready_)
		return make_error_condition(errc::broken_pipe);
	unsigned int count;
	GetReceivingMessageCount(count);
	vector<uint8_t> buffer(kMaxMessageSize);
	for (int i = 0; i < count; i++) {
		int value = mq_receive(down_stream_queue_descriptor_, \
				reinterpret_cast<char*>(buffer.data()), buffer.size(), \
				nullptr);
		value = value + 1;
	}
	return make_error_condition(errc(0));
}
