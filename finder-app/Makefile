writer : writer.o
	aarch64-none-linux-gnu-gcc -g -Wall -c -o writer.o writer.c
	aarch64-none-linux-gnu-gcc -g -Wall -I/ writer.o -o writer

main.o : main.c defs.h
	aarch64-none-linux-gnu-gcc -g -Wall -c -o main.o main.c

clean :
	rm writer writer.o
