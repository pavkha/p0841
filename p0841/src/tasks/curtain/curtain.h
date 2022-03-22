#pragma once

#include "gpio.h"
#include "macs_rtos.hpp"

namespace tasks {

namespace curtain {

class CurtState;

class Curt {
public:
	Curt();
	Curt(Curt const&) = delete;
	Curt(Curt&&) = delete;
	Curt& operator=(Curt const&) = delete;
	Curt& operator=(Curt&&) = delete;

	PortType *const onPort{CurtOnPort};
	PinType const onPin{CurtOnPin};

	PortType *const feedbackOnPort{CurtFeedbackOnPort};
	PinType const feedbackOnPin{CurtFeedbackOnPin};

	PortType *const feedbackOpenPort{CurtFeedbackOpenPort};
	PinType const feedbackOpenPin{CurtFeedbackOpenPin};

	PortType *const feedbackClosePort{CurtFeedbackClosePort};
	PinType const feedbackClosePin{CurtFeedbackClosePin};

	enum class states : std::uint8_t {
		open = 1,
		close,
		stop,
		move
	};

public:
	bool open();
	bool close();
	bool stop();
	ReadPinType checkOn();
	bool isOpen();
	bool isClosed();
	void process();

	auto getTimer() {
		return timer;
	}

	void setTimer(uint32_t time) {
		timer = time;
	}

	auto getState() -> std::uint8_t;

private:
	friend class CurtState;
	void changeState(CurtState*);
	CurtState *state{nullptr};

private:
	std::uint32_t timer{0};
};

class CurtState {
public:
	virtual bool open(Curt*) {
		return true;
	}

	virtual bool close(Curt*) {
		return true;
	}

	virtual bool stop(Curt*) {
		return true;
	}

	virtual ReadPinType checkOn(Curt *curt) {
		return ReadPin(curt->feedbackOnPort, curt->feedbackOnPin);
	}

	virtual void process(Curt*) {

	}

	virtual auto getState() -> std::uint8_t = 0;

protected:
	void changeState(Curt*, CurtState*);
};

class CurtClose : public CurtState {
public:
	virtual bool open(Curt*) override;
	virtual auto getState() -> std::uint8_t override;
};

class CurtOpen : public CurtState {
public:
	virtual bool close(Curt*) override;
	virtual auto getState() -> std::uint8_t override;
};

class CurtStop : public CurtState {
public:
	virtual bool open(Curt*) override;
	virtual bool close(Curt*) override;
	virtual auto getState() -> std::uint8_t override;
};

class CurtMove : public CurtState {
public:
	virtual bool stop(Curt*) override;
	virtual bool open(Curt*) override;
	virtual bool close(Curt*) override;
	virtual auto getState() -> std::uint8_t override;
};

class CurtMoveOpen : public CurtMove {
public:
	virtual void process(Curt*) override;
};

class CurtMoveClose : public CurtMove {
public:
	virtual void process(Curt*) override;
};

}//namespace curtain

}//namespace tasks
