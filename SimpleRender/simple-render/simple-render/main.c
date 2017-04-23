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

	if (dx >= 0) // 从左往右画  
	{
		xstep = 1; // x步进正1  
	}
	else // 从右往左画  
	{
		xstep = -1; // x步进负1  
		dx = -dx; // 取绝对值  
	}

	if (dy >= 0) // 从上往下画  
	{
		ystep = 1; // y步进正1  
	}
	else // 从下往上画  
	{
		ystep = -1; // y步进负1  
		dy = -dy; // 取绝对值  
	}

	int dx2 = dx << 1; // 2 * dx  
	int dy2 = dy << 1; // 2 * dy  

	if (dx > dy) // 近X轴直线  
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
	else // 近Y轴直线  
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