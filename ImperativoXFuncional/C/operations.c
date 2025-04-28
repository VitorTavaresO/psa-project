#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "operations.h"

uint32_t *numbers = NULL;
uint32_t *filtered = NULL;
uint64_t *squared = NULL;

int initializeArrays(void)
{
    numbers = (uint32_t *)malloc(MAX_SIZE * sizeof(uint32_t));
    filtered = (uint32_t *)malloc(MAX_SIZE * sizeof(uint32_t));
    squared = (uint64_t *)malloc(MAX_SIZE * sizeof(uint64_t));

    if (numbers == NULL || filtered == NULL || squared == NULL)
    {
        printf("Error: Memory allocation failed\n");
        freeArrays();
        return 0;
    }
    return 1;
}

void freeArrays(void)
{
    if (numbers)
        free(numbers);
    if (filtered)
        free(filtered);
    if (squared)
        free(squared);

    numbers = NULL;
    filtered = NULL;
    squared = NULL;
}

void filterEvenNumbers(uint32_t *input, uint32_t inputSize, uint32_t *output, uint32_t *outputSize)
{
    *outputSize = 0;

    for (uint32_t i = 0; i < inputSize; i++)
    {
        if (input[i] % 2 == 0)
        {
            output[*outputSize] = input[i];
            (*outputSize)++;
        }
    }
}

void squareNumbers(uint32_t *input, uint32_t inputSize, uint64_t *output)
{
    for (uint32_t i = 0; i < inputSize; i++)
    {
        uint64_t val = (uint64_t)input[i];
        output[i] = val * val;
    }
}

double calculateAverage(uint64_t *input, uint32_t inputSize)
{
    if (inputSize == 0)
    {
        return 0.0;
    }

    uint64_t sum = 0;
    for (uint32_t i = 0; i < inputSize; i++)
    {
        sum += input[i];
    }

    return (double)sum / inputSize;
}

void processData(uint32_t *input, uint32_t inputSize)
{
    if (inputSize > MAX_SIZE)
    {
        printf("Error: Input size exceeds maximum allowed size (%u)\n", MAX_SIZE);
        return;
    }

    clock_t start = clock();

    uint32_t filteredSize = 0;
    filterEvenNumbers(input, inputSize, filtered, &filteredSize);

    squareNumbers(filtered, filteredSize, squared);

    double average = calculateAverage(squared, filteredSize);

    clock_t end = clock();
    double executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Array size: %u\n", inputSize);
    printf("Number of even values: %u\n", filteredSize);

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
        return -1;
    }

    uint32_t count = 0;

    if (!initializeArrays())
    {
        fclose(file);
        return -1;
    }

    while (count < MAX_SIZE && fscanf(file, "%u", &numbers[count]) == 1)
    {
        count++;
    }

    if (!feof(file))
    {
        printf("Warning: Not all numbers were read. Maximum size reached (%u)\n", MAX_SIZE);
    }

    fclose(file);
    return count;
}

int main(int argc, char *argv[])
{
    clock_t total_start = clock();

    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    printf("--- Starting file reading and processing ---\n");

    int size = readNumbersFromFile(argv[1]);
    if (size < 0)
    {
        return 1;
    }

    printf("--- Processing Array (%d elements) ---\n", size);
    processData(numbers, size);

    freeArrays();

    clock_t total_end = clock();
    double total_time = ((double)(total_end - total_start)) / CLOCKS_PER_SEC;
    printf("\nTotal execution time (including file reading): %.6f seconds\n", total_time);

    return 0;
}