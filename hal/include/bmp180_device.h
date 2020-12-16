#ifndef BMP180_DEVICE_H_
#define BMP180_DEVICE_H_

#include "abstract_device.h"
#include "serial_port.h"
#include "bmp180_protocol.h"
#include "rservice.h"

class Bmp180Device: public AbstractDevice {
public:
	static const constexpr char* kI2CAddress = "address";
	static const constexpr char* kPortName = "port_name";

	static const constexpr char* kTemperature = "temperature";
	static const constexpr char* kPressure = "pressure";

	Bmp180Device();
	~Bmp180Device();

	virtual error_condition ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig();

	virtual error_condition Set(const string& key, const unsigned int value);
	virtual error_condition Set(const string& key, const vector<uint8_t>& value);
	virtual error_condition Set(const string& key, const string& value);

	virtual error_condition Get(const string& key, unsigned int& result);
	virtual error_condition Get(const string& key, vector<uint8_t>& result);
	virtual error_condition Get(const string& key, string& value);

protected:
	error_condition GetSystemParam();
	error_condition GetCalibParam();

	error_condition BusRead(const uint8_t reg, const uint8_t len, \
			vector<uint8_t>& read_data);
	error_condition BusWrite(const uint8_t reg, \
			const vector<uint8_t>& write_data);
	void DelayMSec(const unsigned int msec);

	error_condition GetUncompTemperature(int& uncomp_temperature);
	error_condition GetUncompPressure(int& uncomp_pressure);
	error_condition GetTemperature(int& temperature);
	error_condition GetPressure(int& pressure);

private:
	uint8_t write_addr_;
	uint8_t read_addr_;

	SerialPort serial_;
	Bmp180Protocol protocol_;

	static const uint8_t default_i2c_addr_ = 0xEE;

	static const uint8_t kBufferSize = 32;

	// CONSTANTS
	static const uint8_t kInitializeOverSampSetting = 0;
	static const uint8_t kInitializeSWOversamp = 0;
	static const uint8_t kGenReadWriteDataLength = 1;
	static const uint8_t kTemperatureDataLength = 2;
	static const uint8_t kPressureDataLength = 3;
	static const uint8_t kSWOversamp = 1;
	static const uint8_t kOversampSetting = 3;
	static const uint8_t k2MSDelay = 2;
	static const uint8_t k3MSDelay = 3;
	static const uint8_t kAverage = 3;
	static const uint8_t kCheckDivisor = 0;
	static const uint8_t kDataMeasure = 3;
	static const uint8_t kCalculateTruePressure = 8;
	static const uint8_t kCalculateTrueTemperature = 8;

	enum {
		kShiftBitPositionBy01Bits = 1,
		kShiftBitPositionBy02Bits = 2,
		kShiftBitPositionBy04Bits = 4,
		kShiftBitPositionBy06Bits = 6,
		kShiftBitPositionBy08Bits = 8,
		kShiftBitPositionBy11Bits = 11,
		kShiftBitPositionBy12Bits = 12,
		kShiftBitPositionBy13Bits = 13,
		kShiftBitPositionBy15Bits = 15,
		kShiftBitPositionBy16Bits = 16,
	};

	// register definitions
	static const int8_t kPromStartAddr = 0xAA;
	static const int8_t kPromDataLen = 22;

	static const int8_t kChipIDReg = 0xD0;
	static const int8_t kVersionReg = 0xD1;

	static const int8_t kCtrlMeasReg = 0xF4;
	static const int8_t kADCOutMSBReg = 0xF6;
	static const int8_t kADCOutLSBReg = 0xF7;

	static const int8_t kSoftResetReg = 0xE0;

	// temperature measurement
	static const int8_t kTMeasure = 0x2E;
	// pressure measurement
	static const int8_t  kPMeasure = 0x34;

	// TO be spec'd by GL or SB
	static const uint8_t  kTempConversionTime = 5;

	static const int16_t kParamMG = 3038;
	static const int16_t kParamMH = -7357;
	static const int16_t kParamMI = 3791;

	// ARRAY SIZE DEFINITIONS
	static const uint8_t kTemperatureDataBytes = 2;
	enum {
		kTemperatureMSBData,
		kTemperatureLSBData,
	};

	static const uint8_t kPressureDataBytes = 3;
	enum {
		kPressureMSBData,
		kPressureLSBData,
		kPressureXLSBData,
	};

	static const uint8_t kCalibDataSize = 22;
	enum {
		kCalibParamAC1MSB,
		kCalibParamAC1LSB,
		kCalibParamAC2MSB,
		kCalibParamAC2LSB,
		kCalibParamAC3MSB,
		kCalibParamAC3LSB,
		kCalibParamAC4MSB,
		kCalibParamAC4LSB,
		kCalibParamAC5MSB,
		kCalibParamAC5LSB,
		kCalibParamAC6MSB,
		kCalibParamAC6LSB,
		kCalibParamB1MSB,
		kCalibParamB1LSB,
		kCalibParamB2MSB,
		kCalibParamB2LSB,
		kCalibParamMBMSB,
		kCalibParamMBLSB,
		kCalibParamMCMSB,
		kCalibParamMCLSB,
		kCalibParamMDMSB,
		kCalibParamMDLSB,
	};

	// brief This structure holds all device specific calibration parameters
	struct calib_param_t
	{
		int16_t ac1; 	// <calibration ac1 data
		int16_t ac2; 	// <calibration ac2 data
		int16_t ac3; 	// <calibration ac3 data
		uint16_t ac4; 	// <calibration ac4 data
		uint16_t ac5; 	// <calibration ac5 data
		uint16_t ac6; 	// <calibration ac6 data
		int16_t b1; 	// <calibration b1 data
		int16_t b2; 	// <calibration b2 data
		int16_t mb; 	// <calibration mb data
		int16_t mc; 	// <calibration mc data
		int16_t md; 	// <calibration md data/
	};

	struct calib_param_t calib_param_; //<calibration data
	uint8_t mode_; 				// <power mode
	uint8_t chip_id_; 			// <chip id
	uint8_t ml_version_; 		// <ml version
	uint8_t al_version_; 		// <al version*/
	uint8_t dev_addr_; 			// <device address
	uint8_t sensortype_; 		// < sensor type
	int32_t param_b5_; 			// <pram
	int16_t oversamp_setting_; 	// <oversampling setting
	int16_t sw_oversamp_; 		// <software oversampling
};

#endif /* BMP180_DEVICE_H_ */
