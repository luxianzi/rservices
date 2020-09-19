#include <thread>
#include <chrono>
#include "robo_master_c620_device.h"

RoboMasterC620Device::RoboMasterC620Device() :
	can_socket_(),
	can_receive_thread_([&](){ this->ReceiveWork(); }),
	can_transmit_thread_([&](){ this->TransmitWork(); }),
	AbstractSocketDevice(can_socket_),
	motor_count_(kDefaultMotorCount),
	motors_(kMaxMotorCount),
	loop_mode_(kNone),
	motor_index_(0) {
	MotorData zero_motor_data = {};
	fill(motors_.begin(), motors_.end(), zero_motor_data);
	CommonKeyPairs config = {
		{"interface", "can0"},
		{"bit_rate", "1000000"},
		{"control_mode", "triple-sampling on"}
	};
	if (ReConfig(config) != kNoError)
		return;
	SetInitialized();
	can_receive_thread_.Start();
	can_transmit_thread_.Start();
}

RoboMasterC620Device::~RoboMasterC620Device() {
	DeConfig();
}

error_condition RoboMasterC620Device::Set(const string& key, const unsigned int value) {
	static const error_condition kInvalidArgument(errc::invalid_argument);
	if (key.empty())
		return kInvalidArgument;
	lock_guard<mutex> lock(can_receive_mutex_);
	if (key == kMotorCount) {
		if (value >= kMaxMotorCount)
			return kInvalidArgument;
		motor_count_ = value;
	}
	else if (key == kMotorIndex) {
		if (value >= motor_count_)
			return kInvalidArgument;
		motor_index_ = value;
	}
	else if (key == kMotorCurrent) {
		motors_[motor_index_].target_current = static_cast<int>(value);
	}
	else if (key == kMotorSpeed) {
		motors_[motor_index_].target_speed = static_cast<int>(value);
	}
	else
		return kInvalidArgument;
	return kNoError;
}

error_condition RoboMasterC620Device::Get(const string& key, unsigned int& result) {

}

void RoboMasterC620Device::ReceiveWork() {
	if (!IsInitialized()) {
		this_thread::sleep_for(chrono::milliseconds(10));
		return;
	}
	vector<uint8_t> buffer;
	if (can_socket_.Read(buffer) != kNoError)
		return;
	if (buffer.empty())
		return;
	canid_t can_id;
	if (can_socket_.GetReadCANID(can_id) != kNoError)
		return;
	unsigned int motor_id;
	if (CANIDToMotorID(can_id, motor_id) != kNoError)
		return;
	ReceiveData data(buffer);
	if (!data.valid)
		return;
	lock_guard<mutex> lock(can_receive_mutex_);
	motors_[motor_id].angle = data.angle;
	motors_[motor_id].feedback_speed = data.speed;
	motors_[motor_id].feedback_current = data.current;
	motors_[motor_id].temperature = data.temperature;
}

void RoboMasterC620Device::TransmitWork() {
	if (!IsInitialized()) {
		this_thread::sleep_for(chrono::milliseconds(10));
		return;
	}
	LoopMode loop_mode = static_cast<LoopMode>(loop_mode_.load());
	vector<int> currents(kMaxMotorCount);
	fill(currents.begin(), currents.end(), 0);
	{
		lock_guard<mutex> lock(can_receive_mutex_);
		for (int i = 0; i < motor_count_; i++) {
			switch (loop_mode) {
			case kNone:
					currents[i] = motors_[i].target_current;
				break;
			case kSpeed:
					currents[i] = 100 * (motors_[i].target_speed - motors_[i].feedback_speed);
				break;
			default:
				break;
			}
		}
	}
	int frame_id = 0;
	do {
		TransmitData data(currents);
		canid_t can_id;
		FrameIDToCANID(frame_id++, can_id);
		if (can_socket_.SetWriteCANID(can_id) != kNoError || \
				can_socket_.Write(data.buffer) != kNoError)
			return;
		currents.erase(currents.begin(), currents.begin() + \
				kDefaultMotorCount);
	} while (currents.size() != 0);
	this_thread::sleep_for(chrono::milliseconds(1));
}
