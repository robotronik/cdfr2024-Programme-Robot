CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O0 -Iinclude -Irplidar_sdk/sdk/include -Irplidar_sdk/sdk/src
LDFLAGS = -Lrplidar_sdk/output/Linux/Release
LDLIBS = -lsl_lidar_sdk -pthread -li2c -lpigpio -lrt -lpthread


TARGET = bin/programCDFR
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTTAGET := $(wildcard test/*.cpp)
TARGET += $(patsubst test/%.cpp,$(BINDIR)/%,$(TESTTAGET))
SIMPLETARGET := $(patsubst $(BINDIR)/%,%,$(TARGET))

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))
DEPENDS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRC))


.PHONY: all clean $(TARGET)

all: lidarLib $(TARGET)
	@echo "Compilation terminée!"
	@echo "$(SIMPLETARGET)"

$(TARGET): $(OBJ) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPENDS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR):
	mkdir -p $@

$(foreach exec,$(SIMPLETARGET),$(eval $(exec): lidarLib $(BINDIR)/$(exec)))

$(BINDIR):
	mkdir -p $(BINDIR)

lidarLib:
	@echo "Compilation du sous-dossier lidarLib..."
	$(MAKE) -C rplidar_sdk

clean:
	rm -rf $(OBJDIR) bin/
	$(MAKE) -C lidarLib clean