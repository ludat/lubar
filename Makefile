CFLAGS=-Wall -g -I/usr/X11R6/include `pkg-config --cflags cairo pangocairo`
LDFLAGS=-Wall -g `pkg-config --libs cairo pangocairo` -L/usr/X11R6/lib -lX11

all: lubare

lubar: main.o
	cc -o lubar ${LDFLAGS} main.o

main.o: main.c
	cc ${CFLAGS} -c main.c

clean:
	rm lubare *.o
