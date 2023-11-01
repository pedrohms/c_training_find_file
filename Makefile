CC=clang

all : build

clean:
	-rm app2

debug: app2.c
	$(CC) -ggdb -Wall -o app2 app2.c -L/nix/store/pjk6gld9s6434ykb76cmqpgxbqw8sf4b-mariadb-connector-c-3.3.5/lib/mariadb -lmariadb

build: app2.c
	$(CC) -Wall -o app2 app2.c -L/nix/store/pjk6gld9s6434ykb76cmqpgxbqw8sf4b-mariadb-connector-c-3.3.5/lib/mariadb -lmariadb
