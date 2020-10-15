#include "nrt_fpga_device.h"

#include <thread>
#include <string.h>

NRTFPGADevice::NRTFPGADevice() :
	nrt_fpga_fd_(-1) {
	CommonKeyPairs config = {
		{"fpga_device_file", "/dev/iofpga"}
	};
	if (ReConfig(config) != kNoError)
		return;
}

NRTFPGADevice::~NRTFPGADevice() {
	DeConfig();
}

error_condition NRTFPGADevice::ReConfig(const CommonKeyPairs& config) {
	ClearInitialized();
	close(nrt_fpga_fd_);
	error_condition result = kNoError;
	auto it_device_file = config.find(kFPGADeviceFile);
	if (it_device_file == config.end())
		return make_error_condition(errc::invalid_argument);

	int fpga_device_fd = open(it_device_file->second.c_str(), \
			O_RDWR | O_NONBLOCK);
	if (fpga_device_fd < 0)
		return make_error_condition(errc::io_error);

	result = IOFPGAGetInfo(fpga_device_fd);
	if (result != kNoError)
		return result;

	result = IOFPGAGetAndSetConf(fpga_device_fd);
	if (result != kNoError)
		return result;

	// Select data page
	ioctl(fpga_device_fd, IOFPGA_SEL_DATA, NULL);
	// TODO: Read FIFO entry count and Clear FIFO

	nrt_fpga_fd_ = fpga_device_fd;
	SetInitialized();

	return kNoError;
}

error_condition NRTFPGADevice::DeConfig() {
	ClearInitialized();
	close(nrt_fpga_fd_);
	return kNoError;
}

error_condition NRTFPGADevice::Get(const string& key, vector<uint8_t>& result) {
	static const error_condition kInvalidArgument(errc::invalid_argument);
	if (key.empty())
		return kInvalidArgument;
	if (key == kDIData) {
		uint16_t di_p_data = 0;
		uint16_t di_h_data = 0;
		IOFPGARead(nrt_fpga_fd_,\
				IOFPGA_DI_DATA_ADDR(info_.driver.di->data_offset, pdata),\
				&di_p_data,\
				sizeof(di_p_data));
		IOFPGARead(nrt_fpga_fd_,\
				IOFPGA_DI_DATA_ADDR(info_.driver.di->data_offset, hdata),\
				&di_h_data,\
				sizeof(di_h_data));
		result.resize(sizeof(di_p_data) + sizeof(di_h_data));
		result[0] = di_p_data & 0xff;
		result[1] = di_p_data >> 8 & 0xff;
		result[2] = di_h_data & 0xff;
	}
	else if (key == kDOData) {
		uint16_t do_p_data = 0;
		uint16_t do_h_data = 0;
		IOFPGARead(nrt_fpga_fd_, \
				IOFPGA_DO_DATA_ADDR(info_.driver._do->data_offset, pdata), \
				&do_p_data, \
				sizeof(do_p_data));
		IOFPGARead(nrt_fpga_fd_, \
				IOFPGA_DO_DATA_ADDR(info_.driver._do->data_offset, hdata), \
				&do_h_data, \
				sizeof(do_h_data));
		result.resize(sizeof(do_p_data) + sizeof(do_h_data));
		result[0] = do_p_data & 0xff;
		result[1] = do_p_data >> 8 & 0xff;
		result[2] = do_h_data & 0xff;
	}
	else if (key == kIMUData) {
		uint32_t imu_data_addr =
				IOFPGA_IMU_DATA_ADDR(info_.driver.imu->data_offset, data);
		struct st_iofpga_imu_value imu_value;
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, accx), \
				&imu_value.accx, sizeof(imu_value.accx));
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, accy), \
				&imu_value.accy, sizeof(imu_value.accy));
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, accz), \
				&imu_value.accz, sizeof(imu_value.accz));

		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, gyrox), \
				&imu_value.gyrox, sizeof(imu_value.gyrox));
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, gyroy), \
				&imu_value.gyroy, sizeof(imu_value.gyroy));
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, gyroz), \
				&imu_value.gyroz, sizeof(imu_value.gyroz));

		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, magx), \
				&imu_value.magx, sizeof(imu_value.magx));
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, magy), \
				&imu_value.magy, sizeof(imu_value.magy));
		IOFPGARead(nrt_fpga_fd_, IOFPGA_IMU_VALUE_ADDR(imu_data_addr, magz), \
				&imu_value.magz, sizeof(imu_value.magz));

		result.resize((sizeof(struct st_iofpga_imu_value)));
		result[0] = imu_value.accx;
		result[1] = imu_value.accx >> 8 & 0xff;
		result[2] = imu_value.accy;
		result[3] = imu_value.accy >> 8 & 0xff;
		result[4] = imu_value.accz;
		result[5] = imu_value.accz >> 8 & 0xff;

		result[6] = imu_value.gyrox;
		result[7] = imu_value.gyrox >> 8 & 0xff;
		result[8] = imu_value.gyroy;
		result[9] = imu_value.gyroy >> 8 & 0xff;
		result[10] = imu_value.gyroz;
		result[11] = imu_value.gyroz >> 8 & 0xff;

		result[12] = imu_value.magx;
		result[13] = imu_value.magx >> 8 & 0xff;
		result[14] = imu_value.magy;
		result[15] = imu_value.magy >> 8 & 0xff;
		result[16] = imu_value.magz;
		result[17] = imu_value.magz >> 8 & 0xff;
	}
	else
		return kInvalidArgument;

	return kNoError;
}

error_condition NRTFPGADevice::Set(const string& key, \
		const vector<uint8_t>& value) {
	static const error_condition kInvalidArgument(errc::invalid_argument);
	if (key.empty())
		return kInvalidArgument;
	if (key == kDOData) {
		uint16_t do_p_data = 0;
		uint16_t do_h_data = 0;
		if (value.size() < sizeof(do_p_data) + sizeof(do_h_data))
			return kInvalidArgument;

		do_p_data = value[0] | (value[1] << 8 & 0xff00);
		do_h_data = value[2];
		IOFPGAWrite(nrt_fpga_fd_, \
				IOFPGA_DO_DATA_ADDR(info_.driver._do->data_offset, pdata),\
				do_p_data, \
				sizeof(do_p_data));
		IOFPGAWrite(nrt_fpga_fd_, \
				IOFPGA_DO_DATA_ADDR(info_.driver._do->data_offset, hdata),\
				do_h_data, \
				sizeof(do_h_data));
	}
	else
		return kInvalidArgument;

	return kNoError;
}

error_condition NRTFPGADevice::IOFPGAWrite(int fd, int addr, uint64_t value, \
		uint32_t size) {
	int result;
	if (size < sizeof(uint8_t) || size > sizeof(uint64_t))
		return make_error_condition(errc::invalid_argument);

	ioctl(fd, IOFPGA_WR_ADDR, addr);
	result = write(fd, &value, size);
	if (result < 0)
		return make_error_condition(errc::io_error);

	return kNoError;
}

error_condition NRTFPGADevice::IOFPGARead(int fd, int addr, void * buffer, \
		int size)
{
	int result;
	ioctl(fd, IOFPGA_WR_ADDR, addr);
	result = read(fd, buffer, size);
	if (result < 0)
		return make_error_condition(errc::io_error);

	return kNoError;
}

error_condition NRTFPGADevice::IOFPGAGetInfo(int fd) {
	void **driver_info, ** hardware_info;

	info_sc_.mlnk.channel_count = 0;

	ioctl(fd, IOFPGA_SEL_INFO, NULL);
	IOFPGARead(fd, 0, &info_page_, sizeof(info_page_));

	memset(&info_, 0, sizeof(info_));

	info_.hardware.chip = (struct st_iofpga_info *)info_page_.data;

	for (int i = 0; i < info_.hardware.chip->fcount; i++)
	{
		switch (info_.hardware.chip->funcs[i].id)
		{
			case IOFPGA_HW_FUNCID_CLK:
				driver_info = reinterpret_cast<void **>(&info_.driver.clk);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.clk);
				break;
			case IOFPGA_HW_FUNCID_DI:
				driver_info = reinterpret_cast<void **>(&info_.driver.di);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.di);
				break;
			case IOFPGA_HW_FUNCID_DO:
				driver_info = reinterpret_cast<void **>(&info_.driver._do);
				hardware_info = reinterpret_cast<void **>(&info_.hardware._do);
				break;
			case IOFPGA_HW_FUNCID_AI:
				driver_info = reinterpret_cast<void **>(&info_.driver.ai);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.ai);
				break;
			case IOFPGA_HW_FUNCID_AO:
				driver_info = reinterpret_cast<void **>(&info_.driver.ao);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.ao);
				break;
			case IOFPGA_HW_FUNCID_DDA:
				driver_info = reinterpret_cast<void **>(&info_.driver.dda);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.dda);
				break;
			case IOFPGA_HW_FUNCID_CAP:
				driver_info = reinterpret_cast<void **>(&info_.driver.cap);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.cap);
				break;
			case IOFPGA_HW_FUNCID_IMU:
				driver_info = reinterpret_cast<void **>(&info_.driver.imu);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.imu);
				break;
			case IOFPGA_HW_FUNCID_MLNK0:
				driver_info = reinterpret_cast<void **>(&info_.driver.mlnk[0]);
				hardware_info = \
						reinterpret_cast<void **>(&info_.hardware.mlnk[0]);
				info_sc_.mlnk.channel_count++;
				break;
			case IOFPGA_HW_FUNCID_MLNK1:
				driver_info = reinterpret_cast<void **>(&info_.driver.mlnk[1]);
				hardware_info = \
						reinterpret_cast<void **>(&info_.hardware.mlnk[1]);
				info_sc_.mlnk.channel_count++;
				break;
			case IOFPGA_HW_FUNCID_SC_0_0:
				driver_info = reinterpret_cast<void **>(&info_.driver.sc);
				hardware_info = reinterpret_cast<void **>(&info_.hardware.sc);
				break;
			default:
				driver_info = NULL;
				hardware_info = NULL;
				break;
		}

		if (driver_info != NULL)
			* driver_info =  &info_.hardware.chip->funcs[i];
		if (hardware_info != NULL)
			* hardware_info = \
				&info_page_.data[info_.hardware.chip->funcs[i].info_offset];
	}

	return kNoError;
}

error_condition NRTFPGADevice::IOFPGAGetAndSetConf(int fd) {
	error_condition result = kNoError;
	struct st_iofpga_clk_conf clk_conf;
	char iofpga_dda_data_bits;

	/* Get field bus slave number */
	memset(info_sc_.mlnk.slave_count, 0, sizeof(info_sc_.mlnk.slave_count));

	ioctl(fd, IOFPGA_SEL_CONF, NULL);
	IOFPGARead(fd, 0, &conf_page_, sizeof(conf_page_));

	for (int i = 0; i < info_sc_.mlnk.channel_count; i++)
		IOFPGARead(fd, \
				IOFPGA_MLNK_CONF_ADDR(\
				info_.driver.mlnk[i]->conf_offset, sl_cnt),\
				&info_sc_.mlnk.slave_count[i],\
				sizeof(char));

	/* Get IOFPGA maximum DDA data value */
	IOFPGARead(fd, \
			IOFPGA_CLK_CONF_ADDR(info_.driver.clk->conf_offset, intmask),\
			&clk_conf,\
			sizeof(clk_conf));

	switch (clk_conf.con[clk_conf.syncout] >> 4)
	{
		case IOFPGA_CLK_CYCLE_1MS:
			info_sc_.sync.time_us = 1000;
			info_sc_.sync.freq = 1000;
			iofpga_dda_data_bits = info_.hardware.dda->databits1ms;
			break;
		case IOFPGA_CLK_CYCLE_2MS:
			info_sc_.sync.time_us = 2000;
			info_sc_.sync.freq = 500;
			iofpga_dda_data_bits = info_.hardware.dda->databits2ms;
			break;
		case IOFPGA_CLK_CYCLE_4MS:
			info_sc_.sync.time_us = 4000;
			info_sc_.sync.freq = 250;
			iofpga_dda_data_bits = info_.hardware.dda->databits4ms;
			break;
		case IOFPGA_CLK_CYCLE_8MS:
			info_sc_.sync.time_us = 8000;
			info_sc_.sync.freq = 125;
			iofpga_dda_data_bits = info_.hardware.dda->databits8ms;
			break;
		default:
			info_sc_.sync.time_us = 1000;
			iofpga_dda_data_bits = info_.hardware.dda->databits1ms;
			break;
	}

	if (IOFPGA_DDA_MAX_FREQ < \
			info_sc_.sync.freq * (1 << iofpga_dda_data_bits) / 2)
		info_sc_.dda.data_max = IOFPGA_DDA_MAX_FREQ / info_sc_.sync.freq;
	else
		info_sc_.dda.data_max = (1 << iofpga_dda_data_bits) / 2;

	// Disable FPGA
	result = IOFPGAWrite(fd, \
			IOFPGA_CONFPAGE_ADDR(regs.en), \
			0xaa550000, \
			sizeof(conf_page_.regs.en));
	if (result != kNoError)
		return result;
	// Enable high speed capture input channel
	result = IOFPGAWrite(fd, \
			IOFPGA_DI_CONF_ADDR(info_.driver.di->conf_offset, hpen), \
			0x00ff, \
			2);
	if (result != kNoError)
		return result;
	// Set capture AB pulse capture mode
	result = IOFPGAWrite(fd, \
			IOFPGA_CAP_CONF_ADDR(info_.driver.cap->conf_offset, conf),\
			0xff, \
			1);
	if (result != kNoError)
		return result;
	result = IOFPGAWrite(fd,\
			IOFPGA_CAP_CONF_ADDR(info_.driver.cap->conf_offset, en),\
			0x0f,\
			1);
	if (result != kNoError)
		return result;
	// Reconfigure FPGA
	result = IOFPGAWrite(fd, IOFPGA_CONFPAGE_ADDR(regs.reconf), 0x55aa0001, 4);
	if (result != kNoError)
		return result;
	// Enable FPGA
	result = IOFPGAWrite(fd, IOFPGA_CONFPAGE_ADDR(regs.en), 0xaa550001, 4);
	if (result != kNoError)
		return result;

	return kNoError;
}

