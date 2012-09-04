//keyboard
unsigned char keypressed(void);
void send_key(void);
	char read_key_buff(void);
	void write_key_buff(char data);
	int get_key(void);
	char get_code(int index);
void read_keyboard(void);
	unsigned char pinon(unsigned int port, unsigned char pin);
void send_bin_state(void);
	void pack(void);
void send_ascii_state(void);

//communications
void init_UART(unsigned char baudrate);
unsigned char receive_byte(void);
void transmit_byte(unsigned char data);

//alt		=1
//ctrl		=2
//shift		=3
//backspace	=8
const unsigned char keymap[] PROGMEM = { 
	'?', '>', '<', 1, 'm', 'n', 'b', 'v', 'c', 'x', 'z', ' ', 2,
	13, '\'', ';', 'l', 'k', 'j', 'h', 'g', 'f', 'd', 's', 'a', 3,
	'\\', ']', '[', 'p', 'o', 'i', 'u', 'y', 't', 'r', 'e', 'w', 'q',
	8, '=', '-', '0', '9', '8', '7', '6', '5', '4', '3', '2', '1' };