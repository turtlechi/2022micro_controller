################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/7seg_function.c \
../src/button_function.c \
../src/calculator.c \
../src/gpio_function.c \
../src/keypad_function.c \
../src/main.c 

OBJS += \
./src/7seg_function.o \
./src/button_function.o \
./src/calculator.o \
./src/gpio_function.o \
./src/keypad_function.o \
./src/main.o 

C_DEPS += \
./src/7seg_function.d \
./src/button_function.d \
./src/calculator.d \
./src/gpio_function.d \
./src/keypad_function.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -I"C:/Users/USER/STM32/st/workspace/Homework4_2/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


