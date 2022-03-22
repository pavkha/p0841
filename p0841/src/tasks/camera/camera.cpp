#include "camera.h"
#include "../../app/app.h"

namespace tasks {

namespace camera {

using namespace app;

CamPowerOff CamPowerOffState;
CamPowerOn CamPowerOnState;
CamBusyPowerOff CamBusyAfterPowerOffState;
CamBusyPowerOn CamBusyAfterPowerOnState;

Cam::Cam() {
	PinReset(powerPort, powerPin);
	changeState(&CamPowerOffState);
}

bool Cam::powerOn() {
	return state->powerOn(this);
}

bool Cam::powerOff() {
	return state->powerOff(this);
}

void Cam::forcePowerOff() {
	PinReset(powerPort, powerPin);
	changeState(&CamBusyAfterPowerOffState);
}

ReadPinType Cam::checkPowerOn() {
	return state->checkPowerOn(this);
}

void Cam::process() {
	state->process(this);
}

void Cam::changeState(CamState *s) {
	state = s;
}

auto Cam::getState() -> std::uint8_t {
	return state->getState();
}

void CamState::changeState(Cam *cam, CamState *s) {
	cam->changeState(s);
}

bool CamPowerOff::powerOn(Cam *cam) {
	PinSet(cam->powerPort, cam->powerPin);
	cam->setTimer(GetSysTime());
	changeState(cam, &CamBusyAfterPowerOnState);
	return true;
}

auto CamPowerOff::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Cam::states::powerOff);
}

bool CamPowerOn::powerOff(Cam *cam) {
	PinReset(cam->powerPort, cam->powerPin);
	cam->setTimer(GetSysTime());
	changeState(cam, &CamBusyAfterPowerOffState);
	return true;
}

auto CamPowerOn::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Cam::states::powerOn);
}

void CamBusyPowerOff::process(Cam *cam) {
	if(GetSysTime() > cam->getTimer() + cam->afterPowerSwitchDelayMs) {
		changeState(cam, &CamPowerOffState);
	}
}

auto CamBusyPowerOff::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Cam::states::powerOff);
}

void CamBusyPowerOn::process(Cam *cam) {
	if(GetSysTime() > cam->getTimer() + cam->afterPowerSwitchDelayMs) {
		changeState(cam, &CamPowerOnState);
	}
}

auto CamBusyPowerOn::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Cam::states::powerOn);
}

}//namespace camera

}//namespace tasks
