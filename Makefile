CFLAGS+=-Wall -ansi

all: setrlimit

setrlimit: setrlimit.o

clean:
	rm -f *.o setrlimit
