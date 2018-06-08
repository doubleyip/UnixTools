//Simon Yip CSCI 49366 HW 11 Matrix Multiplication
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
 
#define size 3
#define maxnumthread 4
int threadcounter;
int matrix1[size][size];
int matrix2[size][size];
int solution[size][size];

void* multiply(void* arg);
void printmatrix(int a[size][size]);

int main(){
/*
int size;
printf("Enter the size of the matrix you want multiply");
scanf("%d",&size);	
*/
//int solution[size][size];
pthread_t threads[maxnumthread];

    for (int i=0;i<size;i++) 
	{    								//randomly set matrix a
        for (int j=0;j<size;j++) 
		{
            matrix1[i][j]=rand()%9;
        }
    }
 
	for (int i=0;i<size;i++) {		//set matrix to identity matrix to check multiplication
        for (int j=0;j<size;j++) {
            if(i==j)
			{
            matrix2[i][j]=1;
			}
			else matrix2[i][j]=0;
        }
    }

    for (int i =0;i<maxnumthread;i++) 
	{
        int* p;
        pthread_create(&threads[i], NULL, multiply, (void*)(p));
    }
    for (int i = 0;i<maxnumthread;i++) 
	{
        pthread_join(threads[i], NULL); 
	}		
 
	printf("\n");			//print matrix 1
    printf("matrix 1\n");
    printmatrix(matrix1);
 
	printf("\n");			//print matrix 2
    printf("matrix 2 (identity matrix)\n");
    printmatrix(matrix2);

    printf("\n");			//print result matrix
    printf("solution\n");
    printmatrix(solution);
	
    return 0;
}

void* multiply(void* arg)
{
int threads=threadcounter+1;

for (int i=threads*size/4;i<(threads+1)*size/4;i++)
	{		
        for (int j=0;j<size;j++) 
		{
            for (int k=0;k<size;k++) 
			{
                solution[i][j]=solution[i][j]+(matrix1[i][k]*matrix2[k][j]);
				
			}
		}
	}
threadcounter++;
}

void printmatrix(int a[size][size])
{
for (int i=0;i<size;i++) 
	{
        for (int j=0;j<size;j++) 
		{
            printf("%d ",a[i][j]);
		}
		printf("\n");
    }	
}