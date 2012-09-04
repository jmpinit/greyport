void disk_initialize(void);

unsigned char disk_ready(void);
char* disk_get_dir(void);
int disk_get_space(void);
int disk_get_file_size(char* file);
void disk_delete_file(char* file);

void disk_open_writer(char* file);
void disk_open_reader(char* file);
void disk_close(char* file);
char* disk_read_file(char* file, int length);
void disk_write_file(char* file, char* data);
void disk_append(char* file, char* data);