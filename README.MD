
# CSE320 Exam Auto Grading Program
## Introduction
This is a C programming to compile, run, test and grade the CSE320 final exam. It likes mimir, it will run the solutions from the students and detect if it runs beyond the time limit, and detect if the solution has a runtime error.

## Compile and Install
Just run make

## Code Structures
Folder c: the programs for generating answers for each task.

Folder header: header files.

Folder exam: to put the solutions.

## Usage
./main \<netid\> \<first_task_test_cases\> \<second_task_test_cases\> \[any_number\]

When you set the last parameter (any_number), it will not copy the solution from exam/\<netid\>/exam.S to space/program*/exam.S. It is used to debug for some solutions that is failure to compile.