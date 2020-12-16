#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
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

	static const constexpr char* kAddress = "address";
	static const constexpr char* kAddressSize = "address_size";

	SerialPort(void);
	SerialPort(AbstractProtocol& protocol);
	virtual ~SerialPort();

	virtual error_condition ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig();

	virtual error_condition Set(const string& key, const unsigned int value);
	virtual error_condition Set(const string& key, \
			const vector<uint8_t>& value);
	virtual error_condition Set(const string& key, const string& value);
	virtual error_condition Get(const string& key, unsigned int& result);
	virtual error_condition Get(const string& key, vector<uint8_t>& result);
	virtual error_condition Get(const string& key, string& value);

private:
	static void ReadCallback(bool& data_available, unsigned int& data_size, \
			deadline_timer& timeout, const boost::system::error_code& error, \
			std::size_t bytes_transferred) {
		if (error || ! bytes_transferred) {
			data_available = false;
			return;
		}
		timeout.cancel();
		data_available = true;
		data_size = bytes_transferred;
	}
	static void WaitCallback(serial_port& serial, \
			const boost::system::error_code& error) {
		if (error)
			return;
		serial.cancel();
	};
	io_service io_s_;
	serial_port serial_port_;
	CommonKeyPairs config_;
	vector<string> reserved_keys_ = {
		kPortName, kData, kBaudRate, kCharacterSize, kParity, kStopBits,
		kFlowControl
	};
	DummyProtocol dummy_protocol_;
	AbstractProtocol& application_protocol_;

	static const uint8_t data_size_ = 128;
	static const uint8_t timeout_ms_ = 8;
};


#endif /* SERIAL_PORT_H_ */
