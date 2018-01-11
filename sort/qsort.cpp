#include <cstdio>
#include <cstring>

int arr[100001];
int n;

void swap(int &a, int &b) {
  int tmp = a;
  a = b;
  b = tmp;
}

int partition(int start, int end) {
  int index = start;
  // index = random(start, end);
  int left = start + 1;
  int right = end;
  while (1) {
    while (left < end && arr[left] <= arr[index])
      left++;
    while (right > start && arr[right] >= arr[index])
      right--;
    if (left < right) {
      swap(arr[left], arr[right]);
    } else
      break;
  }
  swap(arr[index], arr[right]);
  return right;
}

void qsort(int start, int end) {
  if (start == end)
    return;
  int pos = partition(start, end);
  if (start < pos)
    qsort(start, pos - 1);
  if (pos < end)
    qsort(pos + 1, end);
}

int main(int argc, char const *argv[]) {
  // freopen("qsort_test.txt", "r", stdin);
  while (scanf("%d", &n) != EOF) {
    for (int i = 0; i < n; i++) {
      scanf("%d", arr + i);
    }
    qsort(0, n - 1);
    for (int i = 0; i < n - 1; i++) {
      printf("%d ", arr[i]);
    }
    printf("%d\n", arr[n - 1]);
  }
  return 0;
}
