#include "acllib.h"
#include <stdio.h>

#define max_buffer_size 10005

char buffer[max_buffer_size];
int buffer_size;

void buffer_clear()
{
	buffer_size = 1;
	buffer[buffer_size - 1] = '\0';
}

void buffer_insert(int pos, char charactor)
{
	for(int i = buffer_size; i >= pos; i--)
	{
		buffer[i] = buffer[i - 1];
	}
	buffer[pos] = charactor;
	buffer[++buffer_size] = '\0';
}

void buffer_erase(int pos)
{
	for(int i = pos - 1; i < buffer_size; i++)
	{
		buffer[i] = buffer[i + 1];
	}
	buffer[--buffer_size] = '\0';
}

#define TOP_OFFSET 20
#define LEFT_OFFSET 20

#define TEXT_HEIGHT 20
#define TEXT_WIDTH 10

#define X_MAX 50
#define Y_MAX 36

#define POS_MAP(x, y) (x)*TEXT_WIDTH+LEFT_OFFSET,(y)*TEXT_HEIGHT+TOP_OFFSET


int caps_lock = 0;

int caret_pos_x;
int caret_pos_y;
int caret_pos; // caret_pos is in [0, buffer_size]


void paint()
{
	beginPaint();

	setPenColor(WHITE);
	setBrushColor(WHITE);
	rectangle(0, 0, 1280, 720);

	int caret_x = 0;
	int caret_y = 0;

	char* temp = (char*)malloc(sizeof(char) * 2);
	temp[1] = '\0';
	for (int i = 0; i < buffer_size; i++)
	{
		if (buffer[i] != '\n')
		{
			temp[0] = buffer[i];
			paintText(POS_MAP(caret_x, caret_y), temp);
			if (caret_x == X_MAX)
			{
				caret_x = 0;
				caret_y++;
			}
			else caret_x++;			
		}
		else
		{
			caret_x = 0;
			caret_y++;
		}
	}

	free(temp);

	setCaretPos(POS_MAP(caret_pos_x,caret_pos_y));

	endPaint();
}

int get_caret_position_in_buffer()
{
	int caret_x = 0;
	int caret_y = 0;
	int temp = -1;

	for (int i = 0; i < buffer_size; i++)
	{
		if (caret_x == caret_pos_x && caret_y == caret_pos_y)
		{
			return i;
		}
		if (buffer[i] != '\n')
		{
			if (caret_x == X_MAX)
			{
				caret_x = 0;
				caret_y++;
			}
		}
		else
		{
			caret_x = 0;
			caret_y++;
		}
	}
	return buffer_size;
}

struct Pos
{
	int x, y;
};

typedef struct Pos pos_;

pos_ get_buffer_position_by_caret(int x)
{
	int caret_x = 0;
	int caret_y = 0;
	for (int i = 0; i < x; i++)
	{
		if (buffer[i] != '\n')
		{
			if (caret_x == X_MAX)
			{
				caret_x = 0;
				caret_y++;
			}
			else caret_x++;
		}
		else
		{
			caret_x = 0;
			caret_y++;
		}
	}
	return (pos_) { caret_x, caret_y };
}

#define CHAR(c) (char)(((c)-65)+'a'+caps_lock*('A'-'a'))

enum Direction
{
	FORWARD,
	BACKWARD
};

void move_caret(enum Direction flag)
{
	if(flag == FORWARD) // move to next position
	{
		if (caret_pos == buffer_size) return;
		if (buffer[caret_pos] == '\n' || caret_pos_x == X_MAX)
		{
			caret_pos_x = 0;
			caret_pos_y++;
		}
		else caret_pos_x++;
		caret_pos++;
	}
	if(flag == BACKWARD) // move to last position
	{
		if (caret_pos == 0) return;
		caret_pos--;
		pos_ temp = get_buffer_position_by_caret(caret_pos);
		caret_pos_x = temp.x;
		caret_pos_y = temp.y;
	}
}

void keyboard_event_callback_function(int key_code, int event)
{
	if (event == KEY_DOWN)
	{
		if (65 <= key_code && key_code <= 90) // A~Z
		{
			buffer_insert(caret_pos, CHAR(key_code));
			move_caret(FORWARD);
		}
		else if (key_code >= 48 && key_code <= 57) // 0~9
		{
			buffer_insert(caret_pos, (char)(key_code));
			move_caret(FORWARD);
		}
		else if (key_code >= 96 && key_code <= 105) // also 0~9
		{
			buffer_insert(caret_pos, (char)(key_code - 96 + '0'));
			move_caret(FORWARD);
		}
		else if (key_code <= 111 && key_code >= 106) // * + enter - . /
		{
			switch (key_code)
			{
			case 111:
				buffer_insert(caret_pos, '*');
				break;
			case 112:
				buffer_insert(caret_pos, '+');
				break;
			case 113:
				buffer_insert(caret_pos, '\n');
				break;
			case 114:
				buffer_insert(caret_pos, '-');
				break;
			case 115:
				buffer_insert(caret_pos, '.');
				break;
			default:
				buffer_insert(caret_pos, '/');
			}
			move_caret(FORWARD);
		}
		else if (key_code == 13) // enter
		{
			buffer_insert(caret_pos, '\n');
			move_caret(FORWARD);
		}
		else if (key_code == 8) // backspace
		{
			if(caret_pos != 0)
			{
				buffer_erase(caret_pos);
				move_caret(BACKWARD);
			}
		}
		else if(key_code == 37) // left arrow
		{
			move_caret(BACKWARD);
		}
		else if(key_code == 39) // right arrow
		{
			move_caret(FORWARD);
		}
		else if(key_code == 0x14) // capslock
		{
			caps_lock ^= 1;
		}
		else if(key_code == 0x2e) // delete
		{
			if(caret_pos != buffer_size)
			{
				buffer_erase(caret_pos + 1);
			}
		}
		else if(key_code == 0x24) // home
		{
			caret_pos -= caret_pos_x;
			caret_pos_x = 0;
		}
		else if(key_code == 0x23) // end
		{
			while(caret_pos != buffer_size)
			{
				move_caret(FORWARD);
				if(caret_pos_x == 0)
				{
					move_caret(BACKWARD);
					break;
				}
			}
		}
		else if(key_code == 0x20) // space
		{
			buffer_insert(caret_pos, ' ');
			move_caret(FORWARD);
		}
		paint();
	}
}

int Setup()
{
	initWindow("line_editor", DEFAULT, DEFAULT, 1280, 720);

	beginPaint();

	setCaretSize(1, 20);
	setTextSize(20);
	
	registerKeyboardEvent(keyboard_event_callback_function);

	showCaret();

	endPaint();

	return 0;
}