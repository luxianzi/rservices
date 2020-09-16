#include "rservice.h"
#include "robo_master_c620_device.h"
#include "logger.h"

int main(int argc, char* argv[])
{
	Logger logger;

	logger.Log(Logger::kInfo, "hello world", kCodeLocation);

	return 0;
}
