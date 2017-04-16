#include "acllib.h"
#include <stdio.h>

int last_pos_x = -1;

int last_pos_y = -1;

int is_pressed;

void change_coordinate(int x, int y, ACL_Mouse_Button button, ACL_Mouse_Event event)
{
	if (button == NO_BUTTON)
	{
		return;
	}
	if (button == LEFT_BUTTON && event == BUTTON_DOWN)
	{
		is_pressed = 1;
	}
	if (button == LEFT_BUTTON && event == BUTTON_UP)
	{
		is_pressed = 0;
		last_pos_x = -1;
		last_pos_y = -1;
	}
	if (event == MOUSEMOVE)
	{
		beginPaint();
		setPenColor(WHITE);
		setBrushColor(WHITE);
		rectangle(0, 700, 1280, 720);
		setTextColor(BLUE);
		char str[1000];
		sprintf_s(str, 1000, "(%d,%d)", x, y);
		paintText(0, 700, str);
		if (is_pressed)
		{
			setPenColor(BLACK);
			if (last_pos_x == -1 && last_pos_y == -1)
			{
				last_pos_x = x;
				last_pos_y = y;
			}
			line(last_pos_x, last_pos_y, x, y);
			last_pos_x = x;
			last_pos_y = y;
		}
		endPaint();
	}
}

void custom_char_callback_function(int key)
{
	
}


int Setup()
{
	initWindow("test", DEFAULT, DEFAULT, 1280, 720);
	beginPaint();

	setTextSize(20);
	//registerMouseEvent(change_coordinate);

	setCaretSize(1, 20);
	setCaretPos(100, 100);

	showCaret();

	setTextSize(20);
	paintText(200, 200, "Courier New is the best font ever");


	endPaint();
	return 0;
}
