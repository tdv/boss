# OS: Windows Linux FreeBSD
#OS = Windows
OS = Linux
#OS = FreeBSD

# CXX: gcc clang
CXX = gcc
#CXX = clang

ifeq ($(CXX), gcc)
    COMPILER = g++
endif

ifeq ($(CXX), clang)
    COMPILER = clang++
endif

LIBS =

ifeq ($(OS), Windows)
    EXT = .exe
    EXT_SO = .dll
    PREF_SO =
endif
ifeq ($(OS), Linux)
    EXT =
    EXT_SO = .so
    PREF_SO = lib
    LIBS += -ldl
endif
ifeq ($(OS), FreeBSD)
    EXT =
    EXT_SO = .so
    PREF_SO = lib
    LIBS +=
endif

FLAG_CPP = -Wall -O3 -g0 -std=c++11 -static-libstdc++ -static-libgcc
INCLUDE = -Iboss/include
FLAG_SHARED = -shared -fPIC

SRC_DIR = boss/src
OUT_DIR = bin

all: core examples
	@echo "All: Ok!"

core: service_registry class_factory regtool
	@echo "Core: Ok!"

service_registry:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) $(SRC_DIR)/service_registry/*.cpp \
	$(FLAG_SHARED) -o $(OUT_DIR)/$(PREF_SO)service_registry$(EXT_SO) $(LIBS)

class_factory:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) $(SRC_DIR)/class_factory/*.cpp \
	$(FLAG_SHARED) -o $(OUT_DIR)/$(PREF_SO)class_factory$(EXT_SO) $(LIBS)

regtool:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) \
	-I$(SRC_DIR)/service_registry \
	$(SRC_DIR)/service_registry/service_registry.cpp \
	$(SRC_DIR)/regtool/*.cpp -o $(OUT_DIR)/regtool$(EXT) $(LIBS)

examples: simple_component_client multy_component_client simple_enum \
simple_object objects_as_params query_interface_1 query_interface_2 \
string_enum construct_destroy multy_object
	@echo "Examples: Ok!"

simple_component:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) -Iboss/examples/simple_component/include \
	boss/examples/simple_component/calc_service/*.cpp \
	$(FLAG_SHARED) -o $(OUT_DIR)/$(PREF_SO)calc_service$(EXT_SO) $(LIBS)

simple_component_client: simple_component
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) \
	boss/examples/simple_component/client/*.cpp \
	-Iboss/examples/simple_component/include \
	-o $(OUT_DIR)/simple_component_client$(EXT) $(LIBS)

simple_enum:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/simple_enum/*.cpp \
	-o $(OUT_DIR)/simple_enum$(EXT) $(LIBS)

simple_object:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/simple_object/*.cpp \
	-o $(OUT_DIR)/simple_object$(EXT) $(LIBS)

objects_as_params:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/objects_as_params/*.cpp \
	-o $(OUT_DIR)/objects_as_params$(EXT) $(LIBS)

query_interface_1:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/query_interface_1/*.cpp \
	-o $(OUT_DIR)/query_interface_1$(EXT) $(LIBS)

query_interface_2:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/query_interface_2/*.cpp \
	-o $(OUT_DIR)/query_interface_2$(EXT) $(LIBS)

string_enum:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/string_enum/*.cpp \
	-o $(OUT_DIR)/string_enum$(EXT) $(LIBS)

construct_destroy:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/construct_destroy/*.cpp \
	-o $(OUT_DIR)/construct_destroy$(EXT) $(LIBS)

multy_object:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) boss/examples/multy_object/*.cpp \
	-o $(OUT_DIR)/multy_object$(EXT) $(LIBS)

multy_component:
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) -Iboss/examples/multy_component/include \
	boss/examples/multy_component/service/*.cpp \
	$(FLAG_SHARED) -o $(OUT_DIR)/$(PREF_SO)multy_component_service$(EXT_SO) $(LIBS)

multy_component_client: multy_component
	$(COMPILER) $(FLAG_CPP) $(INCLUDE) \
	boss/examples/multy_component/client/*.cpp \
	-Iboss/examples/multy_component/include \
	-o $(OUT_DIR)/multy_component_client$(EXT) $(LIBS)
