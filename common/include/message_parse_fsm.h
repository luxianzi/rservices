#ifndef MESSAGE_PARSE_FSM_H_
#define MESSAGE_PARSE_FSM_H_

#ifndef MESSAGE_H_
#error Please include message.h before message_parse_fsm.h
#endif

#include <stdint.h>
#include <iostream>
#include <vector>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

// TODO: Change the state machine definition to UML

using namespace std;
namespace msmf = boost::msm::front;
namespace msmb = boost::msm::back;
namespace mpl = boost::mpl;

static const vector<Message::MessageItem> kDummyMessageItems = {};

struct EventInit {
	EventInit(const vector<uint8_t>& buffer, \
			vector<Message::MessageItem>& items) :
		buffer_(buffer),
		items_(items) {}
	vector<uint8_t> buffer_;
	vector<Message::MessageItem>& items_;
};

struct EventTransit {};

struct MessageParseFSMFront:\
		msmf::state_machine_def<MessageParseFSMFront> {

	MessageParseFSMFront() :
		has_error_(false),
		end_of_buffer_(false),
		buffer_position_(0),
		buffer_({}),
		items_(const_cast<vector<Message::MessageItem>*>(&kDummyMessageItems)) {}

	struct StateInit:msmf::state<> {};
	struct StateGetMessageItem:msmf::state<> {};
	struct StateFinish:msmf::state<> {};
	struct StateError:msmf::state<> {};

	typedef StateInit initial_state;

	static bool GetBufferFromBuffer(const vector<uint8_t>& buffer, \
			const unsigned int position, const size_t size, \
			vector<uint8_t>& result) {
		if (position + size > buffer.size())
			return false;
		result.resize(size);
		if (size == 0)
			return true;
		copy(&buffer[position], &buffer[position + size], result.begin());
		return true;
	}

	static bool GetIntFromBuffer(const vector<uint8_t>& buffer, \
			const unsigned int position, const size_t size, unsigned int& result) {
		vector<uint8_t> value;
		if (!GetBufferFromBuffer(buffer, position, size, value))
			return false;
		result = 0;
		copy(value.begin(), value.end(), reinterpret_cast<uint8_t*>(&result));
		return true;
	}

	static bool GetStringFromBuffer(const vector<uint8_t>& buffer, \
			const unsigned int position, const size_t size, string& result) {
		vector<uint8_t> value;
		if (!GetBufferFromBuffer(buffer, position, size, value))
			return false;
		result.resize(size);
		copy(value.begin(), value.end(), result.begin());
		return true;
	}

	struct StateInitAction {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		void operator()(Event const& event, Fsm& fsm, SourceState&, \
				TargetState&) const {
			fsm.has_error_ = false;
			fsm.end_of_buffer_ = false;
			fsm.buffer_position_ = 0;
			fsm.buffer_ = event.buffer_;
			fsm.items_ = &event.items_;
			fsm.items_->clear();
		}
	};

	struct StateGetMessageItemAction {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		void operator()(Event const&, Fsm& fsm, SourceState&, \
				TargetState&) const {
			unsigned int size;
			string key;
			unsigned int type;
			vector<uint8_t> data;
			fsm.has_error_ = !(GetIntFromBuffer(fsm.buffer_, \
					fsm.buffer_position_++, sizeof(uint8_t), size) && \
					GetStringFromBuffer(fsm.buffer_, \
					fsm.buffer_position_, size, key) && \
					(fsm.buffer_position_ += size) > 0 && \
					GetIntFromBuffer(fsm.buffer_, \
					fsm.buffer_position_++, sizeof(uint8_t), type) && \
					GetIntFromBuffer(fsm.buffer_, \
					fsm.buffer_position_, sizeof(uint32_t), size) && \
					(fsm.buffer_position_ += sizeof(uint32_t)) > 0 && \
					GetBufferFromBuffer(fsm.buffer_, \
					fsm.buffer_position_, size, data) && \
					(fsm.buffer_position_ += size) > 0);
			if (!fsm.has_error_)
				fsm.items_->emplace_back(key, \
						static_cast<Message::ParameterType>(type), data);
			fsm.end_of_buffer_ = fsm.buffer_position_ == fsm.buffer_.size();
		}
	};

	struct GuardHasError {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		bool operator()(Event const&, Fsm& fsm, SourceState&, TargetState&) const {
			return fsm.has_error_;
		}
	};

	struct GuardEndOfBuffer {
		template<class Event, class Fsm, class SourceState, \
				class TargetState>
		bool operator()(Event const&, Fsm& fsm, SourceState&, TargetState&) const {
			return fsm.end_of_buffer_;
		}
	};

	struct transition_table:mpl::vector<
		// Row content: Source, Event, Next, Action, Guard
		msmf::Row<StateInit, EventInit, StateInit, \
			StateInitAction, msmf::none>,
		msmf::Row<StateInit, EventTransit, StateGetMessageItem, \
			StateGetMessageItemAction, msmf::none>,
		msmf::Row<StateGetMessageItem, EventTransit, msmf::none, \
				StateGetMessageItemAction, msmf::none>,
		msmf::Row<StateGetMessageItem, EventTransit, StateError, \
				msmf::none, GuardHasError>,
		msmf::Row<StateGetMessageItem, EventTransit, StateFinish, \
				msmf::none, GuardEndOfBuffer>,
		msmf::Row<StateError, EventTransit, msmf::none, \
				msmf::none, msmf::none>,
		msmf::Row<StateFinish, EventTransit, msmf::none, \
				msmf::none, msmf::none>
	> {};

private:
	bool has_error_;
	bool end_of_buffer_;
	int buffer_position_;
	vector<uint8_t> buffer_;
	// TODO: use share_ptr to manage items_ pointer
	vector<Message::MessageItem>* items_;
};

typedef msmb::state_machine<MessageParseFSMFront> MessageParseFSM;
#endif
