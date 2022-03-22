#pragma once

#include "gpio.h"
#include "macs_rtos.hpp"

namespace tasks {

namespace computer {

class SBCState;

class SBC {
public:
	SBC();
	SBC(SBC const&) = delete;
	SBC(SBC&&) = delete;
	SBC& operator=(SBC const&) = delete;
	SBC& operator=(SBC&&) = delete;

	static auto const afterPowerOffDelayMs{200U};
	static auto const afterPowerOnDelayMs{200U};
	static auto const resetPulseTimeMs{100U};

	PortType *const powerPort{SBCPowerPort};
	PinType const powerPin{SBCPowerPin};

	PortType *const feedbackPowerPort{SBCFeedbackPowerPort};
	PinType const feedbackPowerPin{SBCFeedbackPowerPin};

	PortType *const resetPort{SBCResetPort};
	PinType const resetPin{SBCResetPin};

	enum class states : std::uint8_t {
		powerOn = 1,
		powerOff
	};

public:
	bool powerOn();
	bool powerOff();
	bool resetStart();
	bool resetRelease();
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
	friend class SBCState;
	void changeState(SBCState*);
	SBCState *state{nullptr};

private:
	std::uint32_t timer{0};
};

class SBCState {
public:
	virtual bool powerOn(SBC*) {
		return false;
	}

	virtual bool powerOff(SBC*) {
		return false;
	}

	virtual bool resetStart(SBC*) {
		return false;
	}

	virtual bool resetRelease(SBC*) {
		return false;
	}

	virtual ReadPinType checkPowerOn(SBC *sbc) {
		return ReadPin(sbc->feedbackPowerPort, sbc->feedbackPowerPin);
	}

	virtual void process(SBC*) {

	}

	virtual auto getState() -> std::uint8_t = 0;

protected:
	void changeState(SBC*, SBCState*);
};

class SBCPowerOff : public SBCState {
public:
	virtual bool powerOn(SBC*) override;
	virtual std::uint8_t getState() override;
};

class SBCPowerOn : public SBCState {
public:
	virtual bool resetStart(SBC*) override;
	virtual bool powerOff(SBC*) override;
	virtual std::uint8_t getState() override;
};

class SBCBusyPowerOff : public SBCState {
public:
	virtual void process(SBC*) override;
	virtual std::uint8_t getState() override;
};

class SBCBusyPowerOn : public SBCState {
public:
	virtual void process(SBC*) override;
	virtual std::uint8_t getState() override;
};

class SBCBusyReset : public SBCState {
public:
	virtual void process(SBC*) override;
	virtual std::uint8_t getState() override;
};

}//namespace sbc

}//namespace tasks
