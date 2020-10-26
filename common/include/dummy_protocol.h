#ifndef DUMMY_PROTOCOL_H_
#define DUMMY_PROTOCOL_H_

#include <iostream>
#include "abstract_protocol.h"

class DummyProtocol: public AbstractProtocol {
public:
	DummyProtocol() {};
	virtual ~DummyProtocol() {};

	virtual error_condition PackFrame(\
			const vector<uint8_t>& buffer, vector<uint8_t>& data) {
		data = buffer;
		return kNoError;
	};
	virtual error_condition UnpackFrame(\
			const vector<uint8_t>& buffer, vector<uint8_t>& data) {
		data = buffer;
		return kNoError;
	};

protected:
	virtual bool IsFrameComplete(const vector<uint8_t>& buffer) {return true;};
	virtual bool ExtractValidData(const vector<uint8_t>& buffer,\
					vector<uint8_t>& unpacked_data) {return true;};
	virtual error_condition CalcCheckCode(\
			const vector<uint8_t>& buffer, vector<uint8_t> check_code) {
		return kNoError;
	};
};

#endif /* DUMMY_PROTOCOL_H_ */
