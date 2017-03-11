CPP = g++
CPPFLAGS = -O3 -g -march=native -Wall

TARGET = client
SOURCE = $(wildcard *.cpp)
OBJECTS = $(SOURCE:.cpp=.o)
DEP = $(SOURCE:.cpp=.d)

%.d: %.cpp
	@echo "Creating dependency $@"
	@$(CPP) $(CPPFLAGS) -MM -o $@ $?
-include $(DEP)


%.o: %.cpp
	@echo "Compiling $@"
	@$(CPP) $(CPPFLAGS) -MM -o $@ $?

all: $(TARGET)

$(TARGET): $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGET)
