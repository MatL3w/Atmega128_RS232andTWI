################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
ATMEGA128_INZYNIERKA.lss \

FLASH_IMAGE += \
ATMEGA128_INZYNIERKA.hex \

EEPROM_IMAGE += \
ATMEGA128_INZYNIERKA.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ATMEGA128_INZYNIERKA.elf secondary-outputs

# Tool invocations
ATMEGA128_INZYNIERKA.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ATMEGA128_INZYNIERKA.map -mmcu=atmega128 -o "ATMEGA128_INZYNIERKA.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ATMEGA128_INZYNIERKA.lss: ATMEGA128_INZYNIERKA.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ATMEGA128_INZYNIERKA.elf  >"ATMEGA128_INZYNIERKA.lss"
	@echo 'Finished building: $@'
	@echo ' '

ATMEGA128_INZYNIERKA.hex: ATMEGA128_INZYNIERKA.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ATMEGA128_INZYNIERKA.elf  "ATMEGA128_INZYNIERKA.hex"
	@echo 'Finished building: $@'
	@echo ' '

ATMEGA128_INZYNIERKA.eep: ATMEGA128_INZYNIERKA.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ATMEGA128_INZYNIERKA.elf  "ATMEGA128_INZYNIERKA.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ATMEGA128_INZYNIERKA.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 ATMEGA128_INZYNIERKA.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) ATMEGA128_INZYNIERKA.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
