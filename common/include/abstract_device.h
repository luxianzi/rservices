#ifndef ABSTRACTDEVICE_H_
#define ABSTRACTDEVICE_H_

#include <stdint.h>
#include <vector>
#include <atomic>
#include "rservice.h"
#include "error_category.h"

class AbstractDevice {
public:
	AbstractDevice() :
		is_initiated_(false),
		is_busy_(false) { }
	virtual ~AbstractDevice() { }

	virtual error_condition ReConfig(const CommonKeyPairs& config) = 0;
	virtual error_condition DeConfig() = 0;
	virtual error_condition Set(const string& key, \
			const unsigned int value) = 0;
	virtual error_condition Set(const string& key, const int value) {
		return Set(key, static_cast<unsigned int>(value));
	}
	virtual error_condition Set(const string& key, \
			const vector<uint8_t>& value) = 0;
	virtual error_condition Get(const string& key, unsigned int& result) = 0;
	virtual error_condition Get(const string& key, int& result) {
		return Get(key, reinterpret_cast<unsigned int&>(result));
	}
	virtual error_condition Get(const string& key, \
			vector<uint8_t>& result) = 0;
	virtual bool IsInitiated() { return is_initiated_.load(); }
	virtual bool IsBusy() { return is_busy_.load(); }
	virtual void SetInitiated() { is_initiated_.store(true); }
	virtual void ClearInitiated() { is_initiated_.store(false); }
	virtual void SetBusy() { is_busy_.store(true); }
	virtual void ClearBusy() { is_busy_.store(false); }

private:
	atomic_bool is_initiated_;
	atomic_bool is_busy_;
};

#endif /* ABSTRACTDEVICE_H_ */
