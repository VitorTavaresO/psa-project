#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "operations.h"

int numbers[MAX_SIZE];
int filtered[MAX_SIZE];
long long squared[MAX_SIZE];

void filterEvenNumbers(int *input, int inputSize, int *output, int *outputSize)
{
    *outputSize = 0;

    for (int i = 0; i < inputSize; i++)
    {
        if (input[i] % 2 == 0)
        {
            output[*outputSize] = input[i];
            (*outputSize)++;
        }
    }
}

void squareNumbers(int *input, int inputSize, long long *output)
{
    for (int i = 0; i < inputSize; i++)
    {
        long long val = (long long)input[i];
        output[i] = val * val;
    }
}

double calculateAverage(long long *input, int inputSize)
{
    if (inputSize == 0)
    {
        return 0.0;
    }

    long long sum = 0;
    for (int i = 0; i < inputSize; i++)
    {
        sum += input[i];
    }

    return (double)sum / inputSize;
}

void processData(int *input, int inputSize)
{
    if (inputSize > MAX_SIZE)
    {
        printf("Error: Input size exceeds maximum allowed size (%d)\n", MAX_SIZE);
        return;
    }

    clock_t start = clock();

    int filteredSize = 0;
    filterEvenNumbers(input, inputSize, filtered, &filteredSize);

    squareNumbers(filtered, filteredSize, squared);

    double average = calculateAverage(squared, filteredSize);

    clock_t end = clock();
    double executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Array size: %d\n", inputSize);
    printf("Number of even values: %d\n", filteredSize);

    if (inputSize <= 20)
    {
        printf("Original array: ");
        for (int i = 0; i < inputSize; i++)
        {
            printf("%d ", input[i]);
        }
        printf("\n");

        printf("Filtered array (even numbers): ");
        for (int i = 0; i < filteredSize; i++)
        {
            printf("%d ", filtered[i]);
        }
        printf("\n");

        printf("Squared values: ");
        for (int i = 0; i < filteredSize; i++)
        {
            printf("%d ", squared[i]);
        }
        printf("\n");
    }

    printf("Average of squared values: %.2f\n", average);
    printf("Execution time: %f seconds\n", executionTime);
}

int readNumbersFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    int count = 0;

    while (count < MAX_SIZE && fscanf(file, "%d", &numbers[count]) == 1)
    {
        count++;
    }

    if (!feof(file))
    {
        printf("Warning: Not all numbers were read. Maximum size reached (%d)\n", MAX_SIZE);
    }

    fclose(file);
    return count;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    int size = readNumbersFromFile(argv[1]);

    printf("--- Processing Array (%d elements) ---\n", size);
    processData(numbers, size);

    return 0;
}