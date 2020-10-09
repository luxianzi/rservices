/*
 * serial_port.h
 *
 *  Created on: 2020Äê9ÔÂ18ÈÕ
 *      Author: duanning
 */

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "abstract_device.h"
#include "abstract_protocol.h"
#include "dummy_protocol.h"

using namespace boost::asio;

class SerialPort: public AbstractDevice {
public:

	static const constexpr char* kSerialPortName \
			= "serial_port_name";
	static const constexpr char* kSerialPortData \
			= "serial_port_data";
	static const constexpr char* kSerialPortBaudRate \
			= "serial_port_baud_rate";
	static const constexpr char* kSerialPortCharacterSize \
			= "serial_port_character_size";
	static const constexpr char* kSerialPortParity \
			= "serial_port_parity";
	static const constexpr char* kSerialPortStopBits \
			= "serial_port_stop_bits";
	static const constexpr char* kSerialPortFlowControl \
			= "serial_port_flow_control";

	SerialPort(void);
	virtual ~SerialPort();

	virtual error_condition ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig();
	error_condition Set(const string& key, const unsigned int value);
	error_condition Set(const string& key, \
				const vector<uint8_t>& value);
	error_condition Get(const string& key, unsigned int& result);
	error_condition Get(const string& key, \
				vector<uint8_t>& result);

protected:
	DummyProtocol dummy_protocol_;
	AbstractProtocol& application_protocol_;
	SerialPort(AbstractProtocol& protocol);

private:
	io_service io_s_;
	serial_port serial_port_;
	vector<uint8_t> serial_buffer_;
	CommonKeyPairs config_;
};


#endif /* SERIAL_PORT_H_ */
