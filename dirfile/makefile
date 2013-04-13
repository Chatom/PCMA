#
# Pascal Compiler to Mips Architecture
# 
# @file: makefile
# 
# @author: Tom CHASSAGNE
# 
# @date: 10/04/13
# 
# @brief: Makefile pour la compilation gcc du projet
# 

INCLUDE = ../include

COMPILER = gcc -I$(INCLUDE) -Wall -w -g -ansi -pedantic -std=c99 -Werror $*.c

all: PCMA

PCMA: 
	gcc -o PCMA.run -I$(INCLUDE) -Wall -w -g -ansi -pedantic -std=c99 -Werror PCMA.c

# Edition de liens
#
#$(nom).run : $(nom).o
#	gcc -s -o $(nom).run $(nom).o
#
#$(nom).o : $(nom).c
#	$(COMPILER)

#
# Nettoyage du repertoire courant : executables et fichiers .o
#
clean :
	clear; rm -rf *.o *.run ../lib -v *~ ../include/*~

