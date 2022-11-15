fat_sim.out : main.o fat.o dir.o
	gcc -o fat_sim.out main.o fat.o dir.o

main.o : main.c main.h
	gcc -c main.c

fat.o : fat.c main.h
	gcc -c fat.c

dir.o : dir.c main.h
	gcc -c dir.c
