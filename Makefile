CPP = g++
HEADERDIR = include
INCLUDE = -I$(HEADERDIR) -I/usr/local/include #Second option isosx openssl comand
CPPFLAGS = -O3 -g3 -march=native -flto -Wall -Wextra -pthread $(INCLUDE)
LDFLAGS = $(CPPFLAGS) -lcrypto -lssl -lz

TARGET = client
SRCDIR = src
OBJDIR = obj
DEPDIR = dep

SOURCE = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCE:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP = $(SOURCE:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
default: $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp $(DEPDIR)
	@echo "Creating dependency $@"
	@$(CPP) $(CPPFLAGS) -MM -o $@ $<
-include $(DEP)

# NOTE:: DO NOT LIST OBJDIR reciepe as a dependency
# This will cause make to always rebuild each object file
# since mkdir will update the file create time
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR) # Make the director this way
	@echo "Compiling $@"
	@$(CPP) $(CPPFLAGS) -c -o $@ $<


uwebsockets/libuWS.so:
	@$(MAKE) -C uwebsockets

$(TARGET): $(OBJECTS) uwebsockets/libuWS.so
	@mkdir -p $(DEPDIR)
	@echo "Linking client"
	@$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@rm -rdf $(OBJECTS) $(TARGET) $(DEP) *.ii *.s $(OBJDIR)
	@$(MAKE) -C uwebsockets clean
	@$(MAKE) -C src clean

.PHONY: clean
