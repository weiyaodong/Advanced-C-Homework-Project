
#include <acllib.h>

#include <stdio.h>

int get_value(int x)
{
	return 0.01*x * x;
}

int Setup()
{
	initWindow("Test", DEFAULT, DEFAULT, 800, 600);

	initConsole();
	printf("Hello\n");

	beginPaint();

	//line(10, 10, 100, 100);
	for(int i = 1; i <= 400;i++)
	{
		int x = i - 200;
		line(i, get_value(x), i+ 1, get_value(x + 1));
	}
	endPaint();

	return 0;
}