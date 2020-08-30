#ifndef CAN_SOCKET_H_
#define CAN_SOCKET_H_

#include <linux/can/raw.h>
#include "abstract_socket.h"
#include "error_category.h"

using namespace std;

class CANSocket: public AbstractSocket {
public:
	static const constexpr char* kInterface = "interface";
	static const constexpr char* kBitRate = "bit_rate";
	static const constexpr char* kControlMode = "control_mode";

	CANSocket();
	CANSocket(SocketConfig& config);
	virtual ~CANSocket();

	virtual error_condition	ReConfig(SocketConfig& config);
	virtual error_condition Read(vector<uint8_t>& result);
	virtual error_condition Write(const vector<uint8_t>& value);
	virtual bool IsConnected();
	virtual error_condition GetReadCANID(canid_t& id);
	virtual error_condition SetWriteCANID(const canid_t id);

private:
	int can_socket_;
	bool connected_;
	canid_t read_canid_;
	canid_t write_canid_;
};

#endif /* CAN_SOCKET_H_ */
