SRC:=$(wildcard *.c)
pthread_pool_server:$(SRC)
	gcc $^ -o $@ -lpthread -lmysqlclient -lcrypt
clean:
	rm -rf pthread_pool_server

