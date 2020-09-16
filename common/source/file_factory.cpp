#include "file_factory.h"

FileFactory::FileFactory() {

}

FileFactory::~FileFactory() {

}

ofstream FileFactory::MakeOutputFile(const string& file_name, bool append) {
	ios_base::openmode mode = append ? ios_base::app : ios_base::out;
	return ofstream(file_name, mode);
}
