SRC:=$(wildcard *.c)
client:$(SRC)
	gcc $^ -o $@ -lpthread -lmysqlclient -lcrypt
clean:
	rm -rf client

