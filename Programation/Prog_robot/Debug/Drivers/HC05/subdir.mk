################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/HC05/tasks_HC05.c 

OBJS += \
./Drivers/HC05/tasks_HC05.o 

C_DEPS += \
./Drivers/HC05/tasks_HC05.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/HC05/%.o Drivers/HC05/%.su Drivers/HC05/%.cyclo: ../Drivers/HC05/%.c Drivers/HC05/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-HC05

clean-Drivers-2f-HC05:
	-$(RM) ./Drivers/HC05/tasks_HC05.cyclo ./Drivers/HC05/tasks_HC05.d ./Drivers/HC05/tasks_HC05.o ./Drivers/HC05/tasks_HC05.su

.PHONY: clean-Drivers-2f-HC05

