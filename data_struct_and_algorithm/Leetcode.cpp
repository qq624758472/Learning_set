
#include "Leetcode.h"


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
	
	itoa(x, out, 10);  //leetcode 不识别这个函数。
	memset(out2, 0, 32);
	
	//printf("%c\n", out[0]);
	if (out[0] < '0' || out[0] > '9') {
		fuHao = out[0];
	}
	if (fuHao != '-') {
		for (int i = strlen(out) - 1; i >= 0; i--) {
			out2[j] = out[i];
			j++;
		}
	}
	else {
		j++;
		out2[j] = fuHao;
		for (int i = strlen(out) - 2; i >= 0; i--) {
			out2[j] = out[i];
			j++;
		}
	}

	if (out2[0] == '0') {
		for (int j = 0; j < strlen(out2); j++) {
			out2[j] = out2[j+1];
		}
	}
	
	cout << out2 << endl;
	return 1;
}




//167. Two Sum II - Input array is sorted(Easy)
int* twoSum(int numbers[5], int target) 
{
	if (numbers == NULL) return NULL;
	int i = 0, j = 5;
	while (i < j) {
		int sum = numbers[i] + numbers[j];
		if (sum == target) {
			return new int[] {i + 1, j + 1};
		}
		else if (sum < target) {
			i++;
		}
		else {
			j--;
		}
	}
	return NULL;
}