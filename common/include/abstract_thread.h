/*
 * AbstractThread.h
 *
 *  Created on: 2020Äê8ÔÂ16ÈÕ
 *      Author: Luxianzi
 */

#ifndef ABSTRACTTHREAD_H_
#define ABSTRACTTHREAD_H_

#include <functional>

using namespace std;

using WorkFunction = function<void()>;

class AbstractThread {
public:
	AbstractThread(WorkFunction work) :
		work_(work),
		one_shot_(false),
		quit_(false) {}
	virtual ~AbstractThread() {}

	virtual void Start(const bool one_shot = false) = 0;
	virtual void Stop() = 0;

protected:
	virtual void SetOneShot() {
		one_shot_.store(true);
	}
	virtual void SignalQuit() {
		quit_.store(true);
	}
	virtual void Work() {
		do {
			work_();
		} while (!one_shot_.load() && !quit_.load());
	}

private:
	WorkFunction work_;
	atomic_bool one_shot_;
	atomic_bool quit_;
};

#endif /* ABSTRACTTHREAD_H_ */
