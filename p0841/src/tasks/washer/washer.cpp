#include "washer.h"
#include "../../app/app.h"

namespace tasks {

namespace washer {

using namespace app;

WashOff WashOffState;
WashOn WashOnState;
WashBusyOff WashBusyAfterOffState;
WashBusyOn WashBusyAfterOnState;

Wash::Wash() {
	PinReset(onPort, onPin);
	changeState(&WashOffState);
}

bool Wash::on() {
	return state->on(this);
}

bool Wash::off() {
	return state->off(this);
}

void Wash::forceOff() {
	PinReset(onPort, onPin);
	changeState(&WashBusyAfterOffState);
}

ReadPinType Wash::checkOn() {
	return state->checkOn(this);
}

void Wash::process() {
	state->process(this);
}

void Wash::changeState(WashState *s) {
	state = s;
}

auto Wash::getState() -> std::uint8_t {
	return state->getState();
}

void WashState::changeState(Wash *wash, WashState *s) {
	wash->changeState(s);
}

bool WashOff::on(Wash *wash) {
	PinSet(wash->onPort, wash->onPin);
	wash->setTimer(GetSysTime());
	changeState(wash, &WashBusyAfterOnState);
	return true;
}

auto WashOff::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Wash::states::off);
}

bool WashOn::off(Wash *wash) {
	PinReset(wash->onPort, wash->onPin);
	wash->setTimer(GetSysTime());
	changeState(wash, &WashBusyAfterOffState);
	return true;
}

auto WashOn::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Wash::states::on);
}

void WashBusyOff::process(Wash *wash) {
	if(GetSysTime() > wash->getTimer() + wash->afterPowerSwitchDelayMs) {
		changeState(wash, &WashOffState);
	}
}

auto WashBusyOff::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Wash::states::off);
}

void WashBusyOn::process(Wash *wash) {
	if(GetSysTime() > wash->getTimer() + wash->afterPowerSwitchDelayMs) {
		changeState(wash, &WashOnState);
	}
}

auto WashBusyOn::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Wash::states::on);
}

}//namespace washer

}//namespace tasks
