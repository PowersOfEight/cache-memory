NAME=johnson
CC=gcc

README.txt:
	cat README.md > README.txt

README: README.txt

tarball: README
	tar -cvf $(NAME).tar.gz README.txt
