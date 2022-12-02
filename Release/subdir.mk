################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lcdvtwi.c \
../main.c \
../mkuart.c \
../mkuart2.c \
../twi.c 

OBJS += \
./lcdvtwi.o \
./main.o \
./mkuart.o \
./mkuart2.o \
./twi.o 

C_DEPS += \
./lcdvtwi.d \
./main.d \
./mkuart.d \
./mkuart2.d \
./twi.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


