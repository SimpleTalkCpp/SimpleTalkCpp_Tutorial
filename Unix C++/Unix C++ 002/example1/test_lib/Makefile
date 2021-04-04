.PHONY: clean

libtest.a: test.o B.o
	@echo ====== ar libtest.a ========
	ar rcs $@ $<
	
test.o: test.cpp
	@echo ====== compile test.cpp ======
	gcc -c $? -o $@
		
clean:
	rm test.o
	rm libtest.a
