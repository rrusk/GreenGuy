CC = gcc
#CFLAGS = -g -I/project/X11/include -I/home/csc405/include
#LIBSX  = /home/csc405/lib/libsx.a
#LIBS   =  $(LIBSX) -lXaw -lXmu -lXext -lXt -lX11 -lm -lnsl -lsocket
#LDFLAGS = -L/project/X11/lib
LDFLAGS = -L/usr/lib
LIBS   =  -lsx -lXaw -lXmu -lXext -lXt -lX11 -lm -lnsl #-lsocket

All: GreenGuy permissions

GreenGuy: GreenGuy.o object_list.o vertex_list.o creq.o freq.o dirlist.o \
	bitmaps.h
	$(CC) $(LDFLAGS) -o GreenGuy GreenGuy.o object_list.o vertex_list.o \
	creq.o freq.o dirlist.o $(LIBS)

GreenGuy.o: GreenGuy.c object_list.h vertex_list.h creq.h freq.h bitmaps.h

object_list.o: object_list.c object_list.h vertex_list.h

vertex_list.o: vertex_list.c vertex_list.h

creq.o: creq.c creq.h

freq.o: freq.c freq.h

dirlist.o: dirlist.c

permissions:
	chmod a-wx *.guy

clean:
	rm -f core *.o GreenGuy

# Program submission

# For Part A
submit-A:
	/home/will/bin/submit-A README Makefile GreenGuy.c bitmaps.h \
	object_list.h object_list.c vertex_list.h vertex_list.c \
	creq.h creq.c freq.h freq.c dirlist.c \
	gg.guy bg.guy rg.guy yg.guy pg.guy example1.gg example2.gg \
	example3.gg

# For Part B
submit-B:
	/home/will/bin/submit-B PartB
