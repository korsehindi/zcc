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

.PHONY: test clean format
