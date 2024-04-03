CC = gcc
MCC = mpicc
LAB1 = lab1_1
LAB_OBJ= lab1_1.c config.c

LDLIBS = -lm

lab1: 
	$(CC) -o $(LAB1) $(LAB_OBJ) $(LDLIBS)

labmpi:
	$(MCC) -o lab1_2 lab1_2.c $(LDLIBS)


clean:

	$(info -> Makefile: cleanup previous builds...)
	@(rm $(LAB1))
	@(rm data.txt)