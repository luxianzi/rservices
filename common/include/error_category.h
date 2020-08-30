#ifndef ERRORCATEGORY_H_
#define ERRORCATEGORY_H_

#include <system_error>

using namespace std;

enum class ErrorCode {
	kSuccess = 0,
	kKeyReserved,
	kMessageQueueNotReady,
	kMessageParseError,
	kInterfaceNotSpecified,
	kBitRateNotSpecified,
	kControlModeNotSpecified,
	kLast
};
namespace std {
	template<> struct is_error_condition_enum<ErrorCode> : public true_type {};
}

class ErrorCategory: public error_category {
public:
	virtual const char* name() const noexcept { return "CR"; }
	virtual std::string message(int ev) const {
		switch (static_cast<ErrorCode>(ev)) {
			case ErrorCode::kSuccess:
				return "No error";
			case ErrorCode::kKeyReserved:
				return "Key is reserved";
			case ErrorCode::kMessageQueueNotReady:
				return "Message queue not ready";
			case ErrorCode::kMessageParseError:
				return "Message parse error";
			case ErrorCode::kInterfaceNotSpecified:
				return "Interface not specified";
			case ErrorCode::kBitRateNotSpecified:
				return "Bit rate not specified";
			case ErrorCode::kControlModeNotSpecified:
				return "Control mode not specified";
			default:
				return "Unknown error";
		}
	}
};

static const ErrorCategory kErrorCategory;
static const error_condition kNoError(errc(0));

__attribute__((weak)) error_condition make_error_condition(ErrorCode e) {
	return error_condition(static_cast<int>(e), kErrorCategory);
}

#endif /* ERRORCATEGORY_H_ */
