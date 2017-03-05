#define _CRT_SECURE_NO_WARNING

#include <stdio.h>
#include <math.h>

#define MAX_STR_LEN 10004

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

int main(int argc, char* argv[])
{
	while (1)
	{
		char c = getchar();
		int total_len = 0;
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

		double answer = 0.0;
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

			if (temp >= '0' && temp <= '9')
			{
				if (!number_end)
				{
					if (decimal)
					{
						if (last_decimal <= 11)
						{
							last_number += decimal_unit[temp - '0'];
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
			else if(temp == '(')
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
				if ((stack[stack_top] == NUMBER) && temp == ')' && (stack[stack_top-1] == LEFT_BRACKET))
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
				continue;
			}
			printf("%.6lf\n", data_stack[stack_top--]);
		}
	}

	return 0;
}
