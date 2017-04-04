#include "acllib.h"
#include <stdio.h>
#include <math.h>

#define MAX_STR_LEN 10004
#define _CRT_SECURE_NO_WARNINGS

typedef int bool;
enum bool_
{
	false,
	true
};

enum Operators
{
	NUMBER,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	POWER,
	SIN,
	COS,
	LEFT_BRACKET,
	RIGHT_BRACKET
};
double data_stack[MAX_STR_LEN];
int stack[MAX_STR_LEN];
int stack_top;
char str[MAX_STR_LEN];

double decimal_unit[] = { 1, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9, 1e-10, 1e-11 };

#define EPS 1e-11

double power(double x, double y)
{
	return pow(x, y);
}

char current_str[MAX_STR_LEN];
int current_pos;
int total_len;

void preprocess()
{
	char c = getchar();
	str[++total_len] = '(';
	while (c != '\n')
	{
		if (c != ' ')
		{
			str[++total_len] = c;
		}
		c = getchar();
	}
	str[++total_len] = ')';
}

double work(double x_value)
{
	double last_number = 0.0;
	bool number_end = true;
	bool decimal = false;
	int last_decimal = 0;
	int i;
	stack_top = 0;

	bool syntactic_error = false;

	for (i = 1; (i <= total_len) && !syntactic_error; ++i)
	{
		char temp = str[i];

		if (temp == 's' || temp == 'c' || temp == 't')
		{
			int j = i + 4;
			while (str[j] != ')')j++;
			double temp_value;
			if ((j == i + 5) && (str[j - 1] == 'x'))
			{
				temp_value = x_value;
			}
			else
			{
				sscanf_s(str + i + 4, "%lf", &temp_value);
			}
			if (temp == 's') temp_value = sin(temp_value);
			if (temp == 'c') temp_value = cos(temp_value);
			if (temp == 't') temp_value = tan(temp_value);
			number_end = false;
			last_number = temp_value;
			i = j;
		}
		else if(temp == 'x')
		{
			number_end = false;
			last_number = x_value;
		}
		else if (temp >= '0' && temp <= '9')
		{
			if (!number_end)
			{
				if (decimal)
				{
					if (last_decimal <= 11)
					{
						last_number += decimal_unit[last_decimal + 1] * (temp - '0');
						last_decimal++;
					}
				}
				else
				{
					last_number = last_number * 10 + temp - '0';
				}
			}
			else
			{
				number_end = false;
				last_number = temp - '0';
			}
		}
		else if (temp == '.')
		{
			if (!number_end)
			{
				if (decimal)
				{
					puts("syntactic error");
					syntactic_error = true;
					break;
				}
				decimal = true;
			}
			else
			{
				number_end = false;
				decimal = true;
				last_number = true;
			}
		}
		else if (temp == '(')
		{
			if (!number_end)
			{
				puts("syntactic error");
				syntactic_error = true;
				break;
			}
			stack[++stack_top] = LEFT_BRACKET;
		}

		else
		{
			if ((stack[stack_top] == NUMBER) && temp == ')' && (stack[stack_top - 1] == LEFT_BRACKET))
			{
				stack[stack_top - 1] = NUMBER;
				data_stack[stack_top - 1] = data_stack[stack_top];
				stack_top--;
				continue;
			}
			if ((stack[stack_top] != NUMBER) && !number_end)
			{
				stack[++stack_top] = NUMBER;
				data_stack[stack_top] = last_number;

				last_number = 0.0;
				number_end = true;
				decimal = false;
				last_decimal = 0;
			}
			if (temp == ')')
			{

				while ((!syntactic_error) && (stack_top > 2) && (stack[stack_top - 1] != LEFT_BRACKET))
				{
					double temp_num = data_stack[stack_top--];
					int oper = stack[stack_top--];
					double another_num = data_stack[stack_top--];

					if (oper == POWER)
					{
						data_stack[++stack_top] = power(another_num, temp_num);
						stack[stack_top] = NUMBER;
					}
					else if (oper == MULTIPLY)
					{
						data_stack[++stack_top] = temp_num * another_num;
						stack[stack_top] = NUMBER;
					}
					else if (oper == DIVIDE)
					{
						if ((another_num > -EPS) && (another_num < EPS))
						{
							puts("error: divide by 0");
							syntactic_error = true;
							break;
						}
						data_stack[++stack_top] = temp_num / another_num;
						stack[stack_top] = NUMBER;
					}
					else if (oper == PLUS)
					{
						data_stack[++stack_top] = temp_num + another_num;
						stack[stack_top] = NUMBER;
					}
					else if (oper == MINUS)
					{
						data_stack[++stack_top] = temp_num - another_num;
						stack[stack_top] = NUMBER;
					}
				}
				if (stack[stack_top - 1] == LEFT_BRACKET)
				{
					stack[stack_top - 1] = NUMBER;
					data_stack[stack_top - 1] = data_stack[stack_top];
					stack_top--;
				}
				else
				{
					puts("error: loss bracket");
					syntactic_error = true;
					break;
				}
			}
			else
			{
				if (temp == '+' || temp == '-')
				{
					while ((!syntactic_error) && (stack_top > 2) && (stack[stack_top - 1] != LEFT_BRACKET))
					{
						double temp_num = data_stack[stack_top--];
						int oper = stack[stack_top--];
						double another_num = data_stack[stack_top--];

						if (oper == POWER)
						{
							data_stack[++stack_top] = power(another_num, temp_num);
							stack[stack_top] = NUMBER;
						}
						else if (oper == MULTIPLY)
						{
							data_stack[++stack_top] = temp_num * another_num;
							stack[stack_top] = NUMBER;
						}
						else if (oper == DIVIDE)
						{
							if ((another_num > -EPS) && (another_num < EPS))
							{
								puts("error: divide by 0");
								syntactic_error = true;
								break;
							}
							data_stack[++stack_top] = temp_num / another_num;
							stack[stack_top] = NUMBER;
						}
						else if (oper == PLUS)
						{
							data_stack[++stack_top] = temp_num + another_num;
							stack[stack_top] = NUMBER;
						}
						else if (oper == MINUS)
						{
							data_stack[++stack_top] = temp_num - another_num;
							stack[stack_top] = NUMBER;
						}
					}
					stack[++stack_top] = (temp == '+') ? PLUS : MINUS;
				}
				else if (temp == '*' || temp == '/')
				{
					while ((!syntactic_error) && (stack_top > 2) && (stack[stack_top - 1] != LEFT_BRACKET))
					{
						double temp_num = data_stack[stack_top--];
						int oper = stack[stack_top--];
						double another_num = data_stack[stack_top--];
						if (oper == POWER)
						{
							data_stack[++stack_top] = power(another_num, temp_num);
							stack[stack_top] = NUMBER;
						}
						else if (oper == MULTIPLY)
						{
							data_stack[++stack_top] = temp_num * another_num;
							stack[stack_top] = NUMBER;
						}
						else if (oper == DIVIDE)
						{
							if ((another_num > -EPS) && (another_num < EPS))
							{
								puts("error: divide by 0");
								syntactic_error = true;
								break;
							}
							data_stack[++stack_top] = temp_num / another_num;
							stack[stack_top] = NUMBER;
						}
						else
						{
							data_stack[++stack_top] = another_num;
							stack[++stack_top] = oper;
							data_stack[++stack_top] = temp_num;
							break;
						}
					}
					stack[++stack_top] = (temp == '*') ? MULTIPLY : DIVIDE;
				}
				else if (temp == '^')
				{
					while ((!syntactic_error) && (stack_top > 2) && (stack[stack_top - 1] != LEFT_BRACKET))
					{
						double temp_num = data_stack[stack_top--];
						int oper = stack[stack_top--];
						double another_num = data_stack[stack_top--];
						if (oper == POWER)
						{
							data_stack[++stack_top] = power(another_num, temp_num);
							stack[stack_top] = NUMBER;
						}
					}
					stack[++stack_top] = POWER;
				}
			}
		}
	}
	if (!syntactic_error)
	{
		if (stack_top != 1 || stack[stack_top] != NUMBER)
		{
			puts("syntactic error");
		}
		return data_stack[stack_top--];
	}
	return NAN;
}


int Setup()
{
	initWindow("window", DEFAULT, DEFAULT, 1280, 720);
	

	initConsole();
	printf("请不要输入无意义的括号和空格\n");
	printf("输入你要计算的多项式：f(x) = ");
	preprocess();	
	printf("然后输入比例尺（单位长度的像素个数）：");
	double scale = 50.0;
	scanf_s("%lf", &scale);
	//printf("%s\n", str + 1);

	printf("f(1) = %lf\n", work(1.0));

	beginPaint();
	line(50, 360, 1230, 360);
	line(1230, 360, 1223, 363);
	line(1230, 360, 1223, 357);
	line(640, 10, 640, 690);
	line(640, 10, 637, 17);
	line(640, 10, 643, 17);
	//setTextSize(100);
	//paintText(100, 100, str);

	int left_edge = 140;
	int right_edge = 1140;


	setTextSize(20);
	paintText(650, 370, "O");
	paintText(1210, 370, "x");
	paintText(650, 20, "y");

	// line(740, 362, 740, 358);

	ellipse(640 + scale - 3, 357, 640 + scale + 3, 363);
	paintText(640 + scale, 370, "1");

	setPenColor(BLUE);

	double y_lim = 350/scale;

	for(int i = left_edge; i <= right_edge; i++)
	{
		double x_value = (i - 640.0) / scale;
		double y_value = work(x_value);
		double next_x_value = (i + 1.0 - 640.0) / scale;
		double next_y_value = work(next_x_value);
		if(next_y_value > y_lim || next_y_value < -y_lim || y_value > y_lim || y_value < -y_lim)
		{
			continue;
		}
		int real_y = 360 - (int)(y_value * scale);
		int real_next_y = 360 - (int)(next_y_value * scale);
		line(i, real_y, i + 1, real_next_y);
	}
	endPaint();
	return 0;
}