#pragma once

#include "can.h"
#include "macs_rtos.hpp"
#include <cstdint>

namespace tasks {

namespace can {

enum {
	canRxQueueLength = 5
};

class Can1RxIrqHandler : public TaskIrq {
public:
	Can1RxIrqHandler(MessageQueue<CanRxMsg> &q) :
		TaskIrq{"CanRxIrqHandler"},
		rxMsgQueues{q} {}

private:
	virtual void IrqHandler() override;

private:
	MessageQueue<CanRxMsg> &rxMsgQueues;
};

}//namespace can

}//namespace tasks
