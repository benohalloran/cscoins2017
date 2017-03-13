CPP = g++
CPPFLAGS = -O3 -g3 -march=native -flto -Wall -Wextra -pthread -std=gnu++14 -pthread
LDFLAGS = $(CPPFLAGS) -lcrypto

TARGET = client
SRCDIR =src
OBJDIR =obj
DEPDIR =dep

SOURCE = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCE:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP = $(SOURCE:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
default: $(TARGET)

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@echo "Creating dependency $@"
	@$(CPP) $(CPPFLAGS) -MM -o $@ $?
-include $(DEP)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $@"
	@$(CPP) $(CPPFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)
	@echo "Linking client"
	@$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEP) *.ii *.s
