################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USBPD/usbpd_dpm_core.c \
../USBPD/usbpd_dpm_user.c \
../USBPD/usbpd_pwr_if.c \
../USBPD/usbpd_pwr_user.c \
../USBPD/usbpd_vdm_user.c 

C_DEPS += \
./USBPD/usbpd_dpm_core.d \
./USBPD/usbpd_dpm_user.d \
./USBPD/usbpd_pwr_if.d \
./USBPD/usbpd_pwr_user.d \
./USBPD/usbpd_vdm_user.d 

OBJS += \
./USBPD/usbpd_dpm_core.o \
./USBPD/usbpd_dpm_user.o \
./USBPD/usbpd_pwr_if.o \
./USBPD/usbpd_pwr_user.o \
./USBPD/usbpd_vdm_user.o 


# Each subdirectory must supply rules for building sources it contributes
USBPD/%.o: ../USBPD/%.c USBPD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_RTOS -D_SNK -D_TRACE -D_GUI_INTERFACE -DUSBPDCORE_LIB_PD3_FULL -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USBPD/App -I../USBPD -I../Utilities/GUI_INTERFACE -I../Utilities/TRACER_EMB -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

