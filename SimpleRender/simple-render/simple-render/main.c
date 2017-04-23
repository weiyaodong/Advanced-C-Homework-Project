#include "acllib.h"
#include <math.h>

void drawLine(int sx, int sy, int tx, int ty, ACL_Color color)
{
	beginPaint();
	int xstep, ystep, error, index;
	int x = sx;
	int y = sy;
	int dx = tx - sx;
	int dy = ty - sy;

	if (dx >= 0) // �������һ�  
	{
		xstep = 1; // x������1  
	}
	else // ��������  
	{
		xstep = -1; // x������1  
		dx = -dx; // ȡ����ֵ  
	}

	if (dy >= 0) // �������»�  
	{
		ystep = 1; // y������1  
	}
	else // �������ϻ�  
	{
		ystep = -1; // y������1  
		dy = -dy; // ȡ����ֵ  
	}

	int dx2 = dx << 1; // 2 * dx  
	int dy2 = dy << 1; // 2 * dy  

	if (dx > dy) // ��X��ֱ��  
	{
		error = dy2 - dx;
		for (index = 0; index <= dx; ++index)
		{
			putPixel(x, y, color);
			if (error >= 0)
			{
				error -= dx2;
				y += ystep;
			}
			error += dy2;
			x += xstep;
		}
	}
	else // ��Y��ֱ��  
	{
		error = dx2 - dy;
		for (index = 0; index <= dy; ++index)
		{
			putPixel(x, y, color);
			if (error >= 0)
			{
				error -= dy2;
				x += xstep;
			}
			error += dx2;
			y += ystep;
		}
	}
	endPaint();
}

int Setup()
{
	initWindow("title", DEFAULT, DEFAULT, 960, 640);

	drawLine(0, 0, 100, 100, WHITE);


	return 0;
}