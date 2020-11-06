#include "gpio_device.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


GPIODevice::GPIODevice(NRTFPGADevice & nrt_fpga_device) :
	nrt_fpga_device_(nrt_fpga_device) {
	CommonKeyPairs config = {
	};
	ReConfig(config);
}


GPIODevice::~GPIODevice() {
	ClearInitialized();
}

error_condition GPIODevice::ReConfig(const CommonKeyPairs& config) {
	ClearInitialized();
	vector<uint8_t> out_data = {0, 0, 0, 0};
	nrt_fpga_device_.Set(NRTFPGADevice::kDOData, out_data);
	SetInitialized();

	return kNoError;
}

error_condition GPIODevice::Get(const string& key, vector<uint8_t>& result) {
	if (key.empty())
		return kInvalidArgument;
	vector<uint8_t> get_data;
	if (key == kDIPDataName)
	{
		if (nrt_fpga_device_.Get(NRTFPGADevice::kDIData, get_data) != kNoError)
			return kInvalidArgument;

		result.resize(2);
		copy(get_data.begin(), get_data.end(), result.begin());
	}
	else if (key == kDIHDataName)
	{
		if (nrt_fpga_device_.Get(NRTFPGADevice::kDIData, get_data) != kNoError)
			return kInvalidArgument;

		result.resize(2);
		copy(get_data.begin() + 2, get_data.end(), result.begin());
	}
	else if (key == kDOPDataName)
	{
		if (nrt_fpga_device_.Get(NRTFPGADevice::kDOData, get_data) != kNoError)
			return kInvalidArgument;

		result.resize(2);
		copy(get_data.begin(), get_data.end(), result.begin());
	}
	else if (key == kDOHDataName)
	{
		if (nrt_fpga_device_.Get(NRTFPGADevice::kDOData, get_data) != kNoError)
			return kInvalidArgument;

		result.resize(2);
		copy(get_data.begin() + 2, get_data.end(), result.begin());
	}
	else
		return kInvalidArgument;

	return kNoError;
}

error_condition GPIODevice::Get(const string& key, unsigned int& result) {
	if (key.empty())
		return kInvalidArgument;
	int pos = key.find(".", 0);
	if (pos == -1)
		return kInvalidArgument;
	string data_name = key.substr(0, pos);
	int position = 0;
	try {
		position = stoi(key.substr(pos + 1));
	}
	catch (...){
		return kInvalidArgument;
	}
	if ((data_name != kDIPDataName && data_name != kDIHDataName \
			&& data_name != kDOPDataName && data_name != kDOHDataName)\
			|| position < 0 || position > 15)
		return kInvalidArgument;

	vector<uint8_t> get_data;
	if (Get(data_name, get_data) != kNoError)
		return kInvalidArgument;

	int mask = 1 << position;
	result = (get_data[0] | ((get_data[1] << 8) & 0xff00)) & mask ? 1 : 0;

	return kNoError;
}

error_condition GPIODevice::Set(const string& key, \
		const vector<uint8_t>& value) {
	if (value.size() < sizeof(uint16_t))
		return kInvalidArgument;
	vector<uint8_t> get_data;
	vector<uint8_t> set_data(4);
	if (key == kDOPDataName)
	{
		if (Get(kDOHDataName, get_data) != kNoError)
			return kInvalidArgument;

		set_data[0] = value[0];
		set_data[1] = value[1];
		set_data[2] = get_data[0];
		set_data[3] = get_data[1];
	}
	else if (key == kDOHDataName)
	{
		if (Get(kDOPDataName, get_data) != kNoError)
			return kInvalidArgument;

		set_data[0] = get_data[0];
		set_data[1] = get_data[1];
		set_data[2] = value[0];
		set_data[3] = value[1];
	}
	else
		return kInvalidArgument;

	if (nrt_fpga_device_.Set(NRTFPGADevice::kDOData, set_data) != kNoError)
		return kInvalidArgument;

	return kNoError;
}

error_condition GPIODevice::Set(const string& key, const unsigned int value) {
	if (key.empty())
		return kInvalidArgument;
	int pos = key.find(".", 0);
	if (pos == -1)
		return kInvalidArgument;
	string data_name = key.substr(0, pos);
	int position = 0;
	try {
		position = stoi(key.substr(pos + 1));
	}
	catch (...){
		return kInvalidArgument;
	}
	if ((data_name != kDOPDataName && data_name != kDOHDataName) \
			|| position < 0 || position > 15)
		return kInvalidArgument;

	vector<uint8_t> set_data(2);
	vector<uint8_t> get_data;

	if (Get(data_name, get_data) != kNoError)
		return kInvalidArgument;

	copy(get_data.begin(), get_data.end(), set_data.begin());

	int pos_subscript = position / 8;
	int pos_shift = position % 8;
	set_data[pos_subscript] = (value == 1) ?\
			set_data[pos_subscript] | ((1 << pos_shift) & 0xff) : \
			set_data[pos_subscript] & (~(1 << pos_shift) & 0xff);

	if (Set(data_name, set_data) != kNoError)
		return kInvalidArgument;

	return kNoError;
}

