unsigned char invert;

void screen_initialize(void);
void screen_clear(unsigned char color);
void screen_clear_shadow(void);

void screen_print_string(char* addr);
void screen_print_hex(unsigned char val);
void screen_print_char(char c);

void screen_draw_bitmap();

void screen_draw_char(unsigned char x, unsigned char y, char c);
void screen_draw_inverted_char(unsigned char x, unsigned char y, char c);

unsigned char screen_get_x();
unsigned char screen_get_y();
void screen_set_pos(unsigned char x, unsigned char y);

void screen_scrollup(void);
void screen_backspace(void);
void screen_newline(void);