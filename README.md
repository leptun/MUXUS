# MUXUS
This repository contains the MUXUS project, a USB 2.0 switch with 4 inputs (3xB + 1C_PD) and 4 outputs (3xA + STM32)

![image](https://user-images.githubusercontent.com/17808203/142241444-256b472b-77db-441e-9114-9d7403cddd42.png)
![image](https://user-images.githubusercontent.com/17808203/142241922-750ac954-f364-440b-8392-d519324ca072.png)
![image](https://user-images.githubusercontent.com/17808203/142241771-7259bca0-9416-40ce-a2e0-f9933713268e.png)

The project is split into multiple logical parts, all interconected on a PCB.
1. Upstream USB ports
2. USB mux
3. USB hub
4. Downstream USB ports
5. STM32 microcontroller
6. USB PD sink
7. STM32 programming
8. 3.3V rail Buck converter

## 1. Upstream USB ports
There are 4 upstream capable USB ports (UFP1-UFP4): 3x USB-B and 1xUSB-C

The USB-B ports (UFP2-4) are protected by `USBLC6-2SC6` ICs (VBUS, D+/-).

The USB-C port (UFP1) is protected by a `USBLC6-4SC6` IC (VBUS, D+/-, CC1/2).

## 2. USB mux
All Upstream facing ports (UFP) are connected to the USB mux IC (`TS3USBCA420RSVR`).
The mux output is connected to an internal usb bus called IFP (inner facing port).
Even though this IC is designed to multiplex SBU pins on USB-C ports, its bandwidth is high enough for USB 2.0 signals.
This IC is connected to the MCU via a 400KHz I2C bus. All control is done via I2C.

## 3. USB hub
The IFP bus is split into multiple downstream ports (DFP1-DFP4) using a USB 2.0 HS hub (`USB2504A-JT`).
The hub has an I2C interface as well and it is connected to the MCU via the same 400KHz I2C bus.
Most of the control is done via the I2C bus, but there are also a few extra pins required:
- HUB_RESET. It is an active low signal with an external pull-up resistor. It is needed because the I2C bus gets disabled by the hub when the Attach command is executed by the hub.
- IFP_VBUS. The MCU emulates the VBUS connection of the upstream ports since the MUX IC doesn't do that on its own.

Each DFP has a pair of status LEDs (green and amber):
- The green LED indicates if the device is connected and powered. It turns off when the device is suspended or when it is disconnected.
- The amber LED indicates a fault in the port. This seems to only happen when the VBUS is shorted to GND.

## 4. Downstream USB ports
There are 4 downstream capable ports (DFP1-4), however only 3 have connectors (DFP2-4).

DFP1 is connected directly to the mcu and, as a result, no power circuitry is required for this one.

DFP2-4 are usb-A ports. Each port is protected by `USBLC6-2SC6` ICs (VBUS, D+/-) and VBUS is switched using `AP2151W` ICs.

## 5. STM32 microcontroller
The MCU is a `STM32G441KBU6`.
There are a few reasons why this mcu was chosen, but the main restriction that made me choose this was the IC shortage.

The requirements for the mcu were as follows:
- Enough pins for controlling the board. A QFN32 IC was barely enough for this task. A 48 pin IC would have been much easier to use.
- 4 ADC inputs
- An I2C bus
- USB C PD capabilities
- Some kind of debug interface

Even though these requirements could have been fulfilled by much simpler MCUs, I chose this one as I wanted to learn how to use this MCU family.

## 6. USB PD sink
I wanted this hub to be fully featured, so I made sure that all downstream ports are capable of delivering the full USB 2.0 power specification.
As a result, 1.5A + whatever else consumes power on the board is required. The only method I could achieve this is by either using an external 5V power supply, or by using a USBC-PD capable charger. I chose the latter.
With USBC-PD, the board can communicate with the charger directly and request up to 3A at 5V. I configured it to only request 2A.
The UCPD stack provided by ST is surprisingly heavy, eating up ~70KB of flash. Luckily, it leaves enough flash for the rest of the code.

# 7. STM32 programming
The MCU can be programmed in multiple ways:
- The STDC14 connector is the main method I used. It has a SWD interface, SWO trace support and a UART interface to the programmer, making development extremely easy, especially for the STM32CubeMonitor-UCPD tool. Even though the connector is a bit more expensive than I'd like, it is a single connector debug interface that has everything I'd ever need and it is compatible with the STLINK-V3 programmmer. The connector Part number is `FTSH-107-01-L-DV-K-A`
- A pair of 2.54mm pin headers. The same signals are exposed just like in the STDC14 connector, but in a less pleasant pinout.
- USB DFU firmware upload. If the CYCLE/BOOT0 button is held pressed while the reset button is released, the board enters the system memory boot option, through which code can be uploaded. Due to the way the hub is controlled, resetting the mcu will keep the last mux connection active, so code can be flashed through the mux and hub. There is a small design issue however (more on that in the Known issues section).

# 8. 3.3V rail Buck converter
This buck converter (`AOZ1212AI`) powers all the 3.3V ICs.
It is waaaaay overkill for this project, but I already had the ICs and wanted to implement them.
A small LDO would have been much better suited and it might even have been more stable.

# Known issues
- rev1
  - The ICs are painful to solder. I shouldn't have chosen QFN packages. Lesson learned.
  - The clock circuitry for the hub is wrong. 32pF capacitors should have been used and there should also be a 1M resistor between the crystal leads.
  - The USB signal routing is *horrible*. I'm surprised it works at all. I should improve that aspect if I ever make another revision (:doubt:).
  - There is a pinout conflict between the UCPD dead battery signals and the system memory DFU bootloader. When entering the bootloader, the DBCC1 gets assigned as USART1_TX, disabling the dead battery signal. As a result, power gets cut. This issue can be avoided if the usb C cable is connected with one of the ends reversed, so it works only 50% of the time. There are two solutions for a future revision of the board: I could use a protection IC with integrated DB signals such as the one used in X-NUCLEO-USBPDM1, or I could just use an MCU which doesn't have this pinout conflict (such as STM32G0B1KET6N or similar).
  - The component placement could be better and the board could be smaller.
