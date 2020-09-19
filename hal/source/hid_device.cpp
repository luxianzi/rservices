#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/input.h>
#include "hid_device.h"

HIDDevice::HIDDevice(const string& name) :
	hid_receive_thread_([&](){ this->ReceiveWork(); }) {
	CommonKeyPairs config = {
			{"hid_name", name},
	};
	if (ReConfig(config) != kNoError)
		return;
	SetInitialized();
	hid_receive_thread_.Start();
}

HIDDevice::~HIDDevice() {

}

void HIDDevice::IsUpdated() {

}

string HIDDevice::FindKeyName(int key_code) {
	return keys[key_code];
}

string HIDDevice::FindAbsName(int abs_code) {
	return absolutes[abs_code];
}

error_condition HIDDevice::ReConfig(const CommonKeyPairs &config) {
	ClearInitialized();
	auto hid_name = config.find(kHIDName);
	if (hid_name == config.end())
		return make_error_condition(errc::invalid_argument);
	struct dirent** name_list;
	auto name_check = [](const struct dirent* dir) {
		return static_cast<int>(\
				string(dir->d_name).find(kEventInterfaceName) == 0);
	};
	int device_count = scandir(kHIDDirectory, &name_list, name_check, \
			alphasort);
	if (device_count <= 0)
		return make_error_condition(errc::no_such_device);
	for (int i = 0; i < device_count; i++) {
		string device_file_name = string(kHIDDirectory) + "/" + \
				name_list[i]->d_name;
		free(name_list[i]);
		int hid_fd = open(device_file_name.c_str(), O_RDONLY);
		if (hid_fd < 0)
			continue;
		string device_name;
		device_name.resize(256);
		ioctl(hid_fd, EVIOCGNAME(device_name.size()), device_name.data());
		if (string(device_name.c_str()) != \
				string(hid_name->second.c_str()) == 0) {
			close(hid_fd);
			return make_error_condition(errc::invalid_argument);
		}
		hid_fd_ = hid_fd;
		SetInitialized();
		break;
	}
	return kNoError;;
}

error_condition HIDDevice::DeConfig() {
	close(hid_fd_);
	return kNoError;
}

void HIDDevice::ReceiveWork() {
	if (!IsInitialized()) {
		this_thread::sleep_for(chrono::milliseconds(10));
		return;
	}
	struct input_event event;
	int result = read(hid_fd_, &event, sizeof(struct input_event));
	if (result < 0)
		return;
	auto update_keys = [&](const string& key, const unsigned int value) {
		if (key.empty())
			return;
		hid_map_[key] = value;
	};
	if (event.type == EV_KEY) {
		lock_guard<mutex> lock(hid_update_mutex_);
		update_keys(FindKeyName(event.code), event.value);
		hid_update_cond_.notify_one();
	}
	else if (event.type == EV_ABS) {
		lock_guard<mutex> lock(hid_update_mutex_);
		if (event.code == ABS_Z || event.code == ABS_RZ)
			update_keys(FindAbsName(event.code), event.value);
		else if (event.code == ABS_HAT0X || event.code == ABS_HAT0Y)
			update_keys(FindAbsName(event.code), event.value);
		else
			update_keys(FindAbsName(event.code), event.value >> 8);
		hid_update_cond_.notify_one();
	}
}

error_condition HIDDevice::Set(const string& key, const unsigned int value) {
	return kNoError;
}

error_condition HIDDevice::Set(const string& key, const vector<uint8_t>& value) {
	return kNoError;
}

error_condition HIDDevice::Get(const string& key, unsigned int& result) {
	if (!IsInitialized())
		return make_error_condition(errc::no_such_device);
	if (key == kKeyUpdated) {
		unique_lock<mutex> lock(hid_update_mutex_);
		hid_update_cond_.wait(lock);
		lock.unlock();
		return kNoError;
	}
	if (hid_map_.find(key) == hid_map_.end())
		return make_error_condition(errc::invalid_argument);
	result = hid_map_[key];
	return kNoError;
}


error_condition HIDDevice::Get(const string& key, vector<uint8_t>& result) {
	return kNoError;
}
