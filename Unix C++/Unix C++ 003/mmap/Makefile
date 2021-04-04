.PHONY: all clean print

CONFIG ?= Debug

PRODUCT = my_program
CPP_SRC = $(shell ls *.cpp)

CPP_O=$(patsubst %.cpp,%.cpp_o, $(CPP_SRC))
CPP_O_D=$(patsubst %.cpp,%.cpp_o.d, $(CPP_SRC))

PWD = $(shell pwd)

all: $(PRODUCT)

CPP_FLAGS = -std=c++11

ifeq ($(CONFIG), Debug)
	CPP_FLAGS += -D_DEBUG -g
else
	CPP_FLAGS += -D_RELEASE
endif

print:
	@echo CPP_O = $(CPP_O)

$(PRODUCT): $(CPP_O)
	@echo ===== link $@ ========
	g++ -o $@ $?

# include header file dependency
-include $(CPP_O_D)
	
%.cpp_o: %.cpp
	@echo ===== compile $@ ========
	g++ -o $@ -c "$(PWD)/$<" $(CPP_FLAGS) -MMD -MF $@.d
	
clean:
	-rm $(PRODUCT)
	-rm *.cpp_o
	-rm *.cpp_o.d