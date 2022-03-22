#include <sys_clk.h>

void SysClkInit() {
	RST_CLK_DeInit();

	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	while (RST_CLK_HSEstatus() != SUCCESS) {
		__NOP();
	}

	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul8);
	RST_CLK_CPU_PLLcmd(ENABLE);

	while (RST_CLK_CPU_PLLstatus() != SUCCESS) {
		__NOP();
	}

	RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
	RST_CLK_CPU_PLLuse(ENABLE);
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

	RST_CLK_PCLKcmd(RST_CLK_PCLK_RST_CLK, ENABLE);
}
