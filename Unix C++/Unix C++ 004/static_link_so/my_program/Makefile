.PHONY: all clean print

CONFIG ?= Debug

PRODUCT = ../my_prog
CPP_SRC = $(shell ls *.cpp)

CPP_O=$(patsubst %.cpp,%.cpp_o, $(CPP_SRC))
CPP_O_D=$(patsubst %.cpp,%.cpp_o.d, $(CPP_SRC))

PWD = $(shell pwd)

all: $(PRODUCT)

CPP_FLAGS = -std=c++11

# -L../ and -lmy_so to link libmy_so.so
# -Wl,-rpath,./ to tell linker my_prog will use ./ for .so search path
# please using rpath instead of LD_LIBRARY_PATH, because it might affect all program globally
LINK_FLAGS = -L.. -lmy_so -Wl,-rpath,./

# -g for debug information
ifeq ($(CONFIG), Debug)
	CPP_FLAGS += -D_DEBUG -g
else
	CPP_FLAGS += -D_RELEASE
endif

print:
	@echo CPP_O = $(CPP_O)

$(PRODUCT): $(CPP_O)
	@echo ===== link $@ ========
	g++ -o $@ $? $(LINK_FLAGS)

# include header file dependency
-include $(CPP_O_D)
	
%.cpp_o: %.cpp
	@echo ===== compile $@ ========
	g++ -o $@ -c "$(PWD)/$<" $(CPP_FLAGS) -MMD -MF $@.d
	
clean:
	-rm -f $(PRODUCT)
	-rm -f *.cpp_o
	-rm -f *.cpp_o.d