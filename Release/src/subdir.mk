################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/connect4_ai.cpp \
../src/game.cpp \
../src/interface.cpp \
../src/minimax.cpp \
../src/timeit.cpp 

CPP_DEPS += \
./src/connect4_ai.d \
./src/game.d \
./src/interface.d \
./src/minimax.d \
./src/timeit.d 

OBJS += \
./src/connect4_ai.o \
./src/game.o \
./src/interface.o \
./src/minimax.o \
./src/timeit.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/connect4_ai.d ./src/connect4_ai.o ./src/game.d ./src/game.o ./src/interface.d ./src/interface.o ./src/minimax.d ./src/minimax.o ./src/timeit.d ./src/timeit.o

.PHONY: clean-src

