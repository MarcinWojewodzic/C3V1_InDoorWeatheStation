################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/fonts/Tnr10ppt.c \
../Core/Inc/fonts/Tnr12ppt.c \
../Core/Inc/fonts/Tnr16ppt.c \
../Core/Inc/fonts/Tnr22ppt.c \
../Core/Inc/fonts/Tnr28ppt.c 

OBJS += \
./Core/Inc/fonts/Tnr10ppt.o \
./Core/Inc/fonts/Tnr12ppt.o \
./Core/Inc/fonts/Tnr16ppt.o \
./Core/Inc/fonts/Tnr22ppt.o \
./Core/Inc/fonts/Tnr28ppt.o 

C_DEPS += \
./Core/Inc/fonts/Tnr10ppt.d \
./Core/Inc/fonts/Tnr12ppt.d \
./Core/Inc/fonts/Tnr16ppt.d \
./Core/Inc/fonts/Tnr22ppt.d \
./Core/Inc/fonts/Tnr28ppt.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/fonts/%.o Core/Inc/fonts/%.su: ../Core/Inc/fonts/%.c Core/Inc/fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSER_VECT_TAB_ADDRESS -DUSE_HAL_DRIVER -DSTM32L4P5xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-fonts

clean-Core-2f-Inc-2f-fonts:
	-$(RM) ./Core/Inc/fonts/Tnr10ppt.d ./Core/Inc/fonts/Tnr10ppt.o ./Core/Inc/fonts/Tnr10ppt.su ./Core/Inc/fonts/Tnr12ppt.d ./Core/Inc/fonts/Tnr12ppt.o ./Core/Inc/fonts/Tnr12ppt.su ./Core/Inc/fonts/Tnr16ppt.d ./Core/Inc/fonts/Tnr16ppt.o ./Core/Inc/fonts/Tnr16ppt.su ./Core/Inc/fonts/Tnr22ppt.d ./Core/Inc/fonts/Tnr22ppt.o ./Core/Inc/fonts/Tnr22ppt.su ./Core/Inc/fonts/Tnr28ppt.d ./Core/Inc/fonts/Tnr28ppt.o ./Core/Inc/fonts/Tnr28ppt.su

.PHONY: clean-Core-2f-Inc-2f-fonts

