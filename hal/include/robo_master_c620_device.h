#ifndef ROBO_MASTER_C620_DEVICE_H_
#define ROBO_MASTER_C620_DEVICE_H_

#include <algorithm>
#include <vector>
#include <string>
#include <mutex>
#include "rservice.h"
#include "abstract_socket_device.h"
#include "can_socket.h"
#include "abstract_thread.h"
#include "nrt_thread.h"
#include "pid_controller.h"

class RoboMasterC620Device: public AbstractSocketDevice {
public:
	static const constexpr char* kMotorCount = "motor_count";
	static const constexpr char* kMotorIndex = "motor_index";
	static const constexpr char* kMotorCurrent = "motor_current";
	static const constexpr char* kMotorSpeed = "motor_speed";

	RoboMasterC620Device();
	virtual ~RoboMasterC620Device();

	virtual error_condition Set(const string& key, \
			const unsigned int value);
	virtual error_condition Set(const string& key, \
			const vector<uint8_t>& value) {
		return kOperationNotSupported;
	}
	virtual error_condition Set(const string& key, const string& value) {
		return kOperationNotSupported;
	}
	virtual error_condition Get(const string& key, unsigned int& result);
	virtual error_condition Get(const string& key, \
			vector<uint8_t>& result) {
		return kOperationNotSupported;
	}
	virtual error_condition Get(const string& key, string& value) {
		return kOperationNotSupported;
	}
	virtual void ReceiveWork();
	virtual void TransmitWork();

protected:
	virtual error_condition CANIDToMotorID(const canid_t canid, \
			unsigned int& motor_id) {
		static const unsigned int kCANIDOffset = 0x201;
		int result = canid - kCANIDOffset;
		if (result < 0 || result >= kMaxMotorCount)
			return make_error_condition(errc::invalid_argument);
		motor_id = result;
		return kNoError;
	}
	virtual error_condition FrameIDToCANID(const unsigned int frame_id, \
			canid_t& can_id) {
		static const vector<int> kCANIDMap = { 0x200, 0x1ff };
		if (frame_id >= kCANIDMap.size())
			return make_error_condition(errc::invalid_argument);
		can_id = kCANIDMap[frame_id];
		return kNoError;
	}

private:
	enum LoopMode {
		kNone,
		kSpeed,
		kPosition
	};

	static const unsigned int kDefaultMotorCount = 4;
	static const unsigned int kMaxMotorCount = 8;

	struct ReceiveData {
		ReceiveData(const vector<uint8_t>& buffer) {
			static const unsigned int kMotors = 4;
			valid = false;
			if (buffer.size() != kMotors * (sizeof(uint16_t) / \
					sizeof(uint8_t)))
				return;
			angle = static_cast<int>(static_cast<int16_t>(\
					buffer[0] << 8 | buffer[1]));
			speed = static_cast<int>(static_cast<int16_t>(\
					buffer[2] << 8 | buffer[3]));
			current = static_cast<int>(static_cast<int16_t>(\
					buffer[4] << 8 | buffer[5]));
			temperature = static_cast<int>(static_cast<int16_t>(\
					buffer[4] << 8 | buffer[5]));
			valid = true;
		}
		bool valid;
		int angle;
		int speed;
		int current;
		int temperature;
	};

	struct TransmitData {
		TransmitData(const vector<int>& currents) {
			static const unsigned int kMotors = 4;
			buffer.resize(kMotors * sizeof(int16_t));
			fill(buffer.begin(), buffer.end(), 0);
			for (int i = 0; i < min(kMotors, \
					static_cast<unsigned int>(currents.size())); i++) {
				int value = currents[i];
				value = value < 32767 ? value : 32767;
				value = value > -32768 ? value : -32768;
				buffer[i * sizeof(int16_t)] = value >> 8;
				buffer[i * sizeof(int16_t) + 1] = value;
			}
		}
		vector<uint8_t> buffer;
	};

	struct MotorData {
		int target_current;
		int target_speed;
		int angle;
		int feedback_speed;
		int feedback_current;
		int temperature;
	};

	CANSocket can_socket_;
	NRTThread can_receive_thread_;
	NRTThread can_transmit_thread_;
	mutex can_receive_mutex_;
	mutex can_transmit_mutex_;
	atomic_uint motor_count_;
	vector<MotorData> motors_;
	atomic_int loop_mode_;
	atomic_int motor_index_;
	vector<PIDController> motor_speed_pid_;
};

#endif /* ROBO_MASTER_C620_DEVICE_H_ */
