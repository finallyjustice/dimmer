CC = gcc
CFLAGS = -O2 -g

FF = g77
FFLAGS = -O2

all: stream_c.exe

stream_c.exe: stream.c
	$(CC) $(CFLAGS) stream.c -o stream_c.exe

clean:
	rm -f stream_f.exe stream_c.exe *.o
