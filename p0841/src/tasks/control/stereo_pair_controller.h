#pragma once

#include "../computer/computer.h"
#include "../camera/camera.h"
#include "../curtain/curtain.h"
#include "../washer/washer.h"
#include "../can/can_rx_irq_handler.h"
#include "../can/can_protocol_u9.h"
#include "../can/can_protocol_0841.h"
#include "macs_timer.hpp"
#include <cstdint>

namespace tasks {

namespace control {

class StereoPairController : public Task {
public:
	explicit StereoPairController(MessageQueue<CanRxMsg>&);

private:
	virtual void Execute() override;

private:
	bool receiveCanMsg();
	void commandsHandler();
	void sendTelemetry(std::uint8_t);

	void makeTelemetryMsg(std::uint8_t, CanTxMsg&);
	auto makeOkAnswerMsg(CanRxMsg&) -> CanTxMsg;
	auto makeErrorAnswerMsg(CanRxMsg&) -> CanTxMsg;

	void computerCmdHandler();
	void cameraCmdHandler();
	void curtainCmdHandler();
	void washerCmdHandler();
	void telemetryCmdHandler();

private:
	computer::SBC sbc;
	camera::Cam cam;
	curtain::Curt curt;
	washer::Wash wash;

private:
	class TelemetrySendAction : public TimerAction {
	public:
		explicit TelemetrySendAction(StereoPairController *c) : controller{c} {

		}

	private:
		virtual void operator()() override {
			controller->sendTelemetry(controller->currentTelemetryPacket);
		}

	private:
		StereoPairController *controller{nullptr};
	};

	Timer telemtrySendTimer;
	friend class TelemetrySendAction;
	TelemetrySendAction *telemetryAction{nullptr};

private:
	std::uint8_t currentTelemetryPacket{
		tasks::can::protocol_0841::Telemetry::packetZero};

private:
	MessageQueue<CanRxMsg> &canRxMsgQueue;
	CanRxMsg currentRxMsg;
	bool currentRxMsgError{false};
};

}//namespace control

}//namespace tasks
