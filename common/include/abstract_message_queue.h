/*
 * AbstractMessageQueue.h
 *
 *  Created on: 2020��8��16��
 *      Author: Luxianzi
 */

#ifndef ABSTRACTMESSAGEQUEUE_H_
#define ABSTRACTMESSAGEQUEUE_H_

#include <stdint.h>
#include <system_error>
#include <vector>
#include "message.h"

using namespace std;

class AbstractMessageQueue {
public:
	const string kDownStreamSuffix = "_in";
	const string kUpStreamSuffix = "_out";
	const unsigned int kMaxMessageNumber = 16;
	const unsigned int kMaxMessageSize = 4096;

	AbstractMessageQueue(string& name) :
		name_(name) {}
	virtual ~AbstractMessageQueue();

	virtual error_condition Send(Message& message) = 0;
	virtual error_condition Receive(Message& message) = 0;
	virtual error_condition EmptyDownStream() = 0;
	virtual string GetName() {
		return name_;
	}

private:
	string name_;

};

#endif /* ABSTRACTMESSAGEQUEUE_H_ */
