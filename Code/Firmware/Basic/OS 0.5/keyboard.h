*char get_all_keys();			//gets state of entire keyboard at moment
*char last_all_keys();			//gets state of entire keyboard from last time key was pressed
char get_key();					//waits for keypress. returns index of key
char last_key();				//gets the last key pressed
char decode_key(char index);	//returns ASCII code of key index