#include <stdio.h>
#include <stdlib.h>

int N;
double array[20];

int cmpfunc(const void *a, const void *b) {
  return *(double*)a - *(double*)b;
}

double problem2(double *array, int cnt) {
  if (cnt < 3) return 0;
  qsort(array, cnt, sizeof(double), cmpfunc);
  /*if (cnt & 1) {
    return array[cnt / 2];
  } else {
    return 0.5 * (array[cnt/2 - 1] + array[cnt/2]);
  }*/
  return 0.5 * (array[1] + array[cnt-2]);
}

int main() {
  int i;
  while (~scanf("%d", &N)) {
    for (i = 0; i < N; i++) {
      scanf("%lf", &array[i]);
    }
    
    printf("%f\n", problem2(array, N));
  }

  return 0;
}
