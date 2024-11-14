NAME=johnson
CC=gcc

README.txt:
	cat README.md > README.txt

README: README.txt

tarball: README
	tar -czvf $(NAME).tar.gz README.txt
