CPP = g++
HEADERDIR=include
CPPFLAGS = -O3 -g3 -march=native -flto -Wall -Wextra -pthread -std=gnu++14 -pthread \
						-I$(HEADERDIR)
LDFLAGS = $(CPPFLAGS) -lcrypto

TARGET = client
SRCDIR =src
OBJDIR =obj
DEPDIR =dep

SOURCE = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCE:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP = $(SOURCE:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
default: $(OBJDIR) $(DEPDIR) $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp $(DEPDIR)
	@echo "Creating dependency $@"
	@$(CPP) $(CPPFLAGS) -MM -o $@ $?
-include $(DEP)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $@"
	@$(CPP) $(CPPFLAGS) -c -o $@ $<


uwebsockets/libuWS.so:
	@$(MAKE) -C uwebsockets

$(TARGET): $(OBJECTS) uwebsockets/libuWS.so
	@mkdir -p $(DEPDIR)
	@echo "Linking client"
	@$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEP) *.ii *.s
	rmdir $(DEPDIR) $(OBJDIR)
	@$(MAKE) -C uwebsockets clean
