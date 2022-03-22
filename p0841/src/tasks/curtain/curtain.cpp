#include "curtain.h"
#include "../../app/app.h"

namespace tasks {

namespace curtain {

using namespace app;

CurtClose CurtCloseState;
CurtOpen CurtOpenState;
CurtStop CurtStopState;
CurtMoveOpen CurtMoveOpenState;
CurtMoveClose CurtMoveCloseState;

Curt::Curt() {
	PinReset(onPort, onPin);
	changeState(&CurtStopState);

	if(isOpen()) {
		changeState(&CurtOpenState);
	} else if(isClosed()) {
		changeState(&CurtCloseState);
	}
}

bool Curt::open() {
	return state->open(this);
}

bool Curt::close() {
	return state->close(this);
}

bool Curt::stop() {
	return state->stop(this);
}

ReadPinType Curt::checkOn() {
	return state->checkOn(this);
}

bool Curt::isOpen() {
	return ReadPin(feedbackOpenPort, feedbackOpenPin) ==
			GPInStates::pinReset; //TODO дребезг контактов, помехи
}

bool Curt::isClosed() {
	return ReadPin(feedbackClosePort, feedbackClosePin) ==
			GPInStates::pinReset; //TODO дребезг контактов, помехи
}

void Curt::process() {
	state->process(this);
}

void Curt::changeState(CurtState *s) {
	state = s;
}

auto Curt::getState() -> std::uint8_t {
	return state->getState();
}

void CurtState::changeState(Curt *curt, CurtState *s) {
	curt->changeState(s);
}

bool CurtClose::open(Curt *curt) {
	PinSet(curt->onPort, curt->onPin);
	changeState(curt, &CurtMoveOpenState);
	return true;
}

auto CurtClose::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Curt::states::close);
}

bool CurtOpen::close(Curt *curt) {
	PinSet(curt->onPort, curt->onPin);
	changeState(curt, &CurtMoveCloseState);
	return true;
}

auto CurtOpen::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Curt::states::open);
}

bool CurtStop::open(Curt *curt) {
	PinSet(curt->onPort, curt->onPin);
	changeState(curt, &CurtMoveOpenState);
	return true;
}

bool CurtStop::close(Curt *curt) {
	PinSet(curt->onPort, curt->onPin);
	changeState(curt, &CurtMoveCloseState);
	return true;
}

auto CurtStop::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Curt::states::stop);
}

bool CurtMove::close(Curt *curt) {
	changeState(curt, &CurtMoveCloseState);
	return true;
}

bool CurtMove::open(Curt *curt) {
	changeState(curt, &CurtMoveOpenState);
	return true;
}

bool CurtMove::stop(Curt *curt) {
	PinReset(curt->onPort, curt->onPin);
	changeState(curt, &CurtStopState);
	return true;
}

auto CurtMove::getState() -> std::uint8_t {
	return static_cast<std::uint8_t>(Curt::states::move);
}

void CurtMoveOpen::process(Curt *curt) {
	if(curt->isOpen()) {
		PinReset(curt->onPort, curt->onPin);
		changeState(curt, &CurtOpenState);
	}
}

void CurtMoveClose::process(Curt *curt) {
	if(curt->isClosed()) {
		PinReset(curt->onPort, curt->onPin);
		changeState(curt, &CurtCloseState);
	}
}

}//namespace curtain

}//namespace tasks
