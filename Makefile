OBJECTS = spellChecker.o levDis.o hashTable.o
EXECUTABLES = spellChecker attacker
CFLAGS = -g -Wall

all : spellChecker attacker
.PHONY : all

spellChecker : spellChecker.o levDis.o hashTable.o
	gcc $(CFLAGS) -o spellChecker spellChecker.o levDis.o hashTable.o
attacker : attacker.c
	gcc $(CFLAGS) -o attacker attacker.c

spellChecker.o : spellChecker.c levDis.h hashTable.h
	gcc $(CFLAGS) -c spellChecker.c
levDis.o : levDis.c
	gcc $(CFLAGS) -c levDis.c
hashTable.o : hashTable.c hashTable.h
	gcc $(CFLAGS) -c hashTable.c

clean : 
	rm $(OBJECTS) $(EXECUTABLES)
