#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

#define MAX_SIZE 750000000

extern uint32_t *numbers;
extern uint32_t *filtered;
extern uint64_t *squared;

int initializeArrays(void);
void freeArrays(void);

void filterEvenNumbers(uint32_t *input, uint32_t inputSize, uint32_t *output, uint32_t *outputSize);
void squareNumbers(uint32_t *input, uint32_t inputSize, uint64_t *output);
double calculateAverage(uint64_t *input, uint32_t inputSize);
void processData(uint32_t *input, uint32_t inputSize);
int readNumbersFromFile(const char *filename);

#endif