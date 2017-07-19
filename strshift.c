/**
字符串为*号和26个字母、阿拉伯数字的任意组合，把*号都移动到最左侧，把其他字母和数字都移到最右侧并保持相对顺序不变，返回字符*的个数，要求时间复杂度O(n)和空间复杂度O(1)。
*/
#include <stdio.h>
#include <string.h>

#define TEST_CASE(arr)                                                         \
  do {                                                                         \
    int count = strshift(arr, strlen(arr));                                    \
    printf("%d : %s\n", count, arr);                                           \
  } while (0)

int strshift(char arr[], int n) {
  int i;
  int count = 0;
  for (i = n - 1; i >= 0; i--) {
    if (arr[i] == '*') {
      count++;
    } else if (count > 0) // 根据右边子串*的个数可以确定字母往右移动多少
      arr[i + count] = arr[i];
  }
  for (i = 0; i < count; i++)
    arr[i] = '*';
  return count;
}

int main(int argc, char const *argv[]) {
  char arr1[] = "*abc**de***f**";
  char arr2[] = "*abc*de**fg";
  char arr3[] = "abc*de*f***";
  char arr4[] = "***abc*de*f*g**";

  TEST_CASE(arr1);
  TEST_CASE(arr2);
  TEST_CASE(arr3);
  TEST_CASE(arr4);

  return 0;
}
