/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-07-18 14:08:22
 * @LastEditors: ruog__
 * @LastEditTime: 2023-06-25 09:56:07
 */
#include <stdio.h>
#include "1.h"

int main()
{
    int nums1[2] = { 1, 2 };
    int nums2[2] = { 3, 4 };
    printf("%f\n", findMedianSortedArrays(nums1, 2, nums2, 2));
}