
#include "Leetcode.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

/*
给出一个 32 位的有符号整数，你需要将这个整数中每位上的数字进行反转。

示例 1:

输入: 123
输出: 321
 示例 2:

输入: -123
输出: -321
示例 3:

输入: 120
输出: 21

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/reverse-integer
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/
int reverse(int x)
{
	char out[32] = "0";
	char out2[32] = "0";
	char fuHao = -1;
	int j = 0;

	//itoa(x, out, 10); //leetcode 不识别这个函数。
	//snprintf(s, sizeof(s), "%d", i);
	memset(out2, 0, 32);

	//printf("%c\n", out[0]);
	if (out[0] < '0' || out[0] > '9')
	{
		fuHao = out[0];
	}
	if (fuHao != '-')
	{
		for (int i = strlen(out) - 1; i >= 0; i--)
		{
			out2[j] = out[i];
			j++;
		}
	}
	else
	{
		j++;
		out2[j] = fuHao;
		for (int i = strlen(out) - 2; i >= 0; i--)
		{
			out2[j] = out[i];
			j++;
		}
	}

	if (out2[0] == '0')
	{
		for (int j = 0; j < strlen(out2); j++)
		{
			out2[j] = out2[j + 1];
		}
	}

	cout << out2 << endl;
	return 1;
}

// //167. Two Sum II - Input array is sorted(Easy)
// int *twoSum(int numbers[5], int target)
// {
// 	if (numbers == NULL)
// 		return NULL;
// 	int i = 0, j = 5;
// 	while (i < j)
// 	{
// 		int sum = numbers[i] + numbers[j];
// 		if (sum == target)
// 		{
// 			return new int[0]{i + 1, j + 1};
// 		}
// 		else if (sum < target)
// 		{
// 			i++;
// 		}
// 		else
// 		{
// 			j--;
// 		}
// 	}
// 	return NULL;
// }

ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
{
	ListNode *p = l1;
	ListNode *q = l2;
	ListNode *head_new = NULL, *p_head_new = NULL;
	int if_need_add = 0;
	if (p == NULL && q == NULL)
	{
		printf("all is NULL\n");
		return NULL;
	}

	p_head_new = head_new;

	while (p != NULL || q != NULL)
	{

		if (p == NULL && q != NULL)
		{
			if (p_head_new == NULL) //l1 is null
			{
				p_head_new = (ListNode *)malloc(sizeof(ListNode));
				memset(p_head_new, 0, sizeof(ListNode));
				p_head_new->val = q->val;
				p_head_new->next = NULL;
				head_new = p_head_new;
			}
			else
			{
				p_head_new->next = (ListNode *)malloc(sizeof(ListNode));
				memset(p_head_new->next, 0, sizeof(ListNode));
				p_head_new->next->val = q->val + if_need_add;
				if(p_head_new->next->val >= 10) p_head_new->next->val = p_head_new->next->val -10;
				else if_need_add = 0;
				p_head_new->next->next = NULL;
				p_head_new = p_head_new->next;
			}
			q = q->next;
		}
		else if (p != NULL && q == NULL)
		{
			if (p_head_new == NULL) //l2 is null
			{
				p_head_new = (ListNode *)malloc(sizeof(ListNode));
				p_head_new->val = p->val;
				p_head_new->next = NULL;
				head_new = p_head_new;
			}
			else
			{
				p_head_new->next = (ListNode *)malloc(sizeof(ListNode));
				memset(p_head_new->next, 0, sizeof(ListNode));
				p_head_new->next->val = p->val + if_need_add;
				if(p_head_new->next->val >= 10) p_head_new->next->val = p_head_new->next->val -10;
				else if_need_add = 0;
				p_head_new->next->next = NULL;
				p_head_new = p_head_new->next;
			}
			p = p->next;
		}
		else
		{
			if (p_head_new == NULL)
			{
				p_head_new = (ListNode *)malloc(sizeof(ListNode));
				head_new = p_head_new;
				memset(p_head_new, 0, sizeof(ListNode));
				head_new->next = NULL;

				p_head_new->val = p->val + q->val;
				if (p_head_new->val >= 10)
				{
					if_need_add = 1;
					p_head_new->val = p_head_new->val - 10;
				}

				p = p->next;
				q = q->next;
			}
			else
			{
				p_head_new->next = (ListNode *)malloc(sizeof(ListNode));
				memset(p_head_new->next, 0, sizeof(ListNode));
				p_head_new->next->next = NULL;

				p_head_new->next->val = p->val + q->val + if_need_add;
				if (p_head_new->next->val >= 10)
				{
					p_head_new->next->val = p_head_new->next->val - 10;
					if_need_add = 1;
				}
				else
				{
					if_need_add = 0;
				}
				p_head_new = p_head_new->next;
				p = p->next;
				q = q->next;
			}
		}
	}
	if (if_need_add == 1)
	{
		p_head_new->next = (ListNode *)malloc(sizeof(ListNode));
		memset(p_head_new->next, 0, sizeof(ListNode));
		p_head_new->next->next = NULL;

		p_head_new->next->val = 1;
	}
	return head_new;
}
