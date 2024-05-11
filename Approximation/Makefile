build: main.o method.o headers.h 
	gcc $^
	make clean
main.o: main.c
	gcc -c $^
method.o: method.c
	gcc -c $^
clean:
	rm *.gch
	rm *.o