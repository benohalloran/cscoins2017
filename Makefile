CPP = g++
CPPFLAGS = -O3 -g -march=native -Wall -pthread -std=gnu++11
LDFLAGS = -pthread

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
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)
	@echo "Linking client"
	@$(CPP)  $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)
