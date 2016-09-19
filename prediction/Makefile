# ================================================
#                 Fichier Makefile
# ================================================


EXEC = prediction
OBJS = patient.o contextemedical.o prediction.o predictionpartielle.o pile.o main.o
SRCS = patient.c contextemedical.c prediction.c predictionpartielle.c pile.c main.c

# ------------------------------------------------
#             NE PAS MODIFIER
# ------------------------------------------------

CC = gcc
FLAGCOMP = -c -Wall -ansi -pedantic -O3 -std=c99 -g #-pg
FLAGLINK = -o #-pg -o
LIB = -lm
INCL = -I.
LDFLAGS = 
CFLAGS = 

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(FLAGLINK) $(EXEC) $(OBJS) $(LIB)

.c.o:
	$(CC) $(CFLAGS) $(FLAGCOMP) $< $(INCL)

clean:
	rm -rf $(OBJS) $(EXEC) *~ .~

all:
	make depend
	make clean
	make
	mv $(EXEC) ../bin; make clean

info:
	echo "make [all] [depend] [clean] [info]"

depend: $(SRCS)
	makedepend -Y -- $(FLAGCOMP) -- $?

# DO NOT DELETE
