#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <algorithm>
#include "can_socket.h"

CANSocket::CANSocket() :
	can_socket_(-1),
	connected_(false),
	read_canid_(0),
	write_canid_(0) {

}

CANSocket::CANSocket(SocketConfig& config) {
	ReConfig(config);
}

CANSocket::~CANSocket() {
	if (connected_)
		::close(can_socket_);
}

error_condition	CANSocket::ReConfig(SocketConfig& config) {
	// TODO: Replace the configuration with libsocketcan API instead of command
	auto it_interface = config.find(kInterface);
	if (it_interface == config.end())
		return make_error_condition(ErrorCode::kInterfaceNotSpecified);
	auto it_bit_rate = config.find(kBitRate);
	if (it_bit_rate == config.end())
		return make_error_condition(ErrorCode::kBitRateNotSpecified);
	auto it_control_mode = config.find(kControlMode);
	if (it_control_mode == config.end())
		return make_error_condition(ErrorCode::kControlModeNotSpecified);
	string command;
	command = "canconfig " + it_interface->second + " " + \
			"bitrate " + it_bit_rate->second + " " +\
			"ctrlmode " + it_control_mode->second;
	if (connected_)
		::close(can_socket_);
	connected_ = false;
	int result = system(command.c_str());
	if (result != 0)
		return make_error_condition(errc(result));
	int can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (can_socket < 0)
		return make_error_condition(errc(can_socket));
	struct ifreq interface_request;
	copy(it_interface->second.begin(), it_interface->second.end(), \
			interface_request.ifr_name);
	ioctl(can_socket, SIOCGIFINDEX, &interface_request);
	struct sockaddr_can can_socket_address;
	can_socket_address.can_family  = AF_CAN;
	can_socket_address.can_ifindex = interface_request.ifr_ifindex;
	result = bind(can_socket, \
			reinterpret_cast<struct sockaddr *>(&can_socket_address), \
			sizeof(can_socket_address));
	if (result < 0)
		return make_error_condition(errc(result));
	connected_ = true;
	return kNoError;
}

error_condition CANSocket::Read(vector<uint8_t>& result) {
	result.clear();
	if (!connected_)
		return make_error_condition(errc::not_connected);
	struct can_frame frame;
	int ret = read(can_socket_, &frame, sizeof(frame));
	if (ret <= 0)
		return make_error_condition(errc(ret));
	if (ret != sizeof(frame) || frame.can_dlc > CAN_MAX_DLEN)
		return make_error_condition(errc::io_error);
	read_canid_ = frame.can_id;
	if (frame.can_dlc == 0)
		return kNoError;
	result.resize(frame.can_dlc);
	copy(frame.data, frame.data + frame.can_dlc, result.begin());
	return kNoError;
}

error_condition CANSocket::Write(const vector<uint8_t>& value) {
	if (!connected_)
		return make_error_condition(errc::not_connected);
	struct can_frame frame;
	fill_n(reinterpret_cast<uint8_t*>(&frame), sizeof(frame), 0);
	frame.can_id = write_canid_;
	frame.can_dlc = value.size() < CAN_MAX_DLEN ? value.size() : CAN_MAX_DLEN;
	copy(value.begin(), value.end(), frame.data);
	int ret = write(can_socket_, &frame, sizeof(frame));
	if (ret <= 0)
		return make_error_condition(errc(ret));
	if (ret != sizeof(frame))
		return make_error_condition(errc::io_error);
	return kNoError;
}
bool CANSocket::IsConnected() {
	return connected_;
}

error_condition CANSocket::GetReadCANID(canid_t& id) {
	if (!connected_)
		return make_error_condition(errc::not_connected);
	id = read_canid_;
	return kNoError;
}

error_condition CANSocket::SetWriteCANID(const canid_t id) {
	if (!connected_)
		return make_error_condition(errc::not_connected);
	write_canid_ = id;
	return kNoError;
}
