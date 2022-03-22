#include "can.h"

extern "C" void CAN1_IRQHandler(void) {

	extern void MacsIrqHandler(void);

	CAN_ITClearRxTxPendingBit(MDR_CAN1, 0, CAN_STATUS_RX_READY);

	MacsIrqHandler();
}

extern "C" void CAN2_IRQHandler(void) {

}

void CanGpioInit(MDR_CAN_TypeDef *can_instance) {
	PORT_InitTypeDef CANPortInit;

	PORT_StructInit(&CANPortInit);
	CANPortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
	CANPortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	CANPortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
	CANPortInit.PORT_PD = PORT_PD_DRIVER;
	CANPortInit.PORT_GFEN = PORT_GFEN_OFF;
	CANPortInit.PORT_FUNC = PORT_FUNC_MAIN;
	CANPortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
	CANPortInit.PORT_MODE = PORT_MODE_DIGITAL;

	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTE, ENABLE);
	PORT_Init(MDR_PORTE, &CANPortInit);
	CANPortInit.PORT_OE = PORT_OE_IN;

	if(can_instance == MDR_CAN1) {
		CANPortInit.PORT_Pin = PORT_Pin_11;
		PORT_Init(MDR_PORTE, &CANPortInit);
		CANPortInit.PORT_Pin = PORT_Pin_12;
	} else if(can_instance == MDR_CAN2) {
		CANPortInit.PORT_Pin = PORT_Pin_13;
		PORT_Init(MDR_PORTE, &CANPortInit);
		CANPortInit.PORT_Pin = PORT_Pin_14;
	}

	CANPortInit.PORT_OE = PORT_OE_OUT;
	PORT_Init(MDR_PORTE, &CANPortInit);
}

void CanFiltersInit(MDR_CAN_TypeDef *can_instance) {
	CAN_FilterInitTypeDef filter_struct;

	if(can_instance == MDR_CAN1) {
		filter_struct.Mask_ID = CAN_STDID_TO_EXTID(0x7FF);
		filter_struct.Filter_ID = CAN_STDID_TO_EXTID(0x40);
		CAN_FilterInit(can_instance, 0, &filter_struct);
	} else if(can_instance == MDR_CAN2) {
		CAN_FilterInit(can_instance, 0, &filter_struct);
	}
}

void CanInterruptsInit(MDR_CAN_TypeDef *can_instance) {
	if(can_instance == MDR_CAN1) {
		NVIC_EnableIRQ(CAN1_IRQn);
		NVIC_SetPriority(CAN1_IRQn, 1);
		CAN_RxITConfig(can_instance, 1, ENABLE);
		CAN_ITConfig(can_instance, CAN_IT_GLBINTEN | CAN_IT_RXINTEN, ENABLE);
	} else if(can_instance == MDR_CAN2) {
		NVIC_EnableIRQ(CAN2_IRQn);
		NVIC_SetPriority(CAN2_IRQn, 1);
		CAN_RxITConfig(can_instance, 0, ENABLE);
		CAN_ITConfig(can_instance, CAN_IT_GLBINTEN | CAN_IT_RXINTEN, ENABLE);
	}
}

void CanMailBoxesInit(MDR_CAN_TypeDef *can_instance) {
	if(can_instance == MDR_CAN1) {
		CAN_Receive(MDR_CAN1, 0, ENABLE);
	} else if(can_instance == MDR_CAN2) {
		CAN_Receive(MDR_CAN2, 0, ENABLE);
	}
}

void CanBaudrateInit(MDR_CAN_TypeDef *canInstance, CanBaudrate br) {
	CAN_InitTypeDef canInitStruct;

	CAN_StructInit(&canInitStruct);
	canInitStruct.CAN_PSEG = CAN_PSEG_Mul_2TQ;
	canInitStruct.CAN_SEG1 = CAN_SEG1_Mul_3TQ;
	canInitStruct.CAN_SEG2 = CAN_SEG2_Mul_2TQ;
	canInitStruct.CAN_SJW = CAN_SJW_Mul_3TQ;
	canInitStruct.CAN_SB = CAN_SB_3_SAMPLE;
	canInitStruct.CAN_BRP = br;
	CAN_Init(canInstance, &canInitStruct);
}

void CanClkInit(MDR_CAN_TypeDef *canInstance)
{
	CAN_BRGInit(canInstance, CAN_HCLKdiv1);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_CAN1, ENABLE);
}

void CanInit(MDR_CAN_TypeDef *canInstance, CanBaudrate br)
{
	CanGpioInit(canInstance);
	CanClkInit(canInstance);
	CanBaudrateInit(canInstance, br);
	CanFiltersInit(canInstance);
	CanInterruptsInit(canInstance);
	CanMailBoxesInit(canInstance);
	CAN_Cmd(canInstance, ENABLE);
}

void Can1Init(void)
{
	CanInit(MDR_CAN1, CanBaudrate::f1000kHz);
}
