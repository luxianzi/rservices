/*
 * serial_port.cpp
 *
 *  Created on: 2020Äê9ÔÂ18ÈÕ
 *      Author: duanning
 */

#include "serial_port.h"

SerialPort::SerialPort(void):
	application_protocol_(dummy_protocol_),
	serial_port_(io_s_) {
}

SerialPort::~SerialPort() {
	serial_port_.close();
}

SerialPort::SerialPort(AbstractProtocol& protocol):
	application_protocol_(protocol),
	serial_port_(io_s_) {
}

error_condition SerialPort::ReConfig(const CommonKeyPairs &config) {
	ClearInitialized();
	boost::system::error_code ec;
	auto key_value = config.find(kSerialPortName);
	if (key_value == config.end())
		return make_error_condition(errc::invalid_argument);
	serial_port_.open(string(key_value->second.c_str()), ec);
	if (ec)
		return make_error_condition(errc::bad_address);

	key_value = config.find(kSerialPortBaudRate);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::baud_rate(115200), ec);
	else
		serial_port_.set_option(serial_port::baud_rate(\
				atoi(key_value->second.c_str())), ec);
	if (ec)
		return make_error_condition(errc::invalid_argument);

	key_value = config.find(kSerialPortCharacterSize);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::character_size(8), ec);
	else
		serial_port_.set_option(serial_port::character_size(\
				atoi(key_value->second.c_str())), ec);
	if (ec)
		return make_error_condition(errc::invalid_argument);

	key_value = config.find(kSerialPortParity);
	if (key_value == config.end())
		serial_port_.set_option(\
				serial_port::parity(serial_port::parity::none), ec);
	else {
		if (!strcmp(key_value->second.c_str(), "N") || \
				!strcmp(key_value->second.c_str(), "n"))
			serial_port_.set_option(\
					serial_port::parity(serial_port::parity::none), ec);
		else if (!strcmp(key_value->second.c_str(), "E") || \
				!strcmp(key_value->second.c_str(), "e"))
			serial_port_.set_option(\
					serial_port::parity(serial_port::parity::even), ec);
		else
			serial_port_.set_option(\
					serial_port::parity(serial_port::parity::odd), ec);
	}
	if (ec)
		return make_error_condition(errc::invalid_argument);

	key_value = config.find(kSerialPortStopBits);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::stop_bits(\
				serial_port::stop_bits::one), ec);
	else {
		if (!strcmp(key_value->second.c_str(), "1"))
			serial_port_.set_option(serial_port::stop_bits(\
					serial_port::stop_bits::one), ec);
		else if (!strcmp(key_value->second.c_str(), "1.5"))
			serial_port_.set_option(serial_port::stop_bits(\
					serial_port::stop_bits::onepointfive), ec);
		else
			serial_port_.set_option(serial_port::stop_bits(\
					serial_port::stop_bits::two), ec);
	}
	if (ec)
		return make_error_condition(errc::invalid_argument);

	key_value = config.find(kSerialPortFlowControl);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::flow_control(\
				serial_port::flow_control::none), ec);
	else {
		if (!strcmp(key_value->second.c_str(), "none"))
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::none), ec);
		else if (!strcmp(key_value->second.c_str(), "software"))
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::software), ec);
		else
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::hardware), ec);
	}
	if (ec)
		return make_error_condition(errc::invalid_argument);

	SetInitialized();
	return kNoError;
}

error_condition SerialPort::DeConfig() {
	serial_port_.close();
	return kNoError;
}

error_condition SerialPort::Set(const string& key, const unsigned int value) {
	static const error_condition kInvalidArgument(errc::invalid_argument);
	if (key.empty())
		return kInvalidArgument;
	else
		config_[key] = to_string(value);

	return kNoError;
}

error_condition SerialPort::Set(\
		const string& key, const vector<uint8_t>& value) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	boost::system::error_code ec;

	if (key == kSerialPortData) {
		vector<uint8_t> out_data(value);
		application_protocol_.PackFrame(value, out_data);
		if (serial_port_.write_some(buffer(out_data), ec) == 0)
			return make_error_condition(errc::io_error);
	} else {
		string key_value;
		key_value.assign(value.begin(), value.end());
		config_[key] = key_value;
	}
	return kNoError;
}

error_condition SerialPort::Get(const string& key, unsigned int& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (config_.find(key) == config_.end())
		return make_error_condition(errc::invalid_argument);

	string key_value = config_[key];
	result = std::stoi(key_value);
	return kNoError;
}

/*
 * @brief SerialPort support half-duplex communication
 */
error_condition SerialPort::Get(const string& key, vector<uint8_t>& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (key == kSerialPortData) {
		boost::system::error_code ec;
		if (serial_port_.read_some(buffer(serial_buffer_), ec) <= 0)
			return make_error_condition(errc::no_message);
		if (application_protocol_.UnpackFrame(\
				serial_buffer_, result) != kNoError) {
			return make_error_condition(errc::io_error);
		}
	}else {
		if (config_.find(key) == config_.end())
			return make_error_condition(errc::invalid_argument);
		string key_value = config_[key];
		result.resize(key_value.size());
		result.assign(key_value.begin(), key_value.end());
	}
	return kNoError;
}

