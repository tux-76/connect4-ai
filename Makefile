# Variables
CXX = g++
CXXFLAGS = -Wall -O2 -Iinclude
LIBNAME = libconnect4-ai.a
SRCS = src/game.cpp src/interface.cpp src/minimax.cpp src/timeit.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = $(LIBNAME)

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs lib/$(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) lib/$(TARGET)
	$(MAKE) -C examples clean
	$(MAKE) -C utils clean
