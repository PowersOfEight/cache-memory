NAME=johnson
CC=gcc
FLAGS= -Wall -O3

data:
	mkdir data

bin:
	mkdir bin

cache_timer.o: cache_timer.c bin
	$(CC) -c $(FLAGS) -o bin/cache_timer.o cache_timer.c

cache_timer: cache_timer.o bin
	$(CC) $(FLAGS) -o bin/cache_timer bin/cache_timer.o

run-cache-timer: cache_timer data
	sudo ./bin/cache_timer $(ARGS)

cluster.o: bin cluster.c cluster.h
	$(CC) -c $(FLAGS) -o bin/cluster.o cluster.c

cache.o: bin cache.c
	$(CC) -c $(FLAGS) -o bin/cache.o cache.c

cache2.o: bin cache_2.c
	$(CC) -c $(FLAGS) -o bin/cache2.o cache_2.c

cache: bin cache.o cluster.o
	$(CC) $(FLAGS) -o bin/cache bin/cache.o bin/cluster.o


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
