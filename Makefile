CXX = g++
CXXFLAGS = -Wall -g -O0 -Iinclude -Irplidar_sdk/sdk/include -Irplidar_sdk/sdk/src -I/usr/include
LDFLAGS = -Lrplidar_sdk/output/Linux/Release
LDLIBS = -lsl_lidar_sdk -pthread
LDLIBS := /usr/lib/x86_64-linux-gnu/libi2c.a


TARGET = bin/myprogram
SRCDIR = src
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

all: lidarLib $(TARGET)
	@echo "Compilation terminée. Exécutez './$(TARGET)' pour exécuter le programme."

$(TARGET): $(OBJ) rplidar_sdk/output/Linux/Release/libsl_lidar_sdk.a| bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@


bin:
	mkdir -p bin

lidarLib:
	@echo "Compilation du sous-dossier lidarLib..."
	$(MAKE) -C rplidar_sdk

clean:
	rm -rf $(OBJDIR) bin/
	$(MAKE) -C rplidar_sdk clean