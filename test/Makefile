
TEST = ./a

CXX = g++
RM = rm -f

CXXFLAGS = -g -MMD -MP -Wall -Wextra -Wunused-function -I../include \
           `pkg-config eigen3 --cflags` -DEIGEN_NO_DEBUG -Wno-int-in-bool-context \
		    -pthread
LDFLAGS = -lgtest.dll -lgtest_main.dll

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.PHONY: test clean

test: $(TEST)
	$(TEST)

clean:
	$(RM) $(TEST) $(OBJS) $(DEPS)

$(TEST): $(OBJS)
	$(CXX) -o $(TEST) $(OBJS) $(LDFLAGS)

.cpp.o: Makefile
	$(CXX) -c $< $(CXXFLAGS)

-include $(DEPS)
