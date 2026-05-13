CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = toll_plaza

SRCS = main.cpp vehicle.cpp queue.cpp stack.cpp utils.cpp simulator.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe *.csv

run: all
	./$(TARGET)

.PHONY: all clean run
