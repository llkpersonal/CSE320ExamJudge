#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "header/loader.h"
#include <sys/ptrace.h>
#include <signal.h>

char cmd[255], buff[255];
const char *CP[] = {"gcc", "-o", "main", "problem.c", "exam.S", NULL};

void prepare(const char* username) {
  int i;
  for (i = 1; i <=3; i++) {
    sprintf(cmd, "cp ./exam/%s/exam.S ./space/problem%d/exam.S", username, i);
    system(cmd);
  }
}

int compile(const char* taskname) {
  int pid, i, status;
  
  printf("COMPILING %s...", taskname);
  for (i = 0; taskname[i] != '\0'; i++) {
    buff[i] = tolower(taskname[i]);
  }
  buff[8] = '\0';

  pid = fork();
  if (pid == 0) {
    struct rlimit LIM;
    sprintf(cmd, "./space/%s", buff);
    chdir(cmd);
    LIM.rlim_max = 60;
    LIM.rlim_cur = 60;
    setrlimit(RLIMIT_CPU, &LIM);
    alarm(60);
    LIM.rlim_max = 100 * 1048576;
    LIM.rlim_cur = 100 * 1048576;
    setrlimit(RLIMIT_FSIZE, &LIM);

    freopen("compile_error.txt", "w", stderr);
    execvp(CP[0], (char* const*)CP);
    exit(0);
  } else {
    waitpid(pid, &status, 0);
    if (status == 0) printf("SUCCESS!\n");
    else printf("FAIL!\n");
    return status == 0; 
  }
  return 1;
}

void outputTestCases(int task, int caseId) {
  FILE *fp = fopen("test.txt", "w");
  int i;
  if (task == 1) {
    // printf("PRINT CASE #%d.\n", caseId);
    fprintf(fp, "%.2f %.2f %.2f", CasesOne[caseId].a, CasesOne[caseId].b, CasesOne[caseId].c);
  } else if (task == 2) {
    fprintf(fp, "%d\n", CasesTwo[caseId].n);
    for (i = 0; i < CasesTwo[caseId].n; i++) {
      fprintf(fp, "%.2f ", CasesTwo[caseId].array[i]);
    }
  } else {
    fprintf(fp, "%s", CasesThree[caseId].str);
  }
  fclose(fp);
}

int judge(int task, int caseId) {
  double answer;
  int matched;
  sprintf(cmd, "./space/problem%d/out.txt", task);
  FILE *fp = fopen(cmd, "r");
  if (task == 1) {
    fscanf(fp, "%lf", &answer);
    fclose(fp);
    printf(", actual = %.2f, ", answer);
    return judgeProblemOne(&CasesOne[caseId], answer);
  } else if (task == 2) {
    fscanf(fp, "%lf", &answer);
    fclose(fp);
    printf(", actual = %.2f, ", answer);
    return judgeProblemTwo(&CasesTwo[caseId], answer);
  } else {
    fscanf(fp, "%d", &matched);
    fclose(fp);
    printf(", actual = %d, ", matched);
    return judgeProblemThree(&CasesThree[caseId], matched);
  }
}

int run(int task) {
  int i, pid, status, grades = 0;
  printf("RUNNING PROBLEM%d:\n", task);
  for (i = 0; i < 10; i++) {
    pid = fork();
    if (pid == 0) {
      struct rlimit LIM;
      sprintf(cmd, "./space/problem%d", task);
      chdir(cmd);
      outputTestCases(task, i);
      freopen("test.txt", "r", stdin);
      freopen("out.txt", "w", stdout);
      freopen("error.txt", "a+", stderr);
      ptrace(PTRACE_TRACEME, 0, NULL, NULL);
      LIM.rlim_cur = 5;
      LIM.rlim_max = 5;
      setrlimit(RLIMIT_CPU, &LIM);
      alarm(0);
      alarm(50);
      execl("./main", "./main", (char*) NULL);
      exit(0);
    } else {
       if (task == 1) {
         outputProblemOne(&CasesOne[i]);
       } else if (task == 2) {
         outputProblemTwo(&CasesTwo[i]);
       } else if (task == 3) {
         outputProblemThree(&CasesThree[i]);
       }
       int sig, exitcode, flag = 1;
       struct rusage ruse;
       while(1) {
         wait4(pid, &status, 0, &ruse);
         // printf("status = %d ", status);
         if (WIFEXITED(status)) {
           break;
         }
         exitcode = WEXITSTATUS(status);
         if (exitcode && exitcode != 5) {
           switch (exitcode) {
             case SIGCHLD: case SIGALRM:
               alarm(0);
             case SIGKILL: case SIGXCPU:
               flag = 0;
               printf(" Time Limit Exeeded\n");
               break;
             default:
               flag = 0;
               printf(" Run Time Error\n");
           }
           ptrace(PTRACE_KILL, pid, NULL, NULL);
           break;
         }
         if (WIFSIGNALED(status)) {
           sig = WTERMSIG(status);
           switch (sig) {
             case SIGCHLD: case SIGALRM:
               alarm(0);
             case SIGKILL: case SIGXCPU:
               flag = 0;
               printf(" Time Limit Exeeded\n");
               break;
             default:
               flag = 0;
               printf(" Run Time Error\n");
           }
           ptrace(PTRACE_KILL, pid, NULL, NULL);
           break;
         }
         ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
       } // while
       if (flag) {
         int correct = judge(task, i);
         if (correct) {
           printf("CORRECT\n");
         } else {
           printf("WRONG\n");
         }
         grades += correct;
       }
    } // else
  } // for
  printf("GRADES: %d\n", grades);
  return grades;
}

void clear() {
  int i;
  for (i = 1; i <= 3; i++) {
    sprintf(cmd, "./space/problem%d/exam.S", i);
    remove(cmd);
    sprintf(cmd, "./space/problem%d/main", i);
    remove(cmd);
    sprintf(cmd, "./space/problem%d/test.txt", i);
    remove(cmd);
    sprintf(cmd, "./space/problem%d/out.txt", i);
    remove(cmd);
    sprintf(cmd, "./space/problem%d/error.txt", i);
    remove(cmd);
    sprintf(cmd, "./space/problem%d/compile_error.txt", i);
    remove(cmd);
  }
}

int main(int argc, char* argv[]) {
  int i, grades = 0;
  loadTestCases(argv[1], argv[2], argv[3]);
  if (argc < 5) prepare(argv[1]);
  if (compile("PROBLEM1"))
    grades += run(1);
  if (compile("PROBLEM2"))
    grades += run(2);
  if (compile("PROBLEM3"))
    grades += run(3);
  printf("TOTAL GRADES: %d\n", grades);
  clear();
  return 0;
}
