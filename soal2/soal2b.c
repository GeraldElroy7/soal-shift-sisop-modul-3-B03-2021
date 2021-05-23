//REVISI 2B
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/ipc.h>
 
int row = 4, col = 6;
int newmatrix[4][6];
int matrix3[4][6];
 
struct matrix_arg
{
   int cell1;
   int cell2;
};
 
void inputNewMatrix()
{
   printf("Masukkan angka-angka untuk matriks 4*6 yang baru :\n");
   int k, j;
 
   for (k = 0; k < row; k++)
   {
       for (j = 0; j < col; j++)
       {
           scanf("%d", &newmatrix[k][j]);
       }
   }
}
 
void outputMatrix(int a, int b)
{
   printf("[");
   if (a == 0 || b == 0)
       printf("0");
   else
   {
       if (a == b || a < b)
       {
           int counter = a;
           for (int i = a; i > 0; i--)
           {
               printf("%d", i);
               counter--;
               if (counter != 0)
               {
                   printf("*");
               }
           }
       }
       else if (a > b)
       {
           for (int j = a; j > b; j--)
           {
               printf("%d", j);
               if (j != b + 1)
               {
                   printf("*");
               }
           }
       }
   }
   printf("]\t");
}
 
void printmatrix(){
   printf("Matriks pertama :\n");
  
   for (int i = 0; i < row; i++)
   {
       for (int j = 0; j < col; j++)
       {
           printf("[%d]\t", matrix3[i][j]);
       }
       printf("\n");
   }
}
 
void *func(void *arguments)
{
   struct matrix_arg *cell = (struct matrix_arg *)arguments;
 
   outputMatrix(cell->cell1, cell->cell2);
}
 
void main()
{
   pthread_t tid[24];
 
   key_t key = 1234;
 
   int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
   int *value = (int *)shmat(shmid, NULL, 0);
 
   for (int i = 0; i < row; i++)
   {
       for (int j = 0; j < col; j++)
       {
           matrix3[i][j] = value[i*6+j];
       }
   }
 
   printmatrix();
   inputNewMatrix();
 
   int a, b, index = 0;
   struct matrix_arg cell;
 
   for (int i = 0; i < row; i++)
   {
       for (int j = 0; j < col; j++)
       {
           if (matrix3[i][j] == 0)
           {
               cell.cell1 = 0;
               cell.cell2 = 0;
           }
           else if (matrix3[i][j] >= newmatrix[i][j])
           {
               a = matrix3[i][j];
               b = newmatrix[i][j];
               cell.cell1 = a;
               cell.cell2 = b;
           }
           else if (matrix3[i][j] < newmatrix[i][j])
           {
               a = matrix3[i][j];
               cell.cell1 = a;
               cell.cell2 = a;
           }
           pthread_create(&tid[index], NULL, &func, (void *)&cell);
           index++;
       }
       printf("\n");
   }
 
   for(int k = 0; k < index; k++)
   {
       pthread_join(tid[k], NULL);
   }
 
   shmdt(value);
   shmctl(shmid, IPC_RMID, NULL);
}
 
 


