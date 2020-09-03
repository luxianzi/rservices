#ifndef NRTTHREAD_H_
#define NRTTHREAD_H_

#include <thread>
#include "rservice.h"
#include "abstract_thread.h"

class NRTThread: public AbstractThread {
public:
	NRTThread();
	NRTThread(WorkFunction work);
	virtual ~NRTThread();

	virtual void Start(const bool one_shot = false);
	virtual void Stop();

private:
	thread thread_;
};

#endif /* NRTTHREAD_H_ */
