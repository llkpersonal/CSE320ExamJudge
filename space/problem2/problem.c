#include <stdio.h>

double problem2(double *array, int cnt);

double array[20];
int i, N;

int main() {
  scanf("%d", &N);
  for (i = 0; i < N; i++) {
    scanf("%lf", &array[i]);
  }

  printf("%f\n", problem2(array, N));
  return 0;
}
