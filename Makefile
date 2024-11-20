NAME=johnson
CC=gcc
FLAGS= -Wall -O3

all: cache

data:
	mkdir data

bin:
	mkdir bin

cluster.o: bin cluster.c cluster.h
	$(CC) -c $(FLAGS) -o bin/cluster.o cluster.c

cache.o: bin cache.c
	$(CC) -c $(FLAGS) -o bin/cache.o cache.c

cache: bin cache.o cluster.o
	$(CC) $(FLAGS) -o bin/cache bin/cache.o bin/cluster.o 

run: cache data
	./bin/cache $(ARGS)

README.txt:
	cat README.md > README.txt

README: README.txt

tarball: README
	tar -czvf $(NAME).tar.gz README.txt

clean:
	rm -rf README.txt $(NAME).tar.gz bin data
