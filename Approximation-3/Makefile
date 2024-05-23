build: main.o method.o tests.o headers.h 
	gcc $^
	make clean
main.o: main.c
	gcc -c $^
test_sin.o: tests.c
	gcc -c $^
method.o: method.c
	gcc -c $^
clean:
	rm *.gch
	rm *.o
