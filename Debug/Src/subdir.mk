################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/adc_config.c \
../Src/dma_config.c \
../Src/gpio_config.c \
../Src/main.c \
../Src/stm32f0xx_hal_msp.c \
../Src/stm32f0xx_it.c \
../Src/system_stm32f0xx.c \
../Src/systemclock_config.c 

OBJS += \
./Src/adc.o \
./Src/adc_config.o \
./Src/dma_config.o \
./Src/gpio_config.o \
./Src/main.o \
./Src/stm32f0xx_hal_msp.o \
./Src/stm32f0xx_it.o \
./Src/system_stm32f0xx.o \
./Src/systemclock_config.o 

C_DEPS += \
./Src/adc.d \
./Src/adc_config.d \
./Src/dma_config.d \
./Src/gpio_config.d \
./Src/main.d \
./Src/stm32f0xx_hal_msp.d \
./Src/stm32f0xx_it.d \
./Src/system_stm32f0xx.d \
./Src/systemclock_config.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F030x6 -I"C:/Users/Berkant/Desktop/systemworkbench/BMSv2/Inc" -I"C:/Users/Berkant/Desktop/systemworkbench/BMSv2/Drivers/STM32F0xx_HAL_Driver/Inc" -I"C:/Users/Berkant/Desktop/systemworkbench/BMSv2/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Berkant/Desktop/systemworkbench/BMSv2/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"C:/Users/Berkant/Desktop/systemworkbench/BMSv2/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


