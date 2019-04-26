INC_DIR := ./include ./include/network 
SRC_DIR := ./src ./src/network
CXXFLAGS := -std=c++11 -g -Wno-deprecated
LIBS := -ljson -lpthread
INC_FILE := $(addprefix -I, $(INC_DIR))
SRC_FILE := $(wildcard ./src/*.cpp) $(wildcard ./src/network/*.cpp)
TARGET := server
$(TARGET):$(SRC_FILE)
	g++ $(CXXFLAGS) -o $@ $(SRC_FILE) $(INC_FILE) $(LIBS)
clean:
	rm -rf $(TARGET) 
