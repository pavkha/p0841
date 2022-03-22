#include "can_rx_irq_handler.h"

namespace tasks {

namespace can {

void Can1RxIrqHandler::IrqHandler() {
	CanRxMsg rxMsg;

	CAN_GetRawReceivedData(MDR_CAN1, cmdMailBox, &rxMsg);

	Result res = rxMsgQueues.Push(rxMsg, 0);
	if(res != Result::ResultOk) {

	}
}

}//namespace can

}//namespace tasks
