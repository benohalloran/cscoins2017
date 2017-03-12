CPP = g++
CPPFLAGS = -O3 -g -march=native -Wall

TARGET = client
SOURCE = $(wildcard *.cpp)
OBJECTS = $(SOURCE:.cpp=.o)
DEP = $(SOURCE:.cpp=.d)

default: $(TARGET)


%.d: %.cpp
	@echo "Creating dependency $@"
	@$(CPP) $(CPPFLAGS) -MM -o $@ $?
-include $(DEP)


%.o: %.cpp
	@echo "Compiling $@"
	@$(CPP) $(CPPFLAGS) -c -o $@ $?

$(TARGET): $(OBJECTS)
	@echo "Linking client"
	@$(CPP)  $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)
