#ifndef ABSTRACT_PROTOCOL_H_
#define ABSTRACT_PROTOCOL_H_

#include <vector>
#include <stdint.h>
#include "rservice.h"
#include "error_category.h"

class AbstractProtocol {
public:
	AbstractProtocol() {}
	virtual ~AbstractProtocol() {}

	virtual error_condition PackFrame(\
			const vector<uint8_t>& buffer, vector<uint8_t>& packed_data) = 0;
	virtual error_condition UnpackFrame(\
			const vector<uint8_t>& buffer, vector<uint8_t>& unpacked_data) = 0;
protected:
	virtual bool IsFrameComplete(const vector<uint8_t>& buffer) = 0;
	virtual bool IsFrameValid(const vector<uint8_t>& buffer) = 0;
	virtual error_condition CalcCheckCode(\
			const vector<uint8_t>& buffer, vector<uint8_t> check_code) = 0;
};

#endif /* ABSTRACT_PROTOCOL_H_ */
