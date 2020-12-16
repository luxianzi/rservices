#include "bmp180_device.h"
#include <thread>

Bmp180Device::Bmp180Device():
	serial_(protocol_),
	write_addr_(default_i2c_addr_),
	read_addr_(default_i2c_addr_ | 0x01) {
}

Bmp180Device::~Bmp180Device() {

}

error_condition Bmp180Device::ReConfig(const CommonKeyPairs& config) {
	ClearInitialized();
	auto key_value = config.find(kPortName);
	if (key_value == config.end())
		return kInvalidArgument;
	if (serial_.ReConfig(config) != kNoError)
		return kInvalidArgument;
	SetInitialized();

	if (GetSystemParam() != kNoError)
		return kInvalidArgument;

	if (GetCalibParam() != kNoError)
		return kInvalidArgument;

	return kNoError;
}

error_condition Bmp180Device::DeConfig() {
	return kNoError;
}

error_condition Bmp180Device::Set(const string& key, const unsigned int value) {
	return kOperationNotSupported;
}

error_condition Bmp180Device::Set(const string& key, \
		const vector<uint8_t>& value) {
	if (key != kI2CAddress)
		return kInvalidArgument;

	write_addr_ = value[0];
	read_addr_ = value[0] | 0x01 ;

	return kNoError;
}

error_condition Bmp180Device::Set(const string& key, const string& value) {
	return kOperationNotSupported;
}

error_condition Bmp180Device::Get(const string& key, unsigned int& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (key == kTemperature) {
		int temperature;
		if (GetTemperature(temperature) != kNoError) {
			return make_error_condition(errc::io_error);
		}
		result = static_cast<unsigned int>(temperature);
	} else if (key == kPressure) {
		int pressure;
		if (GetPressure(pressure) != kNoError) {
			return make_error_condition(errc::io_error);
		}
		result = static_cast<unsigned int>(pressure);
	} else {
		return kInvalidArgument;
	}
	return kNoError;
}

error_condition Bmp180Device::Get(const string& key, vector<uint8_t>& result) {
	return kOperationNotSupported;
}

error_condition Bmp180Device::Get(const string& key, string& value) {
	return kOperationNotSupported;
}

error_condition Bmp180Device::BusRead(const uint8_t reg, const uint8_t len, \
		vector<uint8_t>& read_data) {
	vector<uint8_t> buffer = {read_addr_, reg, len};
	if (serial_.Set(SerialPort::kData, buffer) != kNoError)
		return make_error_condition(errc::io_error);

	DelayMSec(kTempConversionTime);

	if (serial_.Get(SerialPort::kData, read_data) != kNoError)
		return make_error_condition(errc::io_error);

	return kNoError;
}

error_condition Bmp180Device::BusWrite(const uint8_t reg, \
		const vector<uint8_t>& write_data) {
	vector<uint8_t> buffer = {write_addr_, reg};
	buffer.insert(buffer.end(), write_data.begin(), write_data.end());
	if (serial_.Set(serial_.kData, buffer) != kNoError)
		return make_error_condition(errc::io_error);

	DelayMSec(kTempConversionTime);

	buffer.clear();
	buffer.resize(kBufferSize);
	if (serial_.Get(serial_.kData, buffer) != kNoError)
		return make_error_condition(errc::io_error);

	return kNoError;
}

error_condition Bmp180Device::GetSystemParam(void) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	vector<uint8_t> buffer(kBufferSize);
	if (BusRead(kChipIDReg, kGenReadWriteDataLength, buffer) != kNoError)
		return make_error_condition(errc::io_error);
	chip_id_ = buffer[0];

	oversamp_setting_ = kInitializeOverSampSetting;
	sw_oversamp_ = kInitializeSWOversamp;

	if (BusRead(kVersionReg, kGenReadWriteDataLength, buffer) != kNoError)
		return make_error_condition(errc::io_error);
	ml_version_ = buffer[0] & 0x0F;
	al_version_ = (buffer[0] & 0xF0) >> 4;

	return kNoError;
}

error_condition Bmp180Device::GetCalibParam(void) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	vector<uint8_t> params_buffer(kCalibDataSize);
	if (BusRead(kPromStartAddr, kPromDataLen, params_buffer) != kNoError)
		return make_error_condition(errc::io_error);

	calib_param_.ac1 = static_cast<int16_t>(params_buffer[kCalibParamAC1MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamAC1LSB]);
	calib_param_.ac2 = static_cast<int16_t>(params_buffer[kCalibParamAC2MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamAC2LSB]);
	calib_param_.ac3 = static_cast<int16_t>(params_buffer[kCalibParamAC3MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamAC3LSB]);
	calib_param_.ac4 = static_cast<uint16_t>(params_buffer[kCalibParamAC4MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamAC4LSB]);
	calib_param_.ac5 = static_cast<uint16_t>(params_buffer[kCalibParamAC5MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamAC5LSB]);
	calib_param_.ac6 = static_cast<uint16_t>(params_buffer[kCalibParamAC6MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamAC6LSB]);
	calib_param_.b1 = static_cast<int16_t>(params_buffer[kCalibParamB1MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamB1LSB]);
	calib_param_.b2 = static_cast<int16_t>(params_buffer[kCalibParamB2MSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamB2LSB]);
	calib_param_.mb = static_cast<int16_t>(params_buffer[kCalibParamMBMSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamMBLSB]);
	calib_param_.mc = static_cast<int16_t>(params_buffer[kCalibParamMCMSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamMCLSB]);
	calib_param_.md = static_cast<int16_t>(params_buffer[kCalibParamMDMSB] << \
			kShiftBitPositionBy08Bits | params_buffer[kCalibParamMDLSB]);
	return kNoError;
}

error_condition Bmp180Device::GetTemperature(int& temperature) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	int uncomp_temperature, x1, x2 = 0;

	if (GetUncompTemperature(uncomp_temperature) != kNoError) {
		return make_error_condition(errc::io_error);
	}
	// calculate temperature
	x1 = ((uncomp_temperature - calib_param_.ac6) * calib_param_.ac5) >> \
					kShiftBitPositionBy15Bits;
	if (x1 == kCheckDivisor && calib_param_.md == kCheckDivisor)
		return kInvalidArgument;

	// executed only the divisor is not zero
	x2 = (calib_param_.mc << kShiftBitPositionBy11Bits) /\
			(x1 + calib_param_.md);
	param_b5_ = x1 + x2;
	temperature = ((param_b5_ + kCalculateTrueTemperature) >> \
			kShiftBitPositionBy04Bits);

	return kNoError;
}

error_condition Bmp180Device::GetPressure(int& pressure) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	int temperature, uncomp_pressure, x1, x2, x3, b3, b6 = 0;
	unsigned int b4, b7 = 0;

	if (GetTemperature(temperature) != kNoError) {
		return make_error_condition(errc::io_error);
	}
	if (GetUncompPressure(uncomp_pressure) != kNoError)
		return make_error_condition(errc::io_error);

	b6 = param_b5_ - 4000;

	// calculate b3
	x1 = (b6 * b6) >> kShiftBitPositionBy12Bits;
	x1 *= calib_param_.b2;
	x1 >>= kShiftBitPositionBy11Bits;
	x2 = calib_param_.ac2 * b6;
	x2 >>= kShiftBitPositionBy11Bits;
	x3 = x1 + x2;
	b3 = ((((calib_param_.ac1 * 4) + x3) << \
			oversamp_setting_) + 2) >> kShiftBitPositionBy02Bits;

	// calculate b4
	x1 = (calib_param_.ac3 * b6) >> kShiftBitPositionBy13Bits;
	x2 = (calib_param_.b1 * ((b6 * b6) >> kShiftBitPositionBy12Bits))\
			>> kShiftBitPositionBy16Bits;
	x3 = ((x1 + x2) + 2) >> kShiftBitPositionBy02Bits;
	b4 = calib_param_.ac4 * (x3 + 32768) >> kShiftBitPositionBy15Bits;
	b7 = (uncomp_pressure - b3) * (50000 >> oversamp_setting_);
	if (b7 < 0x80000000)
	{
		if (b4 == kCheckDivisor)
			return kInvalidArgument;
		pressure = (b7 << kShiftBitPositionBy01Bits) / b4;

	}
	else
	{
		if (b4 == kCheckDivisor)
			return kInvalidArgument;
		pressure = (b7 / b4) << kShiftBitPositionBy01Bits;
	}
	x1 = pressure >> kShiftBitPositionBy08Bits;
	x1 *= x1;
	x1 = (x1 * kParamMG) >> kShiftBitPositionBy16Bits;
	x2 = (pressure * kParamMH) >> kShiftBitPositionBy16Bits;

	// pressure in Pa
	pressure += (x1 + x2 + kParamMI) >> kShiftBitPositionBy04Bits;

	return kNoError;
}

error_condition Bmp180Device::GetUncompTemperature(int& uncomp_temperature) {
	uint8_t ctrl_reg_data = 0;

	ctrl_reg_data = kTMeasure;
	vector<uint8_t> buffer = {ctrl_reg_data};
	if (BusWrite(kCtrlMeasReg, buffer) != kNoError)
		return make_error_condition(errc::io_error);

	DelayMSec(kTempConversionTime);

	buffer.clear();
	buffer.resize(kBufferSize);
	if (BusRead(kADCOutMSBReg, kTemperatureDataLength, buffer) != kNoError)
		return make_error_condition(errc::io_error);

	uncomp_temperature = (buffer[kTemperatureMSBData] << \
			kShiftBitPositionBy08Bits) | buffer[kTemperatureLSBData];

	return kNoError;
}

error_condition Bmp180Device::GetUncompPressure(int& uncomp_pressure) {
	uint8_t ctrl_reg_data;
	if(sw_oversamp_ == kSWOversamp && oversamp_setting_ == \
			kOversampSetting) {
		int uncomp_sum_pressure = 0;
		for (int i = 0; i < kDataMeasure; i++) {
			ctrl_reg_data = kPMeasure + \
					(oversamp_setting_ << kShiftBitPositionBy06Bits);
			vector<uint8_t> buffer = {ctrl_reg_data};
			if (BusWrite(kCtrlMeasReg, buffer) != kNoError)
				return make_error_condition(errc::io_error);

			DelayMSec(k2MSDelay + (k3MSDelay << oversamp_setting_));

			buffer.clear();
			buffer.resize(kBufferSize);
			if (BusRead(kADCOutMSBReg, kPressureDataLength, buffer) \
					!= kNoError)
				return make_error_condition(errc::io_error);

			uncomp_pressure = ((buffer[kPressureMSBData] << \
					kShiftBitPositionBy16Bits) | (buffer[kPressureLSBData] << \
					kShiftBitPositionBy08Bits) | buffer[kPressureXLSBData]) >> \
					(kCalculateTruePressure - oversamp_setting_);

			uncomp_sum_pressure += uncomp_pressure;
		}
		uncomp_pressure = uncomp_sum_pressure / kAverage;
	} else if (sw_oversamp_ == kInitializeSWOversamp) {
		ctrl_reg_data = kPMeasure + \
				(oversamp_setting_ << kShiftBitPositionBy06Bits);
		vector<uint8_t> buffer = {ctrl_reg_data};
		if (BusWrite(kCtrlMeasReg, buffer) != kNoError)
			return make_error_condition(errc::io_error);

		DelayMSec(k2MSDelay + (k3MSDelay << oversamp_setting_));

		buffer.clear();
		buffer.resize(kBufferSize);
		if (BusRead(kADCOutMSBReg, kPressureDataLength, buffer) \
				!= kNoError)
			return make_error_condition(errc::io_error);

		uncomp_pressure = ((buffer[kPressureMSBData] << \
				kShiftBitPositionBy16Bits) | (buffer[kPressureLSBData] << \
				kShiftBitPositionBy08Bits) | buffer[kPressureXLSBData]) >> \
				(kCalculateTruePressure - oversamp_setting_);
	}
	return kNoError;
}

void Bmp180Device::DelayMSec(const unsigned int msec) {
	this_thread::sleep_for(chrono::milliseconds(msec));
}
