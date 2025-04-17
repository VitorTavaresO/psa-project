#ifndef OPERATIONS_H
#define OPERATIONS_H

#define MAX_SIZE 1000000

extern int numbers[MAX_SIZE];
extern int filtered[MAX_SIZE];
extern long long squared[MAX_SIZE];

void filterEvenNumbers(int *input, int inputSize, int *output, int *outputSize);
void squareNumbers(int *input, int inputSize, long long *output);
double calculateAverage(long long *input, int inputSize);
void processData(int *input, int inputSize);
int readNumbersFromFile(const char *filename);

#endif