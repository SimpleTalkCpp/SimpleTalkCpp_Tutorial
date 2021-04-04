.PHONY: all clean print

CONFIG ?= Debug

PRODUCT = my_so.so
CPP_SRC = $(shell ls *.cpp)

CPP_O=$(patsubst %.cpp,%.cpp_o, $(CPP_SRC))
CPP_O_D=$(patsubst %.cpp,%.cpp_o.d, $(CPP_SRC))

PWD = $(shell pwd)

all: $(PRODUCT)

# added -fPIC when compile .cpp
# added MY_SO_EXPORT to export functions
CPP_FLAGS = -std=c++11 -fPIC -DMY_SO_EXPORT

LINK_FLAGS = -shared

# -g for debug information
ifeq ($(CONFIG), Debug)
	CPP_FLAGS += -D_DEBUG -g
else
	CPP_FLAGS += -D_RELEASE
endif

print:
	@echo CPP_O = $(CPP_O)

# add -shared for .so
$(PRODUCT): $(CPP_O)
	@echo ===== link $@ ========	
	g++ -shared -o $@ $?

# include header file dependency
-include $(CPP_O_D)
	
%.cpp_o: %.cpp
	@echo ===== compile $@ ========
	g++ -o $@ -c "$(PWD)/$<" $(CPP_FLAGS) -MMD -MF $@.d
	
clean:
	-rm $(PRODUCT)
	-rm *.cpp_o
	-rm *.cpp_o.d