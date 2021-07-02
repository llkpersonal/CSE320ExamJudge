#include "header/loader.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

struct ProblemOneCase CasesOne[10];
struct ProblemTwoCase CasesTwo[10];
struct ProblemThreeCase CasesThree[10];

struct ProblemOneCase problemOneCase(char *name, 
                                     double a, 
                                     double b, 
                                     double c, 
                                     double answer) {
  struct ProblemOneCase res;
  int len = strlen(name);
  res.name = (char*)malloc(len+1);
  memset(res.name, 0, sizeof(char)*(len+1));
  strcpy(res.name, name);
  res.a = a;
  res.b = b;
  res.c = c;
  res.answer = answer;
  return res;
}

struct ProblemTwoCase problemTwoCase(char *name, 
                                     int n, 
                                     double *array, 
                                     double answer) {
  struct ProblemTwoCase res;
  int len = strlen(name);
  int i;
  res.name = (char*)malloc(len+1);
  memset(res.name, 0, sizeof(char)*(len+1));
  strcpy(res.name, name);
  res.array = (double*)malloc(sizeof(double)*n);
  res.n = n;
  for (i = 0; i < n; i++) {
    res.array[i] = array[i];
  }
  res.answer = answer;
  return res;
}

struct ProblemThreeCase problemThreeCase(const char *name, 
                                         char *str, 
                                         int answer) {
  struct ProblemThreeCase res;
  int len = strlen(name);
  res.name = (char*)malloc(len+1);
  memset(res.name, 0, sizeof(char)*(len+1));
  strcpy(res.name, name);
  len = strlen(str);
  res.str = (char*)malloc(len+1);
  memset(res.str, 0, sizeof(char)*(len+1));
  strcpy(res.str, str);
  res.answer = answer;
  return res;
}

void outputProblemOne(struct ProblemOneCase* testcase) {
  printf("RUNNING CASE %s: ", testcase->name);
  printf("a = %.2f, b = %.2f, c = %.2f", testcase->a, testcase->b, testcase->c);
  printf(", expected = %.2f", testcase->answer);
}

void outputProblemTwo(struct ProblemTwoCase* testcase) {
  int i;
  printf("RUNNING CASE %s:", testcase->name);
  printf(" [");
  for (i = 0; i < testcase->n; i++) {
    printf(i == 0 ? "%.2f": ", %.2f", testcase->array[i]);
  }
  printf("], expected = %.2f", testcase->answer);
}

void outputProblemThree(struct ProblemThreeCase* testcase) {
  printf("RUNNING CASE %s: ", testcase->name);
  printf("'%s'", testcase->str);
  printf(", expected = %d", testcase->answer);
}

int judgeProblemOne(struct ProblemOneCase* testcase, double output) {
  if (fabs(output - testcase->answer) < 1e-8) {
    return 1;
  }

  return 0;
}

int judgeProblemTwo(struct ProblemTwoCase* testcase, double output) {
  if (fabs(output - testcase->answer) < 1e-8) {
    return 1;
  }

  return 0;
}

int judgeProblemThree(struct ProblemThreeCase* testcase, int output) {
  return testcase->answer == output;
}

void loadTestCases(const char *username, const char* first, const char* second) {
  FILE* fp = fopen(first, "r");
  char name[20], title[20];
  double a, b, c, answer;
  double array[20];
  int index1 = 0, index2 = 0, index3 = 0, n, i;
  while(~fscanf(fp, "%s", title)) {
    // printf("TITLE: %s\n", title);
    if (!strcmp(title, "PROBLEM1:")) {
      // printf("LOADING PROBLEM1...\n");
      for (index1 = 0; index1 < 10; ++index1) {
        fgetc(fp);
        fgets(name, 20, fp);
        name[strlen(name) - 1] = '\0';
        // printf("NAME: %s\n", name);
        fscanf(fp, "%lf%lf%lf%lf", &a, &b, &c, &answer);
        CasesOne[index1] = problemOneCase(name, a, b, c, answer);
      }
    }
  }
  fclose(fp);
  fp = fopen(second, "r");
  while(~fscanf(fp, "%s", title)) {
    if (!strcmp(title, "PROBLEM2:")) {
      for (index2 = 0; index2 < 10; ++index2) {
        fgetc(fp);
        fgets(name, 20, fp);
        name[strlen(name) - 1] = '\0';
        // printf("NAME: %s\n", name);
        fscanf(fp, "%d", &n);
        for (i = 0; i < n; i++) {
          fscanf(fp, "%lf", &array[i]);
        }
        fscanf(fp, "%lf", &answer);
        CasesTwo[index2] = problemTwoCase(name, n, array, answer);
      }
    }
  }

  char teststr[100];
  // 1
  memset(teststr, 0, sizeof(teststr));
  CasesThree[index3] = problemThreeCase("EMPTY", teststr, 0);
  index3++;
  // 2
  strncpy(teststr, username, 4);
  CasesThree[index3] = problemThreeCase("SHORTER", teststr, 0);
  index3++;
  // 3
  memset(teststr, 0, sizeof(teststr));
  strcpy(teststr, username);
  for (i = 0; teststr[i] != '\0'; i++) {
    teststr[i] = toupper(teststr[i]);
  }
  CasesThree[index3] = problemThreeCase("UPPER", teststr, 0);
  index3++;
  // 4
  memset(teststr, 0, sizeof(teststr));
  strcpy(teststr, username);
  for (i = 0; i < 4; i++) {
    teststr[i] = toupper(teststr[i]);
  }
  CasesThree[index3] = problemThreeCase("PARTIAL UPPER", teststr, 0);
  index3++;
  // 5
  memset(teststr, 0, sizeof(teststr));
  strcpy(teststr, "12948");
  strcat(teststr, username);
  CasesThree[index3] = problemThreeCase("HEAD", teststr, 1);
  index3++;
  // 6
  memset(teststr, 0, sizeof(teststr));
  strcpy(teststr, username);
  CasesThree[index3] = problemThreeCase("EXACT", teststr, 1);
  index3++;
  // 7
  memset(teststr, 0, sizeof(teststr));
  strcpy(teststr, username);
  strcat(teststr, "213!");
  CasesThree[index3] = problemThreeCase("TAIL", teststr, 1);
  index3++;
  // 8
  memset(teststr, 0, sizeof(teststr));
  for (i = 0; i < 4; i++) {
    teststr[i] = username[i];
  }
  teststr[4] = ' ';
  for (i = 5; username[i - 1] != '\0'; i++) {
    teststr[i] = username[i-1];
  }
  CasesThree[index3] = problemThreeCase("SPACE", teststr, 0);
  index3++;
  // 9
  strcat(teststr, username);
  CasesThree[index3] = problemThreeCase("CONNECTION", teststr, 1);
  index3++;
  // 10
  memset(teststr, 0, sizeof(teststr));
  strcpy(teststr, "different");
  CasesThree[index3] = problemThreeCase("DIFF", teststr, 0);
  index3++;
}
