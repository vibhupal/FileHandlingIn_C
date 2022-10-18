SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin
DATA = ../data
RM = rm * .o

INCLUDEFLAGS = -I../include
CFLAGS = -c -g -Wall  ${INCLUDEFLAGS}
all:
	gcc ${CFLAGS} ${SRCDIR}/main.c -o ${OBJDIR}/main.o
	gcc ${OBJDIR}/main.o -o ${BINDIR}/main.exe
clean:
	rm ${OBJDIR}/main.o
