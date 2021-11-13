#pragma once
#include "main.h"
#include "stm++/IO.hpp"

namespace pins {
	static constexpr struct {
		IO RST = {HUB_RESET_GPIO_Port, HUB_RESET_Pin};
		IO IFP_VBUS = {IFP_VBUS_GPIO_Port, IFP_VBUS_Pin};
	} HUB;
	static constexpr IO USER = {BOOT0_GPIO_Port, BOOT0_Pin};
	static constexpr IO LEDs[] = {
			{LED1_GPIO_Port, LED1_Pin},
			{LED2_GPIO_Port, LED2_Pin},
			{LED3_GPIO_Port, LED3_Pin},
			{LED4_GPIO_Port, LED4_Pin},
	};
};
