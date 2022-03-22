#include <sys_clk.h>
#include "macs_system.hpp"
#include "macs_rtos.hpp"
#include "gpio.h"
#include "can.h"

void System::InitCpu() {
	SysClkInit();
	GpioInit();
	Can1Init();
}

void System::HardFaultHandler() {
	while(true) {
		__NOP();
	}
}
