.PHONY: all

PROTO_FILES = test.proto
PB_CC = $(patsubst %.proto,%.pb.cc, $(PROTO_FILES))

all: $(PB_CC)

%.pb.cc: %.proto
	@echo ==== compile $< =====
	../external/_prebuild/x64/protobuf/Release/protoc.exe $< --cpp_out=./