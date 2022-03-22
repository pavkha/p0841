#pragma once

#include "gpio.h"
#include "macs_rtos.hpp"

namespace tasks {

namespace washer {

class WashState;

class Wash {
public:
	Wash();
	Wash(Wash const&) = delete;
	Wash(Wash&&) = delete;
	Wash& operator=(Wash const&) = delete;
	Wash& operator=(Wash&&) = delete;

	static auto const afterPowerSwitchDelayMs{200U};

	PortType *const onPort{WashOnPort};
	PinType const onPin{WashOnPin};

	PortType *const feedbackOnPort{WashFeedbackOnPort};
	PinType const feedbackOnPin{WashFeedbackOnPin};

	enum class states : std::uint8_t {
		on = 1,
		off
	};

public:
	bool on();
	bool off();
	ReadPinType checkOn();
	void process();

	void forceOff();

	auto getTimer() {
		return timer;
	}

	void setTimer(uint32_t time) {
		timer = time;
	}

	auto getState() -> std::uint8_t;

private:
	friend class WashState;
	void changeState(WashState*);
	WashState *state{nullptr};

private:
	std::uint32_t timer{0};
};

class WashState {
public:
	virtual bool on(Wash*) {
		return false;
	}

	virtual bool off(Wash*) {
		return false;
	}

	virtual ReadPinType checkOn(Wash *wash) {
		return ReadPin(wash->feedbackOnPort, wash->feedbackOnPin);
	}

	virtual void process(Wash*) {

	}

	virtual auto getState() -> std::uint8_t = 0;

protected:
	void changeState(Wash*, WashState*);
};

class WashOff : public WashState {
public:
	virtual bool on(Wash*) override;
	virtual std::uint8_t getState() override;
};

class WashOn : public WashState {
public:
	virtual bool off(Wash*) override;
	virtual std::uint8_t getState() override;
};

class WashBusyOff : public WashState {
public:
	virtual void process(Wash*) override;
	virtual std::uint8_t getState() override;
};

class WashBusyOn : public WashState {
public:
	virtual void process(Wash*) override;
	virtual std::uint8_t getState() override;
};

}//namespace washer

}//namespace tasks
