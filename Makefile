NAME=johnson
CC=gcc
FLAGS= -Wall -O3

data:
	mkdir data

bin:
	mkdir bin

cache.o: bin cache.c
	$(CC) -c $(FLAGS) -o bin/cache.o cache.c

cache: bin cache.o
	$(CC) $(FLAGS) -o bin/cache bin/cache.o

run: cache data
	./bin/cache

README.txt:
	cat README.md > README.txt

README: README.txt

tarball: README
	tar -czvf $(NAME).tar.gz README.txt

clean:
	rm -rf README.txt $(NAME).tar.gz bin data
