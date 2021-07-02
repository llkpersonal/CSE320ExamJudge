#include <stdio.h>

double problem1(double a, double b, double c);

int main() {
  double a, b, c;
  scanf("%lf%lf%lf", &a, &b, &c);
  printf("%f\n", problem1(a,b,c));
  return 0;
}
