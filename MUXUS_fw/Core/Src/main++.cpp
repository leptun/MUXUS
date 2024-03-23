#include "pins.hpp"

#include "main.h"
#include "cmsis_os.h"
#include "bootapp.h"

////std libs
//#include <stdio.h>
//#include <algorithm>

//libs
#include "USB2504A/USB2504A.hpp"
#include "TS3USBCA4/TS3USBCA4.hpp"

I2C_Wrapper i2c_bus(&hi2c1, "i2c_bus");
USB2504A hub(0x58, pins::HUB.RST, pins::HUB.IFP_VBUS, &i2c_bus);
TS3USBCA4 mux(0xBA, &i2c_bus);

constexpr TS3USBCA4::Channels upstreams[] {
	TS3USBCA4::Channels::DISABLED,
	TS3USBCA4::Channels::LnB,
	TS3USBCA4::Channels::MICGND,
	TS3USBCA4::Channels::LnA,
	TS3USBCA4::Channels::LnC,
};

uint8_t activeUpstream;
constexpr uint8_t defaultUpstream = 2;

constexpr USB2504A::Config hubConfig = {
	0x0424,
	0x2504,
	0x10A1,
	0xDB,
	0x18,
	0x02,
	0x00,
	0x00,
	0x00,
	0x64,
	0x00,
	0x64,
	0x32,
};

void switchToUpstream(uint8_t port) {
	activeUpstream = port;
	hub.reset();
	mux.setChannel(upstreams[activeUpstream], false);
	hub.writeConfig(hubConfig);
	hub.attach();

	for (uint8_t i = 0; i < (sizeof(pins::LEDs) / sizeof(pins::LEDs[0])); i++) {
		HAL_GPIO_WritePin(pins::LEDs[i].port, pins::LEDs[i].pin, ((i + 1) == activeUpstream) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
}

extern "C" void controlTask_main(void *argument) {
	osDelay(500);
	hub.init();
	mux.init();

	switchToUpstream(defaultUpstream);

	for(;;osDelay(1)) {
		if (HAL_GPIO_ReadPin(pins::USER.port, pins::USER.pin)) {
			uint32_t startTime = osKernelGetTickCount();
			osDelay(10); //debounce
			while (HAL_GPIO_ReadPin(pins::USER.port, pins::USER.pin))
				osDelay(1);
			uint32_t deltaTime = osKernelGetTickCount() - startTime;

			if (deltaTime < 5000) {
				activeUpstream++;
				if (activeUpstream >= (sizeof(upstreams) / sizeof(upstreams[0])))
					activeUpstream = 1;
				switchToUpstream(activeUpstream);
			}
			else {
				boot_app_magic = BOOT_APP_MAGIC;
				boot_app_flags = BOOT_APP_JMP_TO_SYSTEM_MEMORY;
				HAL_NVIC_SystemReset();
			}
		}
	}
}
