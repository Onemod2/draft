#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0
#define SIZE 11

// atoi with check (not number)

int scr_atoi(char ch);

void swap(char* a, char* b);

int prm(char* str) {
  int len = strlen(str);
  int i = len - 2, j;

  // 1st part of algorithm

  while (str[i] > str[i+1]) {
    i--;
    if (i == -1) {
      return 0;
    }
  }

  j = i+2;

  // 2nd part of algorithm

  while (j < len && str[j] > str[i]) {
    j++;
  }
  j--;
  swap(&str[i], &str[j]);

  // 3d part of algorithm

  for (int k = i+1; k < (len+i+1) / 2; k++) {
    swap(&str[k], &str[len - k + i]);
  }
  return 1;

}


int main() {

  int n;

  char* str = (char*)malloc(sizeof(char) * SIZE);
  scanf("%s%d", str, &n);
  int count[10] = {0};

  if (strlen(str) > 10) {
    printf("bad print\n");
    return 0;
  }
// 121456
// count[1]++;
  for (int i = 0; i < strlen(str); i++) {
    int num = scr_atoi(str[i]);
    if (num != -1 && count[num] != 1) {
      count[num]++;
    }
    else {
      printf("bad print");
      return 1;
    }
  }

// 1 permutation every time

  for (int i = 0; i < n; i++) {
    if (!prm(str)) {
      return 0;
    }
    printf("%s\n", str);

  }

  return 0;
}

int scr_atoi(char ch) {
  int num = ch - '0';

  if (num >= 0 && num < 10) {
    return num;
  }
  return -1;
}

void swap(char* a, char* b) {
  char c = *a;
  *a = *b;
  *b = c;
}



//https://en.ppt-online.org/28993
