# Adapted from: https://www.partow.net/programming/makefile/index.html
FLAGS := -pedantic-errors -Wall -Wextra -Werror -lzmq -lzmqpp -lpthread
LDFLAGS := -lzmq -lzmqpp -lpthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
INCLUDE  := -Iinclude/
TARG_DIR := build/objects/src
TARGETS	 := $(TARG_DIR)/broker.o $(TARG_DIR)/main.o $(TARG_DIR)/test_app.o
SRC      :=                      \
   $(wildcard src/*.cpp)         \
   $(wildcard src/message/*.cpp)         \

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/main $(APP_DIR)/broker $(APP_DIR)/test_app

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/broker: $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/broker $(filter-out $(filter-out %/broker.o, $(TARGETS)), $^) $(LDFLAGS)

$(APP_DIR)/main: $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/main $(filter-out $(filter-out %/main.o, $(TARGETS)), $^) $(LDFLAGS)

$(APP_DIR)/test_app: $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/test_app $(filter-out $(filter-out %/test_app.o, $(TARGETS)), $^) $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
