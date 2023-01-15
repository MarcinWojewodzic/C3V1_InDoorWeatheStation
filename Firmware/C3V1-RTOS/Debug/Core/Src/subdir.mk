################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GFX_BW.c \
../Core/Src/GFX_Lepsze.c \
../Core/Src/HC12.c \
../Core/Src/Menu.c \
../Core/Src/WS2812b.c \
../Core/Src/bme280_spi.c \
../Core/Src/crc.c \
../Core/Src/dma.c \
../Core/Src/e_papier.c \
../Core/Src/flash_spi.c \
../Core/Src/fram.c \
../Core/Src/freertos.c \
../Core/Src/gpio.c \
../Core/Src/iwdg.c \
../Core/Src/main.c \
../Core/Src/printf.c \
../Core/Src/rfp.c \
../Core/Src/rtc.c \
../Core/Src/spi.c \
../Core/Src/ssd1306_spi.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_hal_timebase_tim.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/GFX_BW.o \
./Core/Src/GFX_Lepsze.o \
./Core/Src/HC12.o \
./Core/Src/Menu.o \
./Core/Src/WS2812b.o \
./Core/Src/bme280_spi.o \
./Core/Src/crc.o \
./Core/Src/dma.o \
./Core/Src/e_papier.o \
./Core/Src/flash_spi.o \
./Core/Src/fram.o \
./Core/Src/freertos.o \
./Core/Src/gpio.o \
./Core/Src/iwdg.o \
./Core/Src/main.o \
./Core/Src/printf.o \
./Core/Src/rfp.o \
./Core/Src/rtc.o \
./Core/Src/spi.o \
./Core/Src/ssd1306_spi.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_hal_timebase_tim.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/GFX_BW.d \
./Core/Src/GFX_Lepsze.d \
./Core/Src/HC12.d \
./Core/Src/Menu.d \
./Core/Src/WS2812b.d \
./Core/Src/bme280_spi.d \
./Core/Src/crc.d \
./Core/Src/dma.d \
./Core/Src/e_papier.d \
./Core/Src/flash_spi.d \
./Core/Src/fram.d \
./Core/Src/freertos.d \
./Core/Src/gpio.d \
./Core/Src/iwdg.d \
./Core/Src/main.d \
./Core/Src/printf.d \
./Core/Src/rfp.d \
./Core/Src/rtc.d \
./Core/Src/spi.d \
./Core/Src/ssd1306_spi.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_hal_timebase_tim.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSER_VECT_TAB_ADDRESS -DUSE_HAL_DRIVER -DSTM32L4P5xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/GFX_BW.d ./Core/Src/GFX_BW.o ./Core/Src/GFX_BW.su ./Core/Src/GFX_Lepsze.d ./Core/Src/GFX_Lepsze.o ./Core/Src/GFX_Lepsze.su ./Core/Src/HC12.d ./Core/Src/HC12.o ./Core/Src/HC12.su ./Core/Src/Menu.d ./Core/Src/Menu.o ./Core/Src/Menu.su ./Core/Src/WS2812b.d ./Core/Src/WS2812b.o ./Core/Src/WS2812b.su ./Core/Src/bme280_spi.d ./Core/Src/bme280_spi.o ./Core/Src/bme280_spi.su ./Core/Src/crc.d ./Core/Src/crc.o ./Core/Src/crc.su ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/e_papier.d ./Core/Src/e_papier.o ./Core/Src/e_papier.su ./Core/Src/flash_spi.d ./Core/Src/flash_spi.o ./Core/Src/flash_spi.su ./Core/Src/fram.d ./Core/Src/fram.o ./Core/Src/fram.su ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/iwdg.d ./Core/Src/iwdg.o ./Core/Src/iwdg.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/printf.d ./Core/Src/printf.o ./Core/Src/printf.su ./Core/Src/rfp.d ./Core/Src/rfp.o ./Core/Src/rfp.su ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/ssd1306_spi.d ./Core/Src/ssd1306_spi.o ./Core/Src/ssd1306_spi.su ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_hal_timebase_tim.d ./Core/Src/stm32l4xx_hal_timebase_tim.o ./Core/Src/stm32l4xx_hal_timebase_tim.su ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

