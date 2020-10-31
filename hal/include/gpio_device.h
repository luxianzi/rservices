#ifndef GPIO_DEVICE_H_
#define GPIO_DEVICE_H_

#include "abstract_device.h"
#include "nrt_fpga_device.h"

class GPIODevice : public AbstractDevice {
public:
	static const constexpr char* kDOPDataName = "do_pdata";
	static const constexpr char* kDOHDataName = "do_hdata";
	static const constexpr char* kDIPDataName = "di_pdata";
	static const constexpr char* kDIHDataName = "di_hdata";

	GPIODevice(NRTFPGADevice& nrt_fpga_device);
	~GPIODevice();

	virtual error_condition ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig() {
		return kOperationNotSupported;
	}

	virtual error_condition Set(const string& key,\
			const vector<uint8_t>& value);
	virtual error_condition Set(const string& key, const string& value) {
		return kOperationNotSupported;
	}
	virtual error_condition Set(const string& key, const unsigned int value);
	virtual error_condition Get(const string& key, vector<uint8_t>& result);
	virtual error_condition Get(const string& key, string& value) {
		return kOperationNotSupported;
	}
	virtual error_condition Get(const string& key, unsigned int& result);


private:
	NRTFPGADevice& nrt_fpga_device_;
};


#endif /* GPIO_DEVICE_H_ */
