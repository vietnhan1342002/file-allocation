#include <stdio.h>
#include <string.h>
#include "main.h"

#define MAX_DIR_SIZE 30
#define MAX_FILENAME_SIZE 13

static const char* dir_filename = "dir.dat";

static char filenames[MAX_DIR_SIZE][MAX_FILENAME_SIZE];
static int file_start_sectors[MAX_DIR_SIZE];

FILE* open_dir_file(char * options) {
	static FILE* dir_file;

	if(dir_file == NULL) {
		dir_file = fopen(dir_filename, "w");
		fclose(dir_file);
		
		dir_file = fopen(dir_filename, options);
	} else {
		dir_file = freopen(dir_filename, options, dir_file);
	}

	return dir_file;
}

// Reads the dir.dat file into the directory in memory, one entry per line
void read_dir() {
	FILE* dir_file = open_dir_file((char*)"r");

	char filename[MAX_FILENAME_SIZE];
	int file_start_sector;

	int i = 0;
	while(fscanf(dir_file, "%s %d", filename, &file_start_sector) != EOF) {
		strcpy(filenames[i], filename);
		file_start_sectors[i] = file_start_sector;

		i++;
	}

	fclose(dir_file);
}

// Writes the contents of the directory in memory to the dir.dat file, one entry per line
void save_dir() {
	FILE* dir_file = open_dir_file((char*)"w");

	for(int i = 0; i < MAX_DIR_SIZE; i++) {
		if(file_start_sectors[i] != 0) {
			fprintf(dir_file, "%s %d\n", filenames[i], file_start_sectors[i]);
		}
	}

	fclose(dir_file);

}

// Prints the contents of the directory in memory, one entry per line
void print_dir() {
	for(int i = 0; i < MAX_DIR_SIZE; i++) {
		if(file_start_sectors[i] != 0) {
			printf("%s %d\n", filenames[i], file_start_sectors[i]);
		}
	}
}

// Returns the number of files that the directory has remaining space for
int dir_space_left() {
	int space_left = 0;

	for(int i = 0; i < MAX_DIR_SIZE; i++) {
		if(file_start_sectors[i] == 0) {
			space_left++;
		}
	}

	return space_left;
}

// Returns the directory index of the file with the given file name if it exists, -1 otherwise
int does_file_exist_in_dir(char* file) {
	for(int i = 0; i < MAX_DIR_SIZE; i++) {
		if(strcmp(filenames[i], file) == 0) {
			return i;
		}
	}

	return -1;
}

// Returns the start sector of the file at directory index position
int get_start_sector(int position) {
	return file_start_sectors[position];
}

// Adds a file to the directory with the given file name and start sector
void add_file_to_dir(char* file, int start_sector) {
	for(int i = 0; i < MAX_DIR_SIZE; i++) {
		if(file_start_sectors[i] == 0) {
			strcpy(filenames[i], file);
			file_start_sectors[i] = start_sector;
			return;
		}
	}
}

// Deletes the file with directory index position from the directory
void delete_file_from_dir(int position) {
	strcpy(filenames[position], "");
	file_start_sectors[position] = 0;
}







