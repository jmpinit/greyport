#define REG_GRAM	0x22
#define REG_XPOS	0x21
#define REG_YPOS	0x20

void lcd_set_reg(int index);
void lcd_write_data(int val);
void lcd_write_reg(int index, int val);
void lcd_initialize(void);

void lcd_write_gram(int color);
void lcd_set_xpos(int x);
void lcd_set_ypos(int y);
void lcd_set_pos(int x, int y);
//void pixelMode();
//void charMode();