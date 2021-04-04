

gcc -c test_lib/test.cpp -o test_lib/test.o

ar rcs test_lib/libtest.a test_lib/test.o

gcc -o my_program -Itest_lib -Ltest_lib main.cpp -ltest

