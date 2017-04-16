#include <stdio.h>
#include "acllib.h"

int main(int argc, char* argv[])
{
	initWindow("test2", DEFAULT, DEFAULT, 1280, 720);
	
	beginPaint();

	setCaretSize(3, 10);
	setCaretPos(100, 100);

	showCaret();

	endPaint();

	return 0;
}
