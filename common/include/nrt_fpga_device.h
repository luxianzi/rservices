#ifndef NRT_FPGA_DEVICE_H_
#define NRT_FPGA_DEVICE_H_

#include "abstract_device.h"
#include "driver.h"
#include "nrt_thread.h"
#include <vector>
#include <mutex>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>



class NRTFPGADevice : public AbstractDevice {
public:
	static const constexpr char* kFPGADeviceFile = "fpga_device_file";
	static const constexpr char* kDIData = "di_data";
	static const constexpr char* kDOData = "do_data";
	static const constexpr char* kIMUData = "imu_data";

	NRTFPGADevice();
	NRTFPGADevice(CommonKeyPairs& config);
	~NRTFPGADevice();

	virtual error_condition	ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig();
	virtual error_condition Set(const string& key, \
			const vector<uint8_t>& value);
	virtual error_condition Set(const string& key, const string& value) {
		return kOperationNotSupported;
	}
	virtual error_condition Set(const string& key, const unsigned int value) {
		return kOperationNotSupported;
	}
	error_condition Get(const string& key, vector<uint8_t>& result);
	virtual error_condition Get(const string& key, unsigned int& result) {
		return kOperationNotSupported;
	}
	virtual error_condition Get(const string& key, string& value) {
		return kOperationNotSupported;
	}

protected:
	error_condition IOFPGAWrite(int fd, int addr, uint64_t value, uint32_t size);
	error_condition IOFPGARead(int fd, int addr, void * buffer, int size);
	error_condition IOFPGAGetInfo(int fd);
	error_condition IOFPGAGetAndSetConf(int fd);

private:
	int nrt_fpga_fd_;

	struct IOFPGADriverInfo {
		struct st_iofpga_func_info * clk;
		struct st_iofpga_func_info * di;
		struct st_iofpga_func_info * _do;
		struct st_iofpga_func_info * ai;
		struct st_iofpga_func_info * ao;
		struct st_iofpga_func_info * dda;
		struct st_iofpga_func_info * cap;
		struct st_iofpga_func_info * imu;
		struct st_iofpga_func_info * mlnk[2];
		struct st_iofpga_func_info * sc;
	};

	struct IOFPGAHardwareInfo {
		struct st_iofpga_info * chip;
		struct st_iofpga_clk_info * clk;
		struct st_iofpga_di_info * di;
		struct st_iofpga_do_info * _do;
		struct st_iofpga_ai_info * ai;
		struct st_iofpga_ao_info * ao;
		struct st_iofpga_dda_info * dda;
		struct st_iofpga_cap_info * cap;
		struct st_iofpga_imu_info * imu;
		struct st_iofpga_mlnk_info * mlnk[2];
		struct st_iofpga_sc_0_0_info * sc;
	};

	struct IOFPGAGetInfoCache {
		IOFPGADriverInfo driver;
		IOFPGAHardwareInfo hardware;
	};

	struct IOFPGAMlnkInfoSC {
		int channel_count;
		char slave_count[2];
	};

	struct IOFPGASyncInfoSC {
		int time_us;
		int freq;
	};

	struct IOFPGADdaInfoSC {
		int data_max;
	};

	struct IOFPGAInfoSC {
		IOFPGAMlnkInfoSC mlnk;
		IOFPGASyncInfoSC sync;
		IOFPGADdaInfoSC dda;
	};

	union st_iofpga_info_page info_page_;
	union st_iofpga_conf_page conf_page_;
	IOFPGAGetInfoCache info_;
	IOFPGAInfoSC info_sc_;
};


#endif /* NRT_FPGA_DEVICE_H_ */
