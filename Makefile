CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

OUT=zcc

$(OUT): $(OBJS)
	cc $(CFLAGS) -o $@ $(OBJS)

$(OBJS): $(OUT).h

test: $(OUT)
	./test.sh

clean:
	rm -f $(OUT) *.o

format:
	clang-format -i *.c *.h

docker_build:
	docker build -t $(OUT) .

docker_run:
	docker run -v $(PWD):/home -it $(OUT) /bin/bash

.PHONY: test clean format docker_build docker_run
