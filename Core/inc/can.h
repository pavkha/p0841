#pragma once

#include "MDR32F9Qx_can.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include <cstdint>

using CanRxMsg = CAN_RxMsgTypeDef;
using CanTxMsg = CAN_TxMsgTypeDef;
using CanMsgData = CAN_DataTypeDef;

static auto const CanTransmit = CAN_Transmit;

static auto const Can1Instance{MDR_CAN1};
static auto const Can2Instance{MDR_CAN2};

enum : std::uint8_t {
	cmdMailBox = 0,
	errorMailBox = 16,
	okMailBox,
	telemetryMailBox
};

enum CanMode
{
	CAN_MODE_NORMAL,
	CAN_MODE_READ_ONLY,
	CAN_MODE_SELF_TEST,
	CAN_MODE_INIT
};

enum CanBaudrate
{
	f100kHz = 159,
	f125kHz = 127,
	f250kHz = 63,
	f500kHz = 31,
	f1000kHz = 15
};

void CanGpioInit(MDR_CAN_TypeDef*);
void CanClkInit(MDR_CAN_TypeDef*);
void CanFiltersInit(MDR_CAN_TypeDef*);
void CanInterruptsInit(MDR_CAN_TypeDef*);
void CanMailBoxesInit(MDR_CAN_TypeDef*);
void CanBaudrateInit(MDR_CAN_TypeDef*, CanBaudrate);
void CanInit(MDR_CAN_TypeDef*, CanBaudrate);
void Can1Init(void);
