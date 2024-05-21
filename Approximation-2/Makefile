build: main.o method.o test_sin.o test_sin_outl.o test_abs.o headers.h 
	gcc $^
	make clean
main.o: main.c
	gcc -c $^
test_sin.o: test_sin.c
	gcc -c $^
test_sin_outl.o: test_sin_outl.c
	gcc -c $^
test_abs.o: test_abs.c
	gcc -c $^
method.o: method.c
	gcc -c $^
clean:
	rm *.gch
	rm *.o