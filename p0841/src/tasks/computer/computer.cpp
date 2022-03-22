#include "../../app/app.h"
#include "computer.h"

namespace tasks {

namespace computer {

using namespace app;

SBCPowerOff SBCPowerOffState;
SBCPowerOn SBCPowerOnState;
SBCBusyReset SBCBusyResetState;
SBCBusyPowerOff SBCBusyAfterPowerOffState;
SBCBusyPowerOn SBCBusyAfterPowerOnState;

SBC::SBC() {
	PinReset(powerPort, powerPin);
	changeState(&SBCPowerOffState);
}

bool SBC::powerOn() {
	return state->powerOn(this);
}

bool SBC::powerOff() {
	return state->powerOff(this);
}

void SBC::forcePowerOff() {
	PinReset(powerPort, powerPin);
	changeState(&SBCBusyAfterPowerOffState);
}

ReadPinType SBC::checkPowerOn() {
	return state->checkPowerOn(this);
}

bool SBC::resetStart() {
	return state->resetStart(this);
}

bool SBC::resetRelease() {
	return state->resetRelease(this);
}

void SBC::process() {
	state->process(this);
}

void SBC::changeState(SBCState *s) {
	state = s;
}

auto SBC::getState() -> std::uint8_t {
	return state->getState();
}

void SBCState::changeState(SBC *sbc, SBCState *s) {
	sbc->changeState(s);
}

bool SBCPowerOff::powerOn(SBC *sbc) {
	PinSet(sbc->powerPort, sbc->powerPin);
	sbc->setTimer(GetSysTime());
	changeState(sbc, &SBCBusyAfterPowerOnState);
	return true;
}

auto SBCPowerOff::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(SBC::states::powerOff);
}

bool SBCPowerOn::resetStart(SBC *sbc) {
	PinReset(sbc->resetPort, sbc->resetPin);
	sbc->setTimer(GetSysTime());
	changeState(sbc, &SBCBusyResetState);
	return true;
}

bool SBCPowerOn::powerOff(SBC *sbc) {
	PinReset(sbc->powerPort, sbc->powerPin);
	sbc->setTimer(GetSysTime());
	changeState(sbc, &SBCBusyAfterPowerOffState);
	return true;
}

auto SBCPowerOn::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(SBC::states::powerOn);
}

void SBCBusyPowerOff::process(SBC *sbc) {
	if(GetSysTime() > sbc->getTimer() + sbc->afterPowerOffDelayMs) {
		changeState(sbc, &SBCPowerOffState);
	}
}

auto SBCBusyPowerOff::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(SBC::states::powerOff);
}

void SBCBusyPowerOn::process(SBC *sbc) {
	if(GetSysTime() > sbc->getTimer() + sbc->afterPowerOnDelayMs) {
		changeState(sbc, &SBCPowerOnState);
	}
}

auto SBCBusyPowerOn::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(SBC::states::powerOn);
}

void SBCBusyReset::process(SBC *sbc) {
	if(GetSysTime() > sbc->getTimer() + sbc->resetPulseTimeMs) {
		PinSet(sbc->resetPort, sbc->resetPin);
		sbc->setTimer(GetSysTime());
		changeState(sbc, &SBCBusyAfterPowerOnState);
	}
}

auto SBCBusyReset::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(SBC::states::powerOn);
}

}//namespace sbc

}//namespace tasks
