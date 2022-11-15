#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

void command_sectors(char* filename);
void command_add(char* filename, int num_sectors);
void command_delete(char* filename);

int main() {
	reset_fat();
	
	read_fat();
	read_dir();

	char line[100];
	char command[20];
	char arg1[20];
	char arg2[20];

	while(1) {
		printf(":");

		fgets(line, sizeof(line) / sizeof(line[0]), stdin);
		if(sscanf(line, "%s %s %s", command, arg1, arg2) < 1) {
			break;
		}

		if(strcmp(command, "sectors") == 0) {
			command_sectors(arg1);

		} else if(strcmp(command, "add") == 0) {
			command_add(arg1, strtol(arg2, NULL, 10));

		} else if(strcmp(command, "delete") == 0) {
			command_delete(arg1);

		} else if(strcmp(command, "dir") == 0) {
			print_dir();

		} else if(strcmp(command, "quit") == 0) {
			break;

		}
	}

	return 0;
}

// Prints the sectors used by the file with the given filename
void command_sectors(char* filename) {
	int file_position = does_file_exist_in_dir(filename);

	if(file_position < 0) {
		printf("File does not exist\n");
		return;
	}

	int start_sector = get_start_sector(file_position);
	print_sector_chain(start_sector);
}

// Adds a file with the given filename to the directory and allocates it num_sectors in the FAT
void command_add(char* filename, int num_sectors) {
	if(does_file_exist_in_dir(filename) >= 0) {
		printf("File already exists\n");
		return;
	}
	
	if(dir_space_left() == 0) {
		printf("Directory is full\n");
		return;
	}

	if(free_sectors() < num_sectors) {
		printf("Not enough free sectors\n");
		return;
	}

	int start_sector = first_free_sector();
	allocate_sectors(start_sector, num_sectors);

	add_file_to_dir(filename, start_sector);

	save_dir();
	save_fat();
}

// Deletes the file with the given filename from the directory and deallocates its sectors in the FAT
void command_delete(char* filename) {
	int file_position = does_file_exist_in_dir(filename);

	if(file_position < 0) {
		printf("File does not exist\n");
		return;
	}

	int start_sector = get_start_sector(file_position);
	deallocate_sectors(start_sector);

	delete_file_from_dir(file_position);

	save_dir();
	save_fat();
}
