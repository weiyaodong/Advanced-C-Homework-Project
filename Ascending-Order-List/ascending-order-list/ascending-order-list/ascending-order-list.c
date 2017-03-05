#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

struct ListNode 
{
	int value;

	struct ListNode* next_node;
};

typedef struct ListNode* list_node_pointer;

list_node_pointer get_new_list_node()
{
	return (list_node_pointer)malloc(sizeof(struct ListNode));
}


list_node_pointer insert_list(list_node_pointer p, int num)
{
	list_node_pointer temp = get_new_list_node();
	temp->value = num;
	temp->next_node = NULL;
	p->next_node = temp;
	return temp;
}

list_node_pointer create_list()
{
	list_node_pointer head = get_new_list_node();
	list_node_pointer p = get_new_list_node();
	int num;
	scanf("%d", &num);
	if (num == -1)
		return NULL;

	head->value = num;
	head->next_node = NULL;

	p = head;
	scanf("%d", &num);

	while (num != -1) 
	{
		p = insert_list(p, num);
		scanf("%d", &num);
	}

	p->next_node = NULL;
	return head;
}

list_node_pointer link_list(list_node_pointer head1, list_node_pointer head2)
{
	list_node_pointer result = get_new_list_node();
	list_node_pointer node_1;
	list_node_pointer node_2;

	node_1 = head1;
	node_2 = head2;
	
	list_node_pointer p3 = get_new_list_node();

	if (node_1 == NULL)
		return node_2;

	if (node_2 == NULL)
		return node_1;

	if (node_1->value < node_2->value) 
	{
		result->value = node_1->value;
		node_1 = node_1->next_node;
	}
	else 
	{
		result->value = node_2->value;
		node_2 = node_2->next_node;
	}

	p3 = result;

	while (node_1 != NULL || node_2 != NULL) 
	{
		if (node_2 == NULL) 
		{
			p3 = insert_list(p3, node_1->value);
			node_1 = node_1->next_node;
		}
		else if (node_1 == NULL) 
		{
			p3 = insert_list(p3, node_2->value);
			node_2 = node_2->next_node;
		}
		else if (node_1->value <= node_2->value) 
		{
			p3 = insert_list(p3, node_1->value);
			node_1 = node_1->next_node;
		}
		else 
		{
			p3 = insert_list(p3, node_2->value);
			node_2 = node_2->next_node;
		}
	}

	p3->next_node = NULL;
	return result;
}

void print_result(list_node_pointer result)
{
	list_node_pointer p = get_new_list_node();
	p = result;

	printf("%d", result->value);

	p = p->next_node;
	while (p != NULL) {
		printf(" %d", p->value);
		p = p->next_node;
	}
}

int main()
{
	list_node_pointer a = get_new_list_node();
	list_node_pointer b = get_new_list_node();

	a = create_list();
	b = create_list();

	puts("");

	if (a == NULL && b == NULL)
	{
		printf("NULL\n");
		return 0;
	}

	list_node_pointer c = get_new_list_node();
	c = link_list(a, b);

	print_result(c);
	return 0;
}