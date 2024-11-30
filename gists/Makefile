CC = gcc
NAME = johnson
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
OBJ_DIR = bin
SRC = cache_timer.c
OBJ = $(OBJ_DIR)/cache_timer.o
EXEC = cache-main

all: $(EXEC)

data:
	mkdir data

cache-main: cache-main.o cache-size.o cache-block.o linked_list.o utils.o cache-times.o
	$(CC) $(CFLAGS) -o $@ $^

cache-main.o: cache-main.c linked_list.o cache-size.o 
	$(CC) $(CFLAGS) -c $< -o $@

cache-block.o: cache-block.c cache-block.h
	$(CC) $(CFLAGS) -c $< -o $@

linked_list.o: linked_list/linked_list.c linked_list/linked_list.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: utils/utils.c utils/utils.h
	$(CC) $(CFLAGS) -c utils/utils.c -o utils.o

cache-size.o: cache-size.c
	$(CC) $(CFLAGS) -o cache-size.o -c $<

cache-times.o: cache-times.c cache-times.h data
	$(CC) $(CFLAGS) -c $< -o $@ 

run-%: %
	./$(*)

clean:
	rm -rf $(OBJ_DIR) $(EXEC) data

tarball: 
	tar -czvf $(NAME).tar.gz ./*
