build: main.o headers.h library.o
	gcc $^ -lm -lpthread
	make clean
main.o: main.c
	gcc -c $^
library.o: library.c
	gcc -c $^
clean:
	rm *.gch
	rm *.o