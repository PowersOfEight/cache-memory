NAME=johnson
CC=gcc
FLAGS= -Wall -O3

data:
	mkdir data

bin:
	mkdir bin

cache.o: bin cache.c
	$(CC) -c $(FLAGS) -o bin/cache.o cache.c

cache2.o: bin cache_2.c
	$(CC) -c $(FLAGS) -o bin/cache2.o cache_2.c

cache: bin cache.o
	$(CC) $(FLAGS) -o bin/cache bin/cache.o


cache2: bin cache2.o
	$(CC) $(FLAGS) -o bin/cache2 bin/cache2.o

run: cache data
	./bin/cache
run2: cache2 data
	./bin/cache2
README.txt:
	cat README.md > README.txt

README: README.txt

tarball: README
	tar -czvf $(NAME).tar.gz README.txt

clean:
	rm -rf README.txt $(NAME).tar.gz bin data
