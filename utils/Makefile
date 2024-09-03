# Variables
AI_LIB_DISTRIBUTION=..
AI_INCDIR=$(AI_LIB_DISTRIBUTION)/include
AI_LIBDIR=$(AI_LIB_DISTRIBUTION)/lib
AI_LIBRARY_NAME=connect4-ai
AI_LIBRARY=$(AI_LIBDIR)/lib$(AI_LIBRARY_NAME).a

BINARIES = round-robin

CXX = g++
CXXFLAGS = -Wall -O2 -I$(AI_INCDIR)
LDFLAGS = -L$(AI_LIBDIR) -l$(AI_LIBRARY_NAME)
SRCS = round-robin.cpp
OBJS = $(SRCS:.cpp=.o)

# Targets
all: $(BINARIES)

$(AI_LIBRARY):
	$(MAKE) -C $(AI_LIB_DISTRIBUTION)

round-robin: $(OBJS) $(AI_LIBRARY)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BINARIES)

