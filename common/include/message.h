#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <system_error>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

class Message {
public:
	enum Type {
		kInvalid,
		kRequest,
		kResponse,
		kEvent
	};
	enum ParameterType {
		kUndefined,
		kInteger,
		kBool,
		kDouble,
		kString,
		kData
	};

	using MessageItem = tuple<string, ParameterType, vector<uint8_t>>;

	Message();
	Message(const vector<uint8_t>& buffer);
	virtual ~Message();

	virtual error_condition SetItem(const string key, const int value, \
			const bool force = false);
	virtual error_condition SetItem(const string key, const bool value, \
			const bool force = false);
	virtual error_condition SetItem(const string key, const double value, \
			const bool force = false);
	virtual error_condition SetItem(const string key, const string& value, \
			const bool force = false);
	virtual error_condition SetItem(const string key, \
			const vector<uint8_t>& value, const bool force = false);

	virtual error_condition QueryItem(const string key, int& result);
	virtual error_condition QueryItem(const string key, bool& result);
	virtual error_condition QueryItem(const string key, double& result);
	virtual error_condition QueryItem(const string key, string& result);
	virtual error_condition QueryItem(const string key, vector<uint8_t>& result);

	virtual void SetBroadcasting(const bool value);
	virtual void SetType(const Type value);
	virtual void SetSource(const string& value);
	virtual void SetDestination(const string& value);
	virtual void SetID(const int value);

	virtual bool IsBroadcasting();
	virtual Type GetType();
	virtual string GetSource();
	virtual string GetDestination();
	virtual int GetID();

	virtual vector<uint8_t> GetRawMessage() const;
	virtual error_condition ParseRawMessage(const vector<uint8_t>& buffer);

protected:
	virtual vector<uint8_t> CastToVector(const int value);
	virtual vector<uint8_t> CastToVector(const bool value);
	virtual vector<uint8_t> CastToVector(const double value);
	virtual vector<uint8_t> CastToVector(const string& value);
	virtual bool FindAndCheckItem(const string& key, const ParameterType type, const size_t size, MessageItem& result);
	virtual void SetOrReplaceItem(const MessageItem& item);
	virtual bool IsReservedKey(const string& key);

private:
	static const constexpr char* kUnknown = "unknown";
	static const constexpr char* kBroadcasting = "broadcasting";
	static const constexpr char* kType = "type";
	static const constexpr char* kSource = "source";
	static const constexpr char* kDestination = "destination";
	static const constexpr char* kID = "id";
	const vector<string> kReservedKeys = {
		kBroadcasting,
		kType,
		kSource,
		kDestination,
		kID
	};

	vector<MessageItem> items_;
};

#endif /* MESSAGE_H_ */
