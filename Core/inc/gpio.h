#pragma once

#include "main.h"
#include <cstdint>

using PortType = MDR_PORT_TypeDef;
using PinType = std::uint32_t;
using ReadPinType = std::uint8_t;

enum GPInStates : std::uint8_t {
	pinReset,
	pinSet
};

static auto const PinSet = PORT_SetBits;
static auto const PinReset = PORT_ResetBits;
static auto const ReadPin = PORT_ReadInputDataBit;

static auto const SBCPowerPort{MDR_PORTA};
static auto const SBCPowerPin{PORT_Pin_9};

static auto const SBCFeedbackPowerPort{MDR_PORTD};
static auto const SBCFeedbackPowerPin{PORT_Pin_2};

static auto const SBCResetPort{MDR_PORTA};
static auto const SBCResetPin{PORT_Pin_9};

static auto const CamPowerPort{MDR_PORTA};
static auto const CamPowerPin{PORT_Pin_13};

static auto const CamFeedbackPowerPort{MDR_PORTD};
static auto const CamFeedbackPowerPin{PORT_Pin_3};

static auto const WashOnPort{MDR_PORTC};
static auto const WashOnPin{PORT_Pin_9};

static auto const WashFeedbackOnPort{MDR_PORTD};
static auto const WashFeedbackOnPin{PORT_Pin_4};

static auto const CurtOnPort{MDR_PORTC};
static auto const CurtOnPin{PORT_Pin_8};

static auto const CurtFeedbackOnPort{MDR_PORTD};
static auto const CurtFeedbackOnPin{PORT_Pin_5};

static auto const CurtFeedbackOpenPort{MDR_PORTD};
static auto const CurtFeedbackOpenPin{PORT_Pin_6};

static auto const CurtFeedbackClosePort{MDR_PORTD};
static auto const CurtFeedbackClosePin{PORT_Pin_6};

void GpioInit();
