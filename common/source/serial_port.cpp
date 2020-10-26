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
	auto key_value = config.find(kPortName);
	if (key_value == config.end())
		return kInvalidArgument;
	serial_port_.open(string(key_value->second.c_str()), ec);
	if (ec)
		return make_error_condition(errc::no_such_device);

	key_value = config.find(kBaudRate);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::baud_rate(115200), ec);
	else {
		int baud_rate;
		try {
			baud_rate = stoi(key_value->second);
		}
		catch (...) {
			return kInvalidArgument;
		};
		serial_port_.set_option(serial_port::baud_rate(baud_rate), ec);
	}
	if (ec)
		return kInvalidArgument;

	key_value = config.find(kCharacterSize);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::character_size(8), ec);
	else {
		int character_size;
		try {
			character_size = stoi(key_value->second);
		}
		catch (...) {
			return kInvalidArgument;
		}
		serial_port_.set_option(serial_port::character_size(character_size), \
				ec);
	}
	if (ec)
		return kInvalidArgument;

	key_value = config.find(kParity);
	if (key_value == config.end())
		serial_port_.set_option(\
				serial_port::parity(serial_port::parity::none), ec);
	else {
		if (to_lower(key_value->second) == "n")
			serial_port_.set_option(\
					serial_port::parity(serial_port::parity::none), ec);
		else if (to_lower(key_value->second) == "e")
			serial_port_.set_option(\
					serial_port::parity(serial_port::parity::even), ec);
		else if (to_lower(key_value->second) == "o")
			serial_port_.set_option(\
					serial_port::parity(serial_port::parity::odd), ec);
		else
			return kInvalidArgument;
	}
	if (ec)
		return kInvalidArgument;

	key_value = config.find(kStopBits);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::stop_bits(\
				serial_port::stop_bits::one), ec);
	else {
		if (key_value->second == "1")
			serial_port_.set_option(serial_port::stop_bits(\
					serial_port::stop_bits::one), ec);
		else if (key_value->second == "1.5")
			serial_port_.set_option(serial_port::stop_bits(\
					serial_port::stop_bits::onepointfive), ec);
		else if (key_value->second == "2")
			serial_port_.set_option(serial_port::stop_bits(\
					serial_port::stop_bits::two), ec);
		else
			return kInvalidArgument;
	}
	if (ec)
		return kInvalidArgument;

	key_value = config.find(kFlowControl);
	if (key_value == config.end())
		serial_port_.set_option(serial_port::flow_control(\
				serial_port::flow_control::none), ec);
	else {
		if (key_value->second == "none")
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::none), ec);
		else if (key_value->second == "software")
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::software), ec);
		else if (key_value->second == "hardware")
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::hardware), ec);
		else
			return kInvalidArgument;
	}
	if (ec)
		return kInvalidArgument;

	SetInitialized();
	return kNoError;
}

error_condition SerialPort::DeConfig() {
	serial_port_.close();
	return kNoError;
}

error_condition SerialPort::Set(const string& key, const string& value) {
	if (key.empty())
		return kInvalidArgument;

	if (key == kData)
		return kInvalidArgument;

	if (find(reserved_keys_.begin(), reserved_keys_.end(), key) == \
			reserved_keys_.end())
		return kInvalidArgument;

	config_[key] = value;
	return kNoError;
}

error_condition SerialPort::Set(const string& key, \
		const vector<uint8_t>& value) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	boost::system::error_code ec;

	if (key != kData)
		return kInvalidArgument;

	vector<uint8_t> data;
	application_protocol_.PackFrame(value, data);
	if (serial_port_.write_some(buffer(data), ec) == 0)
		return make_error_condition(errc::io_error);

	return kNoError;
}

error_condition SerialPort::Get(const string& key, string& value) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (config_.find(key) == config_.end())
		return kInvalidArgument;

	value = config_[key];
	return kNoError;
}

/*
 * @brief SerialPort support half-duplex communication
 */
error_condition SerialPort::Get(const string& key, vector<uint8_t>& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (key != kData)
		return kInvalidArgument;

	boost::system::error_code ec;
	vector<uint8_t> data;
	if (serial_port_.read_some(buffer(data), ec) <= 0)
		return make_error_condition(errc::no_message);
	serial_buffer_.insert(serial_buffer_.end(), data.begin(), data.end());
	if (application_protocol_.UnpackFrame(\
			serial_buffer_, result) != kNoError) {
		return make_error_condition(errc::io_error);
	}

	return kNoError;
}

