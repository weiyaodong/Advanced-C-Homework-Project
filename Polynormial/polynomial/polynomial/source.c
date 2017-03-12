#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define new(TYPE) (TYPE*)malloc(sizeof(TYPE))

struct Polynomial
{
	int a;
	int b;

	struct Polynomial* next;
};

typedef struct Polynomial* polynomial_pointer;

typedef struct Polynomial Polynomial;

void Print(polynomial_pointer p1)
{
	while (p1 != NULL)
	{
		printf("(%d, %d)\n", p1->a, p1->b);
		p1 = p1->next;
	}
}

polynomial_pointer polynomial_plus(polynomial_pointer p1, polynomial_pointer p2)
{
	polynomial_pointer head = new(Polynomial);
	polynomial_pointer p;
	if (p1->b > p2->b)
	{
		head->b = p1->b;
		head->a = p1->a;
		p1 = p1->next;
	}
	else if (p1->b == p2->b)
	{
		head->b = p1->b;
		head->a = p1->a + p2->a;
		p1 = p1->next;
		p2 = p2->next;
	}
	else
	{
		head->b = p2->b;
		head->a = p2->a;
		p2 = p2->next;
	}
	p = head;
	while (p1->next != NULL || p2->next != NULL)
	{
		if (p1->b > p2->b)
		{
			p->next = new(Polynomial);
			p = p->next;
			p->b = p1->b;
			p->a = p1->a;
			p1 = p1->next;
		}
		else if (p1->b == p2->b)
		{
			p->next = new(Polynomial);
			p = p->next;
			p->b = p1->b;
			p->a = p1->a + p2->a;
			p1 = p1->next;
			p2 = p2->next;
		}
		else
		{
			p->next = new(Polynomial);
			p = p->next;
			p->b = p2->b;
			p->a = p2->a;
			p2 = p2->next;
		}
	}
	p->next = new(Polynomial);
	p = p->next;
	p->b = 0;
	p->a = p1->a + p2->a;
	p->next = NULL;
	return head;
}

polynomial_pointer create_polynomial()
{
	char* order[20] = {"first", "second", "third", "forth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth", "eleventh", "twelevth", "thirteenth", "fourteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "ninteenth", "twentith"};
	polynomial_pointer head = new(Polynomial);
	printf("Please input the first a: ");

	int coefficient;
	int power;

	scanf("%d", &coefficient);
	printf("Please input the first b: ");
	scanf("%d", &power);

	head->a = coefficient;
	head->b = power;

	polynomial_pointer p;
	p = head;
	int i = 1;

	while (power != 0)
	{
		p->next = new(Polynomial);
		p = p->next;
		printf("Please input the %s a: ", order[i]);
		scanf("%d", &coefficient);
		printf("Please input the %s b: ", order[i]);
		scanf("%d", &power);
		i++;
		p->a = coefficient;
		p->b = power;
	}
	if (p->b != 0)
	{
		p->next = new(Polynomial);
		p = p->next;
		p->a = 0;
		p->b = 0;
		p->next = NULL;
	}
	else
		p->next = NULL;

	return head;
}

polynomial_pointer polynomial_multiply(polynomial_pointer p1, polynomial_pointer p2)
{
	int greatest_power = p1->b + p2->b;

	polynomial_pointer head1 = p1;
	polynomial_pointer head2 = p2;
	polynomial_pointer head = new(Polynomial);

	int i;

	head->b = greatest_power;
	head->a = p1->a * p2->a;

	polynomial_pointer p;
	p = head;
	int sum;
	int f;
	int g;

	for (i = greatest_power - 1; i >= 0; --i)
	{
		sum = 0;
		f = 1;
		p1 = head1;
		while (p1->next != NULL)
		{
			if (f == 1)
				f = 0;
			else
				p1 = p1->next;
			g = 1;
			p2 = head2;
			while (p2->next != NULL)
			{
				if (g == 1)
					g = 0;
				else
					p2 = p2->next;
				if (p1->b + p2->b == i)
				{
					sum += (p1->a * p2->a);
				}
			}
		}
		if (sum != 0)
		{
			p->next = new(Polynomial);
			p = p->next;
			p->b = i;
			p->a = sum;
		}
	}
	p->next = NULL;
	return head;
}

void polynomial_neg(polynomial_pointer p)
{
	while (p != NULL)
	{
		p->a *= -1;
		p = p->next;
	}
}

polynomial_pointer polynomial_minus(polynomial_pointer p1, polynomial_pointer p2)
{
	polynomial_neg(p2);
	return polynomial_plus(p1, p2);
}

void show_ui()
{
	printf("Pleace choose the operation:\n");
	printf("Press 1 to add\n");
	printf("Press 2 to minus\n");
	printf("Press 3 to multiply\n");
	printf("Press 0 to quit\n");
	printf(">");
}

int main()
{
	polynomial_pointer p1 = NULL, p2 = NULL, p3 = NULL;
	while (1)
	{
		show_ui();

		int option;

		scanf("%d", &option);

		if (option == 0)
		{
			return 0;
		}
		while (option != 1 && option != 2 && option != 3)
		{
			printf("Maybe it is not a proper input, please try again.\n");
			show_ui();
			scanf("%d", &option);
			if (option == 0)
			{
				printf("Thanks for using!\n");
				return 0;
			}
		}

		p1 = create_polynomial();
		p2 = create_polynomial();

		switch (option)
		{
		case 1:
			p3 = polynomial_plus(p1, p2);
			break;
		case 2:
			p3 = polynomial_minus(p1, p2);
			break;
		case 3:
			p3 = polynomial_multiply(p1, p2);
			break;
		default:
			break;
		}

		Print(p3);
	}
}
