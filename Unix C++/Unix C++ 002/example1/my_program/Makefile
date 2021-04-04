
.PHONY: clean

my_program: main.cpp ../test_lib/libtest.a
	@echo ====== link my_program =======
	gcc -o $@ -I../test_lib -L../test_lib main.cpp -ltest

clean:
	rm my_program	

