#include <stdio.h>
#include "main.h"

#define FAT_SIZE 32

static const char* fat_filename = "fat.dat";

static int fat[FAT_SIZE];

FILE* open_fat_file(char * options) {
	static FILE* fat_file;

	if(fat_file == NULL) {
		fat_file = fopen(fat_filename, "w");
		fclose(fat_file);

		fat_file = fopen(fat_filename, options);
	} else {
		fat_file = freopen(fat_filename, options, fat_file);
	}

	return fat_file;
}

// Reads the fat.dat file into the FAT in memory (int fat[32]), one int per line
void read_fat() {
	FILE* fat_file = open_fat_file((char*)"r");

	int entry;
	int i = 0;
	while(fscanf(fat_file, "%d", &entry) != EOF) {
		fat[i] = entry;
		i++;
	}

	// fclose(fat_file);
}

// Writes the FAT in memory (int fat[32]) to the fat.dat file, one int per line
void save_fat() {
	FILE* fat_file = open_fat_file((char*)"w");

	for(int i = 0; i < FAT_SIZE; i++) {
		fprintf(fat_file, "%d\n", fat[i]);
	}

	fclose(fat_file);
}

// Clears the FAT in memory (int fat[32])
void reset_fat() {
	fat[0] = 0;
	fat[1] = 0;
	
	for(int i = 2; i < FAT_SIZE; i++) {
		fat[i] = 1;
	}
}

// Returns the number of free sectors in the FAT
int free_sectors() {
	int num_free = 0;

	for(int i = 0; i < FAT_SIZE; i++) {
		if(fat[i] == 1) {
			num_free++;
		}
	}

	return num_free;
}

// Returns the index of the lowest number sector that is currently free
int first_free_sector() {
	for(int i = 0; i < FAT_SIZE; i++) {
		if(fat[i] == 1) {
			return i;
		}
	}

	return -1;
}

// Allocates a chain of sectors n long, starting at start_sector
void allocate_sectors(int start_sector, int n) {
	if(n == 1) {
		fat[start_sector] = 0;
	} else {
		int sectors_allocated = 0;
		int previous_sector = start_sector;

		for(int i = 0; i < FAT_SIZE; i++) {
			if(fat[i] == 1 && i != start_sector) {
				fat[previous_sector] = i;
				previous_sector = i;

				sectors_allocated++;

				if(sectors_allocated == n - 1) {
					fat[previous_sector] = 0;
					break;
				}
			}
		}
	}
}

// Deallocates the chain of sectors starting at start_sector
void deallocate_sectors(int start_sector) {
	int current_sector = start_sector;
	int next_sector;

	do {
		next_sector = fat[current_sector];
		fat[current_sector] = 1;
		current_sector = next_sector;
	} while(next_sector != 0);
}

// Prints the chain of sectors that starts at start_sector
void print_sector_chain(int start_sector) {
	int current_sector = start_sector;
	int next_sector;

	do {
		printf("%d", current_sector);
		next_sector = fat[current_sector];
		current_sector = next_sector;

		if(next_sector != 0) {
			printf(" ");
		}
	} while(next_sector != 0);

	printf("\n");
}







