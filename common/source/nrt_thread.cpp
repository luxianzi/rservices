#include "nrt_thread.h"

NRTThread::NRTThread() :
	AbstractThread() {

}

NRTThread::NRTThread(WorkFunction work) :
	AbstractThread(work) {

}

NRTThread::~NRTThread() {
	Stop();
}

void NRTThread::Start(const bool one_shot) {
	if (one_shot)
		SetOneShot();
	thread_ = thread(&NRTThread::Work, this);
}

void NRTThread::Stop() {
	SignalQuit();
	if (thread_.joinable())
		thread_.join();
}
