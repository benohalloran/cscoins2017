CPP = g++
CPPFLAGS = -O3 -g3 -march=native -flto -Wall -Wextra -pthread -std=gnu++14 -pthread
LDFLAGS = $(CPPFLAGS) -lcrypto

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
	@$(CPP) $(CPPFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo "Linking client"
	@$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEP) *.ii *.s
