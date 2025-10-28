#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define R 10000
#define C 10000

int sumArrayRows(int a[R][C]) {
    int sum = 0;
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            sum += a[i][j];
    return sum;
}

int sumArrayCols(int a[R][C]) {
    int sum = 0;
    for (int j = 0; j < C; j++)
        for (int i = 0; i < R; i++)
            sum += a[i][j];
    return sum;
}

int main() {
    static int array[R][C]; // use static to avoid stack overflow

    // Fill array
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            array[i][j] = i * C + j + 1;

    clock_t start, end;
    double cpu_time;

    // Time sumArrayRows
    start = clock();
    int sumRows = sumArrayRows(array);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("sumArrayRows = %d, Time = %f seconds\n", sumRows, cpu_time);

    // Time sumArrayCols
    start = clock();
    int sumCols = sumArrayCols(array);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("sumArrayCols = %d, Time = %f seconds\n", sumCols, cpu_time);

    return 0;
}



