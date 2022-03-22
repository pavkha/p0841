#pragma once

#include "macs_rtos.hpp"
#include <cstdint>

namespace app {

class Board0841 : public Application {
private:
	virtual void Initialize() override;
};

auto GetSysTime() -> std::uint32_t;

}//namespace app
