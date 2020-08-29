/*
 * AbstractSocket.h
 *
 *  Created on: 2020��8��22��
 *      Author: Luxianzi
 */

#ifndef ABSTRACTSOCKET_H_
#define ABSTRACTSOCKET_H_

#include <string>
#include <map>
#include <system_error>
#include <vector>

using namespace std;

using SocketConfig = map<string, string>;

class AbstractSocket {
public:
	AbstractSocket() {}
	AbstractSocket(SocketConfig& config) {}
	virtual ~AbstractSocket() = 0;

	virtual error_condition	ReConfig(SocketConfig& config) = 0;
	virtual error_condition Read(vector<uint8_t>& buffer) = 0;
	virtual error_condition Write(const vector<uint8_t>& buffer) = 0;
	virtual bool IsConnected() = 0;
};

#endif /* ABSTRACTSOCKET_H_ */
