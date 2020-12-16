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

	virtual void SetAddressSize(const unsigned int address_size) {
		address_size_ = address_size;
	}
	virtual void SetAddress(const unsigned int address) {
		for (unsigned int i = 0; i < address_size_; i++)
			address_.push_back((address >> (8 * i)) & 0xFF);
	};
	virtual unsigned int GetAddressSize(void) {
		return address_size_;
	}
	virtual unsigned int GetAddress(void) {
		unsigned int address = 0;
		for (unsigned int i = 0; i < address_.size(); i++)
			address += static_cast<unsigned int>(address_[i] << (8 * i));
		return address;
	};

protected:
	virtual bool IsFrameComplete(const vector<uint8_t>& buffer) = 0;
	virtual bool ExtractValidData(const vector<uint8_t>& buffer,\
				vector<uint8_t>& data) = 0;
	virtual error_condition CalcCheckCode(\
			const vector<uint8_t>& buffer, vector<uint8_t> check_code) = 0;
	vector<uint8_t> address_;
	unsigned int address_size_;
};

#endif /* ABSTRACT_PROTOCOL_H_ */
