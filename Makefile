# Variables ...................................................................:

## Compilateur ................................................................:

CC = gcc
CFLAGS = -g3 -Wall
LDFLAGS = -lm

## Structure du projet ........................................................:

GEFFE = bin/geffe
FEISTEL = bin/feistel
SPECTRE = bin/spectre
SRC_PATH = src/
OBJ_PATH = out/
INC_PATH = lib/
REPORT_PATH = doc/
SRC = $(shell find $(SRC_PATH)*.c)
OBJ = $(SRC:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

## Autres .....................................................................:

GEFFE_ARGS = 1 0 0 0 1 1 1 0 111100111100111010100101011111010110010011010011 16

FEISTEL_ARGS =

## Lancement ..................................................................:

geffe : compilgeffe
	@./$(GEFFE) $(GEFFE_ARGS)

feistel : compilfeistel
	@./$(FEISTEL) $(FEISTEL_ARGS)

## Spectre ....................................................................;

spectre : compilspectre
	@./$(SPECTRE) >> spectre.txt

compilspectre :
	@$(CC) src/spectre.c -o $(SPECTRE) $(LDFLAGS)

## Compilation Geffe ..........................................................:

compilgeffe : $(GEFFE)

$(GEFFE) : out/geffe.o
	@$(CC) $^ -o $(GEFFE) $(LDFLAGS)

out/geffe.o : src/geffe.c lib/geffe.h
	@$(CC) -c $< -o $@ $(CFLAGS)

## Compilation Feistel ........................................................:

compilfeistel : $(FEISTEL)

$(FEISTEL) : out/feistel.o
	@$(CC) $^ -o $(FEISTEL) $(LDFLAGS)

out/feistel.o : src/feistel.c lib/feistel.h
	@$(CC) -c $< -o $@ $(CFLAGS)

## Nettoyage ..................................................................:

clean :
	$(info Suppression de $(GEFFE) et de $(OBJ))
	@rm -f $(EXEC)
	@rm -f $(OBJ)
	@rm -f spectre.txt

## Debugger ...................................................................:

gdb : compil
	$(info Debbugage avec $@)
	@gdb --args ./$(EXEC) $(ARGS)
