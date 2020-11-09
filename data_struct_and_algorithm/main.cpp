#include "common.h"
#include "Data_struct.h"
#include "Leetcode.h"
#include <iostream>

using namespace std;

#define DEBUG

int main()
{

#ifdef DEBUG1
	reverse(1256);
#endif

#ifdef DEBUG2
	ListNode *l1 = NULL;
	ListNode *l2 = NULL;
	ListNode *p = NULL,*q = NULL;
	int tmp1[3] = {5};
	int tmp2[3] = {5};
	for (int i = 0; i < 4; i++)
	{
		if (l1 == NULL)
		{
			l1 = (ListNode *)malloc(sizeof(ListNode));
			l1->val = tmp1[i];
			l2 = (ListNode *)malloc(sizeof(ListNode));
			l2->val = tmp2[i];
			p=l1;
			q=l2;
		}
		else
		{
			p->next = (ListNode *)malloc(sizeof(ListNode));
			p->next->val = tmp1[i];
			q->next = (ListNode *)malloc(sizeof(ListNode));
			q->next->val = tmp2[i];
			p=p->next;
			q=q->next;
		}		
	}
	printf("l1:\n");
	printf_list(l1);
	printf("l2:\n");
	printf_list(l2);
	printf("\n");
	p = addTwoNumbers(l1,l2);
	printf("new:\n");
	printf_list(p);
#endif

#ifdef DEBUG
    // DirectInsert tmp;
    DirectInsert *pComm = new DirectInsert();
#endif

	getchar();
}

void printf_list(ListNode *l1)
{
	ListNode * p = l1;
	while(p!=NULL)
	{
		printf(" %d ",p->val);
		p=p->next;
	}
}