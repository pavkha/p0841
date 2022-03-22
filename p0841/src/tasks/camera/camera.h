#pragma once

#include "gpio.h"
#include "macs_rtos.hpp"

namespace tasks {

namespace camera {

class CamState;

class Cam {
public:
	Cam();
	Cam(Cam const&) = delete;
	Cam(Cam&&) = delete;
	Cam& operator=(Cam const&) = delete;
	Cam& operator=(Cam&&) = delete;

	static auto const afterPowerSwitchDelayMs{200U};

	PortType *const powerPort{CamPowerPort};
	PinType const powerPin{CamPowerPin};

	PortType *const feedbackPowerPort{CamFeedbackPowerPort};
	PinType const feedbackPowerPin{CamFeedbackPowerPin};

	enum class states : std::uint8_t {
		powerOn = 1,
		powerOff
	};

public:
	bool powerOn();
	bool powerOff();
	ReadPinType checkPowerOn();
	void process();

	void forcePowerOff();

	auto getTimer() {
		return timer;
	}

	void setTimer(uint32_t time) {
		timer = time;
	}

	auto getState() -> std::uint8_t;

private:
	friend class CamState;
	void changeState(CamState*);
	CamState *state{nullptr};

private:
	std::uint32_t timer{0};
};

class CamState {
public:
	virtual bool powerOn(Cam*) {
		return false;
	}

	virtual bool powerOff(Cam*) {
		return false;
	}

	virtual ReadPinType checkPowerOn(Cam *Cam) {
		return ReadPin(Cam->feedbackPowerPort, Cam->feedbackPowerPin);
	}

	virtual void process(Cam*) {

	}

	virtual auto getState() -> std::uint8_t = 0;

protected:
	void changeState(Cam*, CamState*);
};

class CamPowerOff : public CamState {
public:
	virtual bool powerOn(Cam*) override;
	virtual std::uint8_t getState() override;
};

class CamPowerOn : public CamState {
public:
	virtual bool powerOff(Cam*) override;
	virtual std::uint8_t getState() override;
};

class CamBusyPowerOff : public CamState {
public:
	virtual void process(Cam*) override;
	virtual std::uint8_t getState() override;
};

class CamBusyPowerOn : public CamState {
public:
	virtual void process(Cam*) override;
	virtual std::uint8_t getState() override;
};

}//namespace camera

}//namespace tasks
