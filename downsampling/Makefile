C = g++
CFLAGS = -g -Wall
SRCS = downsample.cpp
PROG = downsample.o

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
