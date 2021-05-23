//REVISI 2A
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
 
int matrix[4][3], matrix2[3][6];
int finalmatrix[4][6];
int row1 = 4, row2 = 3;
int col1 = 3, col2 = 6;
 
void InputMatrix()
{
   printf("Masukkan angka-angka untuk matriks 4*3 :\n");
   int k, j;
 
   for (k = 0; k < row1; k++)
   {
       for (j = 0; j < col1; j++)
       {
           scanf("%d", &matrix[k][j]);
       }
   }
 
}
 
void InputMatrix2()
{
   printf("Masukkan angka-angka untuk matriks 3*6 :\n");
   int k, j;
 
   for (k = 0; k < row2; k++)
   {
       for (j = 0; j < col2; j++)
       {
           scanf("%d", &matrix2[k][j]);
       }
   }
}
 
void multiplication()
{
   int i, j, k;
   int result = 0;
 
   for (i = 0; i < row1; i++)
   {
       for (j = 0; j < col2; j++)
       {
           for (k = 0; k < col1; k++)
           {
               result += matrix[i][k] * matrix2[k][j];
           }
           finalmatrix[i][j] = result;
           result = 0;
       }
   }
}
 
void printmatrix()
{
   int i,j;
 
   for (i = 0; i < row1; i++)
   {
       for (j = 0; j < col2; j++)
       {
           printf("[%d]\t", finalmatrix[i][j]);
       }
       printf("\n");
   }
}
 
int main()
{
   int i, j;
   key_t key = 1234;
   int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
   int *value = (int *)shmat(shmid, NULL, 0);
 
   InputMatrix();
   InputMatrix2();
   multiplication();
   printmatrix();
 
   for (i = 0; i < row1; i++)
   {
       for (j = 0; j < col2; j++)
       {
           value[i*6+j]=finalmatrix[i][j];
       }
   }
 
   sleep(40);
 
   shmdt(value);
   shmctl(shmid, IPC_RMID, NULL);
 
   return 0;
}
