CC=gcc

base64: base64.o
	$(CC) -o base64 base64.o

base64.o: base64.c
	$(CC) -o base64.o -c base64.c

clean:
	rm -rf *.o

mrproper: clean
	rm -rf base64
