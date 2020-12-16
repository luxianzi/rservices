#include "serial_port.h"

SerialPort::SerialPort(void):
	application_protocol_(dummy_protocol_),
	serial_port_(io_s_) {
}

SerialPort::SerialPort(AbstractProtocol& protocol):
	application_protocol_(protocol),
	serial_port_(io_s_) {
}

SerialPort::~SerialPort() {
	serial_port_.close();
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
		if (to_lower(key_value->second) == "none")
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::none), ec);
		else if (to_lower(key_value->second) == "software")
			serial_port_.set_option(serial_port::flow_control(\
					serial_port::flow_control::software), ec);
		else if (to_lower(key_value->second) == "hardware")
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

error_condition SerialPort::Set(const string&key, unsigned int value) {
	if (key.empty())
		return kInvalidArgument;

	if (key == kAddress)
		application_protocol_.SetAddress(value);
	else if (key == kAddressSize)
		application_protocol_.SetAddressSize(value);
	else
		return kInvalidArgument;

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

	vector<uint8_t> data(data_size_);
	application_protocol_.PackFrame(value, data);
	if (serial_port_.write_some(buffer(data), ec) == 0)
		return make_error_condition(errc::io_error);

	return kNoError;
}

error_condition SerialPort::Get(const string& key, unsigned int& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (key == kAddress)
		result = application_protocol_.GetAddress();
	if (key == kAddressSize)
		result = application_protocol_.GetAddressSize();

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

//@brief SerialPort support half-duplex communication
error_condition SerialPort::Get(const string& key, vector<uint8_t>& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);

	if (key != kData)
		return kInvalidArgument;

	bool data_available = false;
	unsigned int data_size = 0;
	deadline_timer timeout(io_s_);
	vector<uint8_t> data(data_size_);

	io_s_.reset();
	serial_port_.async_read_some(buffer(data), \
			boost::bind(&ReadCallback, boost::ref(data_available), \
			boost::ref(data_size), boost::ref(timeout), \
			boost::asio::placeholders::error, \
			boost::asio::placeholders::bytes_transferred));

	timeout.expires_from_now(boost::posix_time::milliseconds(timeout_ms_));
	timeout.async_wait(boost::bind(&WaitCallback, \
			boost::ref(serial_port_), boost::asio::placeholders::error));

	io_s_.run();

	data.resize(data_size);
	if (application_protocol_.UnpackFrame(\
			data, result) != kNoError) {
		return make_error_condition(errc::io_error);
	}

	return kNoError;
}

