/*
 * NRTThread.h
 *
 *  Created on: 2020��8��23��
 *      Author: Luxianzi
 */

#ifndef NRTTHREAD_H_
#define NRTTHREAD_H_

#include <thread>
#include <atomic>
#include "abstract_thread.h"

using namespace std;

class NRTThread: public AbstractThread {
public:
	NRTThread(WorkFunction work);
	virtual ~NRTThread();

	virtual void Start(const bool one_shot = false);
	virtual void Stop();

private:
	thread thread_;
};

#endif /* NRTTHREAD_H_ */
