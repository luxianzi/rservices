#ifndef FILE_FACTORY_H_
#define FILE_FACTORY_H_

#include <fstream>
#include <string>

using namespace std;

class FileFactory {
public:
	FileFactory();
	virtual ~FileFactory();

	virtual ofstream MakeOutputFile(const string& file_name, \
			bool append = false);
};

#endif /* FILE_FACTORY_H_ */
