# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2018-19

SOURCES =router.c maze.c grid.c coordinate.c CircuitRouter-SeqSolver.c
SOURCES+= ../lib/vector.c ../lib/queue.c ../lib/list.c ../lib/pair.c ../lib/commandlinereader.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -std=gnu99 -g -I../
LDFLAGS=-lm
TARGET = CircuitRouter-SeqSolver

all: shell solver


shell: ../lib/commandlinereader.o
	$(CC) $(CFLAGS) ../lib/commandlinereader.o CircuitRouter-SimpleShell.c -o CircuitRouter-SimpleShell


solver: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS)

CircuitRouter-SeqSolver.o: CircuitRouter-SeqSolver.c maze.h router.h ../lib/list.h ../lib/timer.h ../lib/types.h
router.o: router.c router.h coordinate.h grid.h ../lib/queue.h ../lib/vector.h
maze.o: maze.c maze.h coordinate.h grid.h ../lib/list.h ../lib/queue.h ../lib/pair.h ../lib/types.h ../lib/vector.h
grid.o: grid.c grid.h coordinate.h ../lib/types.h ../lib/vector.h
coordinate.o: coordinate.c coordinate.h ../lib/pair.h ../lib/types.h
../lib/vector.o: ../lib/vector.c ../lib/vector.h ../lib/types.h ../lib/utility.h
../lib/queue.o: ../lib/queue.c ../lib/queue.h ../lib/types.h
../lib/list.o: ../lib/list.c ../lib/list.h ../lib/types.h
../lib/pair.o: ../lib/pair.c ../lib/pair.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	@echo Cleaning...
	rm -f *.res
	rm -f *.old
	rm -f $(OBJS) $(TARGET)
	rm -f CircuitRouter-SimpleShell CircuitRouter-SimpleShell.o
