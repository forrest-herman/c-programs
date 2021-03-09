#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

int **readFile(int);
int **create2DArray(unsigned);
void printMatrix(int **, int);
void bubbleSort(int arr[], int n, bool reverse);
void swap(int *, int *);
int threadOpCount = 0, phaseCount = 0;

#define NUMTHRDS 4
pthread_t callThd[NUMTHRDS];

//declare mutex
pthread_mutex_t mutex_memory;
pthread_mutex_t mutex_count;
pthread_mutex_t mp; // mutex for condition variable

//declare condition var
pthread_cond_t cond_phaseComplete = PTHREAD_COND_INITIALIZER;

// datastructure to pass to each thread
struct arg_struct
{
};

typedef struct
{
    int **arrayFromFile;
    int n;
} DATASTRUCT;

DATASTRUCT arg_struct;

void *threadOperations(void *threadID)
{
    long threadNum = (long)threadID;
    int n = arg_struct.n;
    int **matrix = arg_struct.arrayFromFile;

    int *vector;
    vector = new int[n];
    bool row = true;
    bool reverse = false;
    bool odd = false;

    while (true)
    {
        //printf("\nThread: %ld \n", threadNum);
        //Critical Condition: Copy vectory from memory
        pthread_mutex_lock(&mutex_memory);
        if (row)
        {
            //printf("ROW\n");
            for (int j = 0; j < n; j++)
            {
                vector[j] = matrix[threadNum][j];
            }
        }
        else
        {
            //printf("COLUMN\n");
            // create column vector
            for (int j = 0; j < n; j++)
            {
                vector[j] = matrix[j][threadNum];
            }
            // sem_post(&mutexRow); // -3 // -2 // -1 // 0
        }
        pthread_mutex_unlock(&mutex_memory);

        //Sort vectors
        odd = threadNum % 2;
        if (row and odd)
        {
            reverse = true;
            bubbleSort(vector, n, reverse); //sortVector
        }
        else
        {
            reverse = false;
            bubbleSort(vector, n, reverse); //sortVector
        }

        //Critical Condition: Save sorted vector to memory
        pthread_mutex_lock(&mutex_memory);

        if (row)
            for (int j = 0; j < n; j++)
            {
                matrix[threadNum][j] = vector[j];
            }
        else
        {
            // create column vector
            for (int j = 0; j < n; j++)
            {
                matrix[j][threadNum] = vector[j];
            }
        }
        pthread_mutex_unlock(&mutex_memory);

        //Print Sorted Vector
        // for (int j = 0; j < n; j++)
        // {
        //     printf("%d ", vector[j]);
        // }

        //Lock common variable threadOpCount which keeps track of threads that have sorted in the phase
        pthread_mutex_lock(&mutex_count);
        threadOpCount++;
        //printf("\nThreadOpCount %d\n", threadOpCount);

        //all threads h
        if (threadOpCount >= 4)
        {
            phaseCount++;

            printf("-- Phase %d --\nMatrix:\n", phaseCount);
            printMatrix(matrix, n);

            threadOpCount = 0;

            //switch from sorting rows to col
            row = !row;
            //unblock all 3 waiting threads.
            pthread_cond_broadcast(&cond_phaseComplete);

            pthread_mutex_unlock(&mutex_count);
        }
        // ELSE WAIT ------------------------------------------------------------
        else
        {
            pthread_mutex_unlock(&mutex_count);

            //printf("\nBLOCKED Thread %ld\n", threadNum);

            // first 3 threads blocked here. Until 4th thread unblocks all.
            pthread_cond_wait(&cond_phaseComplete, &mp);
            //printf("\nUNBLOCKED Thread: %ld\n", threadNum);

            //switch from sorting rows to col
            row = !row;
        }

        if (phaseCount >= n + 1)
        {
            break;
        }
    }
    // quit thread
    exit(1);
}

int main(int argc, char *argv[])
{
    // from other file
    arg_struct.n = NUMTHRDS; //n by n matrix input
    arg_struct.arrayFromFile = readFile(arg_struct.n);
    printf("-- Initialize --\nMatrix:\n");
    printMatrix(arg_struct.arrayFromFile, arg_struct.n);

    long threadNum; // to indetify each thread
    pthread_attr_t attr;

    //initialize mutex
    pthread_mutex_init(&mutex_memory, NULL);
    pthread_mutex_init(&mutex_count, NULL);
    pthread_mutex_init(&mp, NULL);

    // /* Create threads to perform the sorting  */
    pthread_attr_init(&attr);

    // initialize n threads
    for (threadNum = 0; threadNum < NUMTHRDS; threadNum++)
    {
        //Each thread works on a different set of data
        pthread_create(&callThd[threadNum], &attr, threadOperations, (void *)threadNum);
    }

    //destroy semaphores and mutex
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex_memory);
    pthread_mutex_destroy(&mutex_count);
    //destroy condition variable
    pthread_cond_destroy(&cond_phaseComplete);
    pthread_mutex_destroy(&mp);

    pthread_exit(NULL);
}

//Functions -----------------------------

// Returns a pointer to a newly created 2d array the array2D has size [n x n]
int **create2DArray(unsigned size)
{
    int **array2D = 0;
    array2D = new int *[size];

    for (int h = 0; h < size; h++)
    {
        array2D[h] = new int[size];
    }

    return array2D;
}

int **readFile(int n)
{
    //Read input file
    string filename("input.txt");
    int number, row = 0, col = 0;
    int **input_array;

    input_array = create2DArray(n);

    ifstream input_file(filename);
    if (!input_file.is_open())
    {
        printf("No File Found\n");
        exit(1);
    }

    while (input_file >> number)
    {
        //cout << number << "; ";
        input_array[col][row] = number;
        row++;
        if (row >= n)
        {
            row = 0;
            col++;
        }
    }
    input_file.close();

    return input_array;
}

void printMatrix(int **arrayFromFile, int n)
{
    //print matrix
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", arrayFromFile[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n, bool reverse)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (reverse)
            {
                if (arr[j] < arr[j + 1])
                    swap(&arr[j], &arr[j + 1]);
            }
            else
            {
                if (arr[j] > arr[j + 1])
                    swap(&arr[j], &arr[j + 1]);
            }
}