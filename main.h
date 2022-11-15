// fat.c
void read_fat();
void save_fat();
void reset_fat();
int free_sectors();
int first_free_sector();
void allocate_sectors(int start_sector, int n);
void deallocate_sectors(int start_sector);
void print_sector_chain(int start_sector);

// dir.c
void read_dir();
void save_dir();
void print_dir();
int dir_space_left();
int does_file_exist_in_dir(char* file);
int get_start_sector(int position);
void add_file_to_dir(char* file, int start_sector);
void delete_file_from_dir(int position);
