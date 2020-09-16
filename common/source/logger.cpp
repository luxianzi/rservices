#include <iostream>
#include "logger.h"
#include "error_category.h"

Logger::Logger() :
	default_file_factory_(),
	file_factory_(default_file_factory_) {
	Init(kNullFile);
}

Logger::Logger(const string& file_name) :
	default_file_factory_(),
	file_factory_(default_file_factory_) {
	Init(file_name);
}

Logger::Logger(const string& file_name, const FileFactory& file_factory) :
	default_file_factory_(),
	file_factory_(const_cast<FileFactory&>(file_factory)) {
	Init(file_name);
}

error_condition Logger::Init(const string& file_name) {
	if (log_file_)
		log_file_.close();
	log_file_ = file_factory_.MakeOutputFile(file_name, true);
	if (!log_file_)
		log_file_ = file_factory_.MakeOutputFile(kNullFile, true);
	return kNoError;
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::Log(const Level level, const string& message, \
		const string& location, const bool end_line) {
	string log_line = location;
	switch (level) {
	case kDebug:
		log_line += "DEBUG: ";
		break;
	case kWarning:
		log_line += "WARNING: ";
		break;
	case kError:
		log_line += "ERROR: ";
		break;
	default:
		log_line += "INFO: ";
		break;
	}
	log_line += message;
	log_line += end_line ? "\n" : "";
	if (log_file_)
		log_file_ << log_line;
	cout << log_line;
}
