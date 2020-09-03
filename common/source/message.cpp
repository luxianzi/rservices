#include <algorithm>
#include "message.h"
#include "message_parse_fsm.h"

Message::Message() {
	SetBroadcasting(false);
	SetType(kInvalid);
	SetSource(kUnknown);
	SetDestination(kUnknown);
	SetID(0);
}

Message::Message(const vector<uint8_t>& buffer) {
	Message();
	ParseRawMessage(buffer);
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}

error_condition Message::SetItem(const string key, const int value, \
		const bool force) {
	if (!force && IsReservedKey(key))
		return make_error_condition(ErrorCode::kKeyReserved);
	MessageItem item(key, kInteger, CastToVector(value));
	SetOrReplaceItem(item);
	return kNoError;
}

error_condition Message::SetItem(const string key, const bool value, \
		const bool force) {
	if (!force && IsReservedKey(key))
		return make_error_condition(ErrorCode::kKeyReserved);
	MessageItem item(key, kBool, CastToVector(value));
	SetOrReplaceItem(item);
	return kNoError;
}

error_condition Message::SetItem(const string key, const double value, \
		const bool force) {
	if (!force && IsReservedKey(key))
		return make_error_condition(ErrorCode::kKeyReserved);
	MessageItem item(key, kDouble, CastToVector(value));
	SetOrReplaceItem(item);
	return kNoError;
}

error_condition Message::SetItem(const string key, const string& value, \
		const bool force) {
	if (!force && IsReservedKey(key))
		return make_error_condition(ErrorCode::kKeyReserved);
	MessageItem item(key, kString, CastToVector(value));
	SetOrReplaceItem(item);
	return kNoError;
}

error_condition Message::SetItem(const string key, \
		const vector<uint8_t>& value, const bool force) {
	if (!force && IsReservedKey(key))
		return make_error_condition(ErrorCode::kKeyReserved);
	MessageItem item(key, kData, value);
	SetOrReplaceItem(item);
	return kNoError;
}

error_condition Message::QueryItem(const string key, int& result) {
	MessageItem item;
	if (!FindAndCheckItem(key, kInteger, sizeof(int), item))
		return make_error_condition(errc::invalid_argument);
	copy(get<2>(item).begin(), get<2>(item).begin() + sizeof(int), &result);
	return kNoError;
}

error_condition Message::QueryItem(const string key, bool& result) {
	MessageItem item;
	if (!FindAndCheckItem(key, kBool, sizeof(uint8_t), item))
		return make_error_condition(errc::invalid_argument);
	result = get<2>(item)[0] != 0;
	return kNoError;
}

error_condition Message::QueryItem(const string key, double& result) {
	MessageItem item;
	if (!FindAndCheckItem(key, kDouble, sizeof(double), item))
		return make_error_condition(errc::invalid_argument);
	copy(get<2>(item).begin(), get<2>(item).begin() + sizeof(double), &result);
	return kNoError;
}

error_condition Message::QueryItem(const string key, string& result) {
	MessageItem item;
	if (!FindAndCheckItem(key, kString, 0, item))
		return make_error_condition(errc::invalid_argument);
	result.resize(get<2>(item).size());
	copy(get<2>(item).begin(), get<2>(item).end(), result.begin());
	return kNoError;
}

error_condition Message::QueryItem(const string key, vector<uint8_t>& result) {
	MessageItem item;
	if (!FindAndCheckItem(key, kData, 0, item))
		return make_error_condition(errc::invalid_argument);
	result.resize(get<2>(item).size());
	copy(get<2>(item).begin(), get<2>(item).end(), result.begin());
	return kNoError;
}

void Message::SetBroadcasting(const bool value) {
	SetItem(kBroadcasting, value, true);
}

void Message::SetType(const Type value) {
	SetItem(kType, value, true);
}

void Message::SetSource(const string& value) {
	SetItem(kSource, value, true);

}

void Message::SetDestination(const string& value) {
	SetItem(kDestination, value, true);
}

void Message::SetID(const int value) {
	SetItem(kID, value, true);
}

bool Message::IsBroadcasting() {
	// TODO: Add query fail log output
	bool result = false;
	QueryItem(kBroadcasting, result);
	return result;
}

Message::Type Message::GetType() {
	// TODO: Add query fail log output
	int result = kInvalid;
	QueryItem(kType, result);
	return static_cast<Type>(result);
}

string Message::GetSource() {
	// TODO: Add query fail log output
	string result = kUnknown;
	QueryItem(kSource, result);
	return result;
}

string Message::GetDestination() {
	// TODO: Add query fail log output
	string result = kUnknown;
	QueryItem(kDestination, result);
	return result;
}

int Message::GetID() {
	// TODO: Add query fail log output
	int result = 0;
	QueryItem(kID, result);
	return result;
}

// Layout of a single raw message item:
//     key_length: uint8_t
//     key: array of uint8_t
//     parameter_type: uint8_t
//     data_length: uint32_t
//     data: array of uint8_t

vector<uint8_t> Message::GetRawMessage() const {
	vector<uint8_t> buffer;
	for (auto iter : items_) {
		string key = get<0>(iter);
		char parameter_type = get<1>(iter);
		vector<uint8_t> data = get<2>(iter);
		uint32_t data_length = data.size();
		buffer.push_back(key.size() < 255 ? key.size() : 255);
		buffer.insert(buffer.end(), key.begin(), key.end());
		buffer.push_back(parameter_type);
		buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&data_length), \
				reinterpret_cast<uint8_t*>(&data_length) + sizeof(uint32_t));
		buffer.insert(buffer.end(), data.begin(), data.end());
	}
	return buffer;
}

error_condition Message::ParseRawMessage(const vector<uint8_t>& buffer) {
	MessageParseFSM fsm;

	fsm.start();
	fsm.process_event(EventInit(buffer, items_));
	msmf::state<>* state;
	do {
		fsm.process_event(EventTransit());
		state = static_cast<msmf::state<>*>(fsm.get_state_by_id(*fsm.current_state()));
	} while (state == fsm.get_state<MessageParseFSM::StateGetMessageItem*>());

	if (state == fsm.get_state<MessageParseFSM::StateError*>())
		return make_error_condition(ErrorCode::kMessageParseError);

	return kNoError;
}

vector<uint8_t> Message::CastToVector(const int value) {
	vector<uint8_t> result(sizeof(int));
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(&value);
	copy(buffer, buffer + sizeof(int), result.begin());
	return result;
}
vector<uint8_t> Message::CastToVector(const bool value) {
	vector<uint8_t> result(sizeof(uint8_t));
	result[0] = static_cast<uint8_t>(value);
	return result;
}

vector<uint8_t> Message::CastToVector(const double value) {
	vector<uint8_t> result(sizeof(double));
	const uint8_t* buffer = reinterpret_cast<const uint8_t*>(&value);
	copy(buffer, buffer + sizeof(double), result.begin());
	return result;
}

vector<uint8_t> Message::CastToVector(const string& value) {
	vector<uint8_t> result(value.size());
	copy(value.begin(), value.end(), result.begin());
	result.push_back(0);
	return result;
}

bool Message::FindAndCheckItem(const string& key, const ParameterType type, \
		const size_t size, MessageItem& result) {
	// TODO: Add locks to enable multi-threading
	auto it = find_if(items_.begin(), items_.end(), \
			[&](MessageItem& i){ return get<0>(i) == key; });
	if (it == items_.end())
		return false;
	if (get<1>(*it) != type)
		return false;
	if (get<2>(*it).size() < size)
		return false;
	get<0>(result) = get<0>(*it);
	get<1>(result) = get<1>(*it);
	get<2>(result) = get<2>(*it);
	return true;
}

void Message::SetOrReplaceItem(const MessageItem& item) {
	// TODO: Add locks to enable multi-threading
	auto it = find_if(items_.begin(), items_.end(), \
			[&](MessageItem& i){ return get<0>(i) == get<0>(item); });
	if (it == items_.end())
		items_.push_back(item);
	else
		*it = item;
}

bool Message::IsReservedKey(const string& key) {
	auto it = find(kReservedKeys.begin(), kReservedKeys.end(), key);
	return it != kReservedKeys.end();
}
