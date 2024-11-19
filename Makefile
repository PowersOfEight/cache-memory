NAME=johnson
CC=gcc

bin:
	mkdir bin

cache.o: bin cache.c
	$(CC) -c -o bin/cache.o cache.c

cache: bin cache.o
	$(CC) -o bin/cache bin/cache.o

run: cache
	./bin/cache

README.txt:
	cat README.md > README.txt

README: README.txt

tarball: README
	tar -czvf $(NAME).tar.gz README.txt

clean:
	rm -rf README.txt $(NAME).tar.gz bin
