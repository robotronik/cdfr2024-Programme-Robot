CXX = g++
CXXFLAGS = -Wall -g -O0 -Iinclude -Irplidar_sdk/sdk/include -Irplidar_sdk/sdk/src
LDFLAGS = -Lrplidar_sdk/output/Linux/Release
LDLIBS = -lsl_lidar_sdk -pthread -li2c


TARGET = bin/myprogram
SRCDIR = src
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

all: lidarLib $(TARGET)
	@echo "Compilation terminée. Exécutez './$(TARGET)' pour exécuter le programme."

$(TARGET): $(OBJ) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@


bin:
	mkdir -p bin

clean:
	rm -rf $(OBJDIR) bin/
	$(MAKE) -C rplidar_sdk clean
