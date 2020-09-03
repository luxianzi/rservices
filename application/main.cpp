#include "rservice.h"
#include "robo_master_c620_device.h"

int main(int argc, char* argv[])
{
	RoboMasterC620Device c620;

	while (1) {
		c620.Set(RoboMasterC620Device::kMotorIndex, 0);
		c620.Set(RoboMasterC620Device::kMotorSpeed, 10);
	}

	return 0;
}
