#ifndef BMP180_PROTOCOL_H_
#define BMP180_PROTOCOL_H_

#include "abstract_protocol.h"

class Bmp180Protocol: public DummyProtocol {
public:
	Bmp180Protocol() {}
	~Bmp180Protocol() {}

	virtual error_condition UnpackFrame(
			const vector<uint8_t>& buffer, vector<uint8_t>& unpacked_data) {
		if (!ExtractValidData(buffer, unpacked_data)) {
			return make_error_condition(errc::bad_message);
		}
		return kNoError;
	};
protected:
	virtual error_condition CalcCheckCode(\
			const vector<uint8_t>& buffer, vector<uint8_t> check_code) {
		if ((buffer.back() != check_code.back()) || (buffer.at(buffer.size() - 2) \
				!= check_code.at(check_code.size() - 2))) {
			return make_error_condition(errc::bad_message);
		}
		return kNoError;
	};
	virtual bool ExtractValidData(const vector<uint8_t>& buffer,\
			vector<uint8_t>& unpacked_data) {
		if (!IsFrameComplete(buffer)) {
			return false;
		}
		if (CalcCheckCode(buffer, terminator_) != kNoError) {
			return false;
		}
		unpacked_data.assign(buffer.begin(), buffer.begin() + buffer.size() -\
				terminator_.size());

		return true;
	};
private:
	vector<uint8_t> terminator_ = {0x01, 0x0A};
};

#endif /* BMP180_PROTOCOL_H_ */
