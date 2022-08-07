/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-07-18 14:08:12
 * @LastEditors: ruog__
 * @LastEditTime: 2022-07-21 10:26:59
 */


//https://leetcode.cn/problems/median-of-two-sorted-arrays/
// 4. 寻找两个正序数组的中位数
// 给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。

// 算法的时间复杂度应该为 O(log (m+n)) 。

 

// 示例 1：

// 输入：nums1 = [1,3], nums2 = [2]
// 输出：2.00000
// 解释：合并数组 = [1,2,3] ，中位数 2
// 示例 2：

// 输入：nums1 = [1,2], nums2 = [3,4]
// 输出：2.50000
// 解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5
#include "main.h"

#if 1  //从未见过如此奇葩代码，想想怎么优化。
double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size){
    
    int num = nums1Size + nums2Size;
    int *arr = (int *)malloc(sizeof(int)*num);
    if(arr == NULL) return 0.0;
    memset(arr,0,num);
    int a = 0,n2 = 0;

    //memcpy(arr,nums1,nums1Size);
    for(int i=0;i<nums1Size;i++)
    {
        for(;n2<nums2Size;n2++)
        {
            if(nums1[i] > nums2[n2])
            {
                arr[a] = nums2[n2];
                a++;
            }
            else break;
        }
        arr[a] = nums1[i];
        a++;
    }
    if(n2 < nums2Size)//说明nums2还有后续的数字未赋值
    {
        for(int i=n2; i<nums2Size; i++)
        {
            arr[a] = nums2[i];
            a++;
        }
    }

    double re = 0.0;
    if(a %2 ==0)
        re = (arr[(a/2)-1] + arr[(a/2)]) /2.0;
    else
        re = arr[a/2];
    return re;
}

#endif