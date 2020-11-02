all: ./bin/main

run: ./bin/main
	./bin/main

test: ./bin/tests
	./bin/tests

clean:
	rm -rf ./obj/*

./bin/main: main.c queue.c queue.h process.c process.h event.c event.h
	gcc -Wall -o ./bin/main main.c queue.c process.c event.c

./bin/tests: tests.c queue.c queue.h process.c process.h event.c event.h
	gcc -Wall -o ./bin/tests tests.c queue.c process.c event.c

