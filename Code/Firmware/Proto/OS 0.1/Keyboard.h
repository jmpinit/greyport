#define KEY_ALT			1
#define KEY_CTRL		2
#define KEY_SHIFT		3
#define KEY_BACKSPACE	8

//interface hardware
char key_get_key(void);
void key_store_state(void);
char* key_peek_state(void);
char* key_get_state(void);

//utilities
char key_decode(unsigned char index);