#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include "file_factory.h"

#define kCodeLocation (string("In function \"") + __func__ + "\" of " + \
		__FILE__ + ":" + to_string(__LINE__) + ", ")

using namespace std;

class Logger {
public:
	enum Level {
		kDebug,
		kInfo,
		kWarning,
		kError
	};

	Logger();
	Logger(const string& file_name);
	Logger(const string& file_name, const FileFactory& file_factory);
	virtual error_condition Init(const string& file_name);
	virtual ~Logger();

	virtual void Log(const Level level, const string& message, \
			const string& location = {}, const bool end_line = true);

private:
	static const constexpr char* kNullFile = "/dev/null";

	FileFactory default_file_factory_;
	FileFactory& file_factory_;
	ofstream log_file_;
};

#endif /* LOGGER_H_ */
