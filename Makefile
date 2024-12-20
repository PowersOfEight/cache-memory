NAME=johnson
CC=gcc
FLAGS= -Wall -O3

data:
	mkdir data

bin:
	mkdir bin

linked_list.o: bin linked_list/linked_list.h linked_list/linked_list.c
	$(CC) -c $(FLAGS) -o bin/linked_list.o linked_list/linked_list.c


vector.o: bin vector/vector.h vector/vector.c
	$(CC) -c $(FLAGS) -o bin/vector.o vector/vector.c

cache_timer.o: cache_timer.c vector/vector.h bin
	$(CC) -c $(FLAGS) -o bin/cache_timer.o cache_timer.c

cache_timer: cache_timer.o vector.o linked_list.o bin
	$(CC) $(FLAGS) -o bin/cache_timer bin/cache_timer.o bin/vector.o bin/linked_list.o -lm

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
