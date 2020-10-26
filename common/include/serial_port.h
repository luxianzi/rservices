#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "abstract_device.h"
#include "abstract_protocol.h"
#include "error_category.h"
#include "dummy_protocol.h"

using namespace boost::asio;

class SerialPort: public AbstractDevice {
public:
	static const constexpr char* kPortName = "port_name";
	static const constexpr char* kData = "data";
	static const constexpr char* kBaudRate = "baud_rate";
	static const constexpr char* kCharacterSize = "character_size";
	static const constexpr char* kParity = "parity";
	static const constexpr char* kStopBits = "stop_bits";
	static const constexpr char* kFlowControl = "flow_control";

	SerialPort(void);
	virtual ~SerialPort();

	virtual error_condition ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig();
	virtual error_condition Set(const string& key, const unsigned int value) {
		return kOperationNotSupported;
	}
	virtual error_condition Set(const string& key, \
			const vector<uint8_t>& value);
	virtual error_condition Set(const string& key, const string& value);
	virtual error_condition Get(const string& key, unsigned int& result) {
		return kOperationNotSupported;
	}
	virtual error_condition Get(const string& key, vector<uint8_t>& result);
	virtual error_condition Get(const string& key, string& value);

protected:
	DummyProtocol dummy_protocol_;
	AbstractProtocol& application_protocol_;
	SerialPort(AbstractProtocol& protocol);

private:
	io_service io_s_;
	serial_port serial_port_;
	vector<uint8_t> serial_buffer_;
	CommonKeyPairs config_;
	vector<string> reserved_keys_ = {
		kPortName, kData, kBaudRate, kCharacterSize, kParity, kStopBits,
		kFlowControl
	};
};

#endif /* SERIAL_PORT_H_ */
