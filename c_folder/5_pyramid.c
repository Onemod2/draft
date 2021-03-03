#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int*, int*);

void insert(int* heap, size_t size, int el);

int extract(int* heap, size_t size);


int main() {

  srand(time(0));

  int n;
  printf("Print size of massive\n");
  scanf("%d", &n);

  int* m = (int*) malloc (sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    int val;
    scanf("%d", &val);

    insert(m, i, val);
  }
  printf("\n");

  for (int i = 0; i < n; i++) {
    printf("%d ", extract(m, n-i));
  }
  printf("\n");

  free(m);

  return 0;
}


void swap(int* a, int* b) {
  int c = *a;
  *a = *b;
  *b = c;
}

void insert(int* heap, size_t size, int el) {
  heap[size] = el;
  size++;

  int i = size-1;

  while (i > 0 && heap[i] < heap[(i-1) / 2]) {
    swap(&heap[i], &heap[(i-1)/2]);
    i = (i-1) / 2;
  }
}

int extract(int* heap, size_t size) {
  int val = heap[0];

  heap[0] = heap[size - 1];
  size--;

  int i = 0;
  while (2 * i + 1 < size) {
    int j = i;

    if (heap[i*2 + 1] < heap[i]) {
      j = i*2 + 1;
    }
    if (2 * i + 2 < size && heap[i*2 + 2] < heap[j]) {
      j = i*2 + 2;
    }
    if (j == i) {
      return val;
    }
    swap(&heap[i], &heap[j]);

    i = j;
  }
  return val;

}
