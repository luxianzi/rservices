/*
 * NRTMessageQueue.h
 *
 *  Created on: 2020��8��23��
 *      Author: Luxianzi
 */

#ifndef NRTMESSAGEQUEUE_H_
#define NRTMESSAGEQUEUE_H_

#include <mqueue.h>
#include "abstract_message_queue.h"

class NRTMessageQueue: public AbstractMessageQueue {
public:
	const unsigned int kNormalPriority = 50;

	NRTMessageQueue(string& name);
	virtual ~NRTMessageQueue();

	virtual error_condition Send(Message& message);
	virtual error_condition Receive(Message& message);
	virtual error_condition EmptyDownStream();

private:
	mqd_t up_stream_queue_descriptor_;
	mqd_t down_stream_queue_descriptor_;
	bool ready_;
};

#endif /* NRTMESSAGEQUEUE_H_ */
