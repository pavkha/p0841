#include "app.h"
#include "../tasks/can/can_rx_irq_handler.h"
#include "../tasks/control/stereo_pair_controller.h"

namespace app {

void Board0841::Initialize() {
	MessageQueue<CanRxMsg> *canRxMsgQueue =
			new MessageQueue<CanRxMsg>(
					tasks::can::canRxQueueLength);

	TaskIrq::Add(new tasks::can::Can1RxIrqHandler(*canRxMsgQueue),
			CAN1_IRQn,
			Task::PriorityRealtime,
			Task::ModePrivileged,
			0x100);

	Task::Add(new tasks::control::StereoPairController(*canRxMsgQueue),
			Task::PriorityNormal,
			0x200);
}

auto GetSysTime() -> std::uint32_t {
	return Sch().GetTickCount();
}

}//namespace app
