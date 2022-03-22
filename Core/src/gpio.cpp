#include "gpio.h"

void GpioInit() {
	PORT_InitTypeDef PORT_InitStructure;

	RST_CLK_PCLKcmd(
			RST_CLK_PCLK_PORTA |
			RST_CLK_PCLK_PORTC |
			RST_CLK_PCLK_PORTD,
			ENABLE);

	PORT_InitStructure.PORT_Pin = (PORT_Pin_9 | PORT_Pin_13);
	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTA, &PORT_InitStructure);

	PORT_InitStructure.PORT_Pin = (PORT_Pin_8 | PORT_Pin_9);
	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_Init(MDR_PORTC, &PORT_InitStructure);

	PORT_InitStructure.PORT_Pin = (
			PORT_Pin_2 |
			PORT_Pin_3 |
			PORT_Pin_4 |
			PORT_Pin_5 |
			PORT_Pin_6);
	PORT_InitStructure.PORT_OE = PORT_OE_IN;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_Init(MDR_PORTD, &PORT_InitStructure);
}
