CXX := g++

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj

PT_SRC_DIR := $(SRC_DIR)/pathTracer
SE_SRC_DIR := $(SRC_DIR)/sceneExplorer
COM_SRC_DIR := $(SRC_DIR)/common

PT_TARGET_EXEC := $(BIN_DIR)/myPT
SE_TARGET_EXEC := $(BIN_DIR)/mySceneExp

PT_INC_PATHS := -I./include
SE_INC_PATHS := -I./include 

all: $(OBJ_DIR) $(PT_TARGET_EXEC) $(SE_TARGET_EXEC)

# DIRECTORY FOR BINARIES

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# COMMON

COM_CPP_FILES := $(shell find $(COM_SRC_DIR) -name '*.cpp') 
COM_HPP_FILES := $(shell find $(COM_SRC_DIR) -name '*.hpp')
TEMP := $(patsubst %.cpp, %.o, $(COM_CPP_FILES))
COM_OBJ_FILES := $(patsubst $(COM_SRC_DIR)/%, $(OBJ_DIR)/%, $(TEMP)) 

$(OBJ_DIR)/comUtils.o: $(COM_SRC_DIR)/comUtils.cpp $(COM_HPP_FILES)
	$(CXX) -c $(COM_SRC_DIR)/comUtils.cpp -o $@

# SCENE EXPLORER

SE_CPP_FILES := $(shell find $(SE_SRC_DIR) -name '*.cpp') $(COM_CPP_FILES) 
SE_HPP_FILES := $(shell find $(SE_SRC_DIR) -name '*.hpp') $(COM_HPP_FILES)

SE_LIBS := -lGL -lSDL2 -lGLEW -lassimp 

$(SE_TARGET_EXEC): $(SE_CPP_FILES) $(SE_HPP_FILES)
	$(CXX) $(SE_CPP_FILES) $(SE_INC_PATHS) $(SE_LIBS) -o $@

# PATH TRACER

PT_CPP_FILES := $(shell find $(PT_SRC_DIR) -name '*.cpp')
PT_HPP_FILES := $(shell find $(PT_SRC_DIR) -name '*.hpp') $(COM_HPP_FILES)

PT_LIBS := -lassimp

TEMP := $(patsubst %.cpp, %.o, $(PT_CPP_FILES))
PT_OBJ_FILES := $(patsubst $(PT_SRC_DIR)/%, $(OBJ_DIR)/%, $(TEMP)) $(COM_OBJ_FILES)

$(PT_TARGET_EXEC): $(PT_OBJ_FILES)
	$(CXX) $(PT_OBJ_FILES) $(PT_LIBS) -o $@

$(OBJ_DIR)/aabb.o: $(PT_SRC_DIR)/aabb.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/aabb.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/camera.o: $(PT_SRC_DIR)/camera.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/camera.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/image.o: $(PT_SRC_DIR)/image.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/image.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/interval.o: $(PT_SRC_DIR)/interval.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/interval.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/main.o: $(PT_SRC_DIR)/main.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/main.cpp $(PT_INC_PATHS) -o $@ 

$(OBJ_DIR)/material.o: $(PT_SRC_DIR)/material.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/material.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/mesh.o: $(PT_SRC_DIR)/mesh.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/mesh.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/model.o: $(PT_SRC_DIR)/model.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/model.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/scenes.o: $(PT_SRC_DIR)/scenes.cpp $(PT_HPP_FILES) 
	$(CXX) -c $(PT_SRC_DIR)/scenes.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/sphere.o: $(PT_SRC_DIR)/sphere.cpp $(PT_HPP_FILES) 
	$(CXX) -c $(PT_SRC_DIR)/sphere.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/triangle.o: $(PT_SRC_DIR)/triangle.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/triangle.cpp $(PT_INC_PATHS) -o $@

$(OBJ_DIR)/utilities.o: $(PT_SRC_DIR)/utilities.cpp $(PT_HPP_FILES)
	$(CXX) -c $(PT_SRC_DIR)/utilities.cpp $(PT_INC_PATHS) -o $@

clean:
	rm $(PT_TARGET_EXEC)
	rm $(SE_TARGET_EXEC)
	rm $(OBJ_DIR)/*
