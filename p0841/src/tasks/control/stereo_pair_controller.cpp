#include "stereo_pair_controller.h"

namespace tasks {

namespace control {

using namespace tasks::can::protocol_u9;
using namespace tasks::can::protocol_0841;

StereoPairController::StereoPairController(MessageQueue<CAN_RxMsgTypeDef>& p) :
				Task{"StereoPairController"},
				canRxMsgQueue{p} {
	telemetryAction = new TelemetrySendAction{this};
}

void StereoPairController::Execute() {
	while(true) {
		if(receiveCanMsg()) {
			commandsHandler();
		}

		sbc.process();
		cam.process();
		curt.process();
		wash.process();
	}
}

bool StereoPairController::receiveCanMsg() {
	if(canRxMsgQueue.Count() != 0) {
		return canRxMsgQueue.Pop(currentRxMsg, 0) == ResultOk;
	} else {
		return false;
	}
}

void StereoPairController::commandsHandler() {
	currentRxMsgError = false;

	auto cmd = reinterpret_cast<std::uint8_t*>(
			&currentRxMsg.Data[0])[0];

	if(cmd == Commands::computerCmd) {
		computerCmdHandler();
	} else if(cmd == Commands::cameraCmd) {
		cameraCmdHandler();
	} else if(cmd == Commands::curtainCmd) {
		curtainCmdHandler();
	} else if(cmd == Commands::washerCmd) {
		washerCmdHandler();
	} else if(cmd == Commands::telemetryCmd) {
		telemetryCmdHandler();
	} else {
		currentRxMsgError = true;
	}

	if(currentRxMsgError) {
		auto txMsg = makeErrorAnswerMsg(currentRxMsg);

		CanTransmit(Can1Instance, errorMailBox, &txMsg);
	} else {
		auto txMsg = makeOkAnswerMsg(currentRxMsg);

		CanTransmit(Can1Instance, okMailBox, &txMsg);
	}
}

void StereoPairController::sendTelemetry(std::uint8_t packet) {
	CanTxMsg txMsg{};

	makeTelemetryMsg(packet, txMsg);
	CanTransmit(Can1Instance, telemetryMailBox, &txMsg);
}

void StereoPairController::makeTelemetryMsg(std::uint8_t packet,
		CanTxMsg &txMsg) {
	if(packet == Telemetry::packetZero) {
		auto msgId = can_msg_id::status_telemetry | can_device_id::spair;

		txMsg.ID = CAN_STDID_TO_EXTID(msgId);
		txMsg.DLC = Telemetry::packetZeroDlc;

		reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[0] =
				Telemetry::packetZeroCode;
		reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[1] = sbc.getState();
		reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[2] = cam.getState();
		reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[3] = curt.getState();

		reinterpret_cast<std::uint8_t*>(&txMsg.Data[1])[0] = wash.getState();
		reinterpret_cast<std::uint8_t*>(&txMsg.Data[1])[1] = 0xA5;
		reinterpret_cast<std::uint8_t*>(&txMsg.Data[1])[2] = 0xA5;
		reinterpret_cast<std::uint8_t*>(&txMsg.Data[1])[3] = 0xA5;
	}
}

auto StereoPairController::makeOkAnswerMsg(CanRxMsg &rxMsg) -> CanTxMsg {
	auto msgId = CAN_EXTID_TO_STDID(rxMsg.Rx_Header.ID);

	msgId |= can_msg_id::events_telemetry;
	rxMsg.Rx_Header.ID = CAN_STDID_TO_EXTID(msgId);

	CanTxMsg txMsg{};

	txMsg.ID = rxMsg.Rx_Header.ID;
	txMsg.DLC = rxMsg.Rx_Header.DLC;
	txMsg.Data[0] = rxMsg.Data[0];
	txMsg.Data[1] = rxMsg.Data[1];

	return txMsg;
}

auto StereoPairController::makeErrorAnswerMsg(CanRxMsg &rxMsg) -> CanTxMsg {
	auto msgId = CAN_EXTID_TO_STDID(rxMsg.Rx_Header.ID);

	msgId |= can_msg_id::events_telemetry;
	rxMsg.Rx_Header.ID = CAN_STDID_TO_EXTID(msgId);

	CanTxMsg txMsg{};

	txMsg.ID = rxMsg.Rx_Header.ID;
	txMsg.DLC = rxMsg.Rx_Header.DLC;

	reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[0] = ErrorCode::common;
	reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[1] =
			reinterpret_cast<std::uint8_t*>(&rxMsg.Data[0])[0];
	reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[2] = 0xA5;
	reinterpret_cast<std::uint8_t*>(&txMsg.Data[0])[3] = 0xA5;

	txMsg.Data[1] = 0xA5A5A5A5;

	return txMsg;
}

void StereoPairController::computerCmdHandler() {
	auto cmd = reinterpret_cast<std::uint8_t*>(&currentRxMsg.Data[0])[1];

	if(cmd == Commands::compPowerOn) {
		if(!sbc.powerOn()) {
			currentRxMsgError = true;
		}
	} else if(cmd == Commands::compPowerOff) {
		if(!sbc.powerOff()) {
			currentRxMsgError = true;
		}
	} else if(cmd == Commands::compReset) {
		if(!sbc.resetStart()) {
			currentRxMsgError = true;
		}
	} else {
		currentRxMsgError = true;
	}
}

void StereoPairController::cameraCmdHandler() {
	auto cmd = reinterpret_cast<std::uint8_t*>(&currentRxMsg.Data[0])[1];

	if(cmd == Commands::camPowerOn) {
		if(!cam.powerOn()) {
			currentRxMsgError = true;
		}
	} else if(cmd == Commands::camPowerOff) {
		if(!cam.powerOff()) {
			currentRxMsgError = true;
		}
	} else {
		currentRxMsgError = true;
	}
}
void StereoPairController::curtainCmdHandler() {
	auto cmd = reinterpret_cast<std::uint8_t*>(&currentRxMsg.Data[0])[1];

	if(cmd == Commands::curtOpen) {
		if(!curt.open()) {
			currentRxMsgError = true;
		}
	} else if(cmd == Commands::curtClose) {
		if(!curt.close()) {
			currentRxMsgError = true;
		}
	} else if(cmd == Commands::curtStop) {
		if(!curt.stop()) {
			currentRxMsgError = true;
		}
	} else {
		currentRxMsgError = true;
	}
}

void StereoPairController::washerCmdHandler() {
	auto cmd = reinterpret_cast<std::uint8_t*>(&currentRxMsg.Data[0])[1];

	if(cmd == Commands::washOn) {
		if(!wash.on()) {
			currentRxMsgError = true;
		}
	} else if(cmd == Commands::washOff) {
		if(!wash.off()) {
			currentRxMsgError = true;
		}
	}  else {
		currentRxMsgError = true;
	}
}

void StereoPairController::telemetryCmdHandler() {
	auto packet = reinterpret_cast<std::uint8_t*>(&currentRxMsg.Data[0])[1];

	if(packet >= Telemetry::packetCount) {
		currentRxMsgError = true;
	} else {
		auto mode = reinterpret_cast<std::uint8_t*>(&currentRxMsg.Data[0])[2];

		if(mode == Telemetry::once) {
			sendTelemetry(packet);
		} else if(mode == Telemetry::cyclic) {
			auto period = currentRxMsg.Data[1];

			if(period < 10) {
				period = 10;
			}
			currentTelemetryPacket = packet;
			telemtrySendTimer.Initialize(
					period, Timer::MeasureMode::Milliseconds, telemetryAction);
			telemtrySendTimer.Start();
		} else if(mode == Telemetry::stop) {
			telemtrySendTimer.Stop();
			auto txMsg = makeOkAnswerMsg(currentRxMsg);

			CanTransmit(Can1Instance, okMailBox, &txMsg);
		} else {
			currentRxMsgError = true;
		}
	}
}

}//namespace control

}//namespace tasks
