#ifndef __LOADER_H_
#define __LOADER_H_

struct ProblemOneCase {
  char *name;
  double a, b, c;
  double answer;
};

struct ProblemTwoCase {
  char *name;
  int n;
  double *array;
  double answer;
};

struct ProblemThreeCase {
  char *name;
  char *str;
  int  answer;
};

struct ProblemOneCase problemOneCase(char *name, double a, double b, double c, double answer);
struct ProblemTwoCase problemTwoCase(char *name, int n, double *array, double answer);
struct ProblemThreeCase problemThreeCase(const char *name, char *str, int answer);

void outputProblemOne(struct ProblemOneCase* testcase);
void outputProblemTwo(struct ProblemTwoCase* testcase);
void outputProblemThree(struct ProblemThreeCase* testcase);

int judgeProblemOne(struct ProblemOneCase* testcase, double output);
int judgeProblemTwo(struct ProblemTwoCase* testcase, double output);
int judgeProblemThree(struct ProblemThreeCase* testcase, int output);

extern struct ProblemOneCase CasesOne[10];
extern struct ProblemTwoCase CasesTwo[10];
extern struct ProblemThreeCase CasesThree[10];

void loadTestCases(const char *username, const char* first, const char* second);

#endif
