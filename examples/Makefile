# Variables
AI_LIB_DISTRIBUTION=..
AI_INCDIR=$(AI_LIB_DISTRIBUTION)/include
AI_LIBDIR=$(AI_LIB_DISTRIBUTION)/lib
AI_LIBRARY_NAME=connect4-ai
AI_LIBRARY=$(AI_LIBDIR)/lib$(AI_LIBRARY_NAME).a

TARGET = terminal-example

CXX = g++
CXXFLAGS = -Wall -O2 -I$(AI_INCDIR)
LDFLAGS = -L$(AI_LIBDIR) -l$(AI_LIBRARY_NAME)
SRCS = $(TARGET).cpp
OBJS = $(SRCS:.cpp=.o)

# Targets
all: $(TARGET)

$(AI_LIBRARY):
	$(MAKE) -C $(AI_LIB_DISTRIBUTION)

$(TARGET): $(OBJS) $(AI_LIBRARY)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
