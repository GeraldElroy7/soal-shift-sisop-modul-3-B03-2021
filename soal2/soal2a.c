#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/ipc.h>

int (*value)[7];
int matrix[7][7], matrix2[7][7];
int row1 = 4, row2 = 3;
int col1 = 3, col2 = 6;
pthread_t tid1, tid2, tid3;

void *InputMatrix1(void *arg)
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

    return NULL;
}

void *InputMatrix2(void *arg)
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

    return NULL;
}

void *multiplication(void *arg)
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
            value[i][j] = result;
            result = 0;
        }
    }
    return NULL;
}

int main()
{
    int i, j;
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int[7][7]), IPC_CREAT | 0666);
    value = shmat(shmid, 0, 0);

    pthread_create(&tid1, NULL, InputMatrix1, NULL);
    pthread_create(&tid2, NULL, InputMatrix2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    for (i = 0; i < row1; i++)
    {
        for (j = 0; j < col2; j++)
        {
            value[i][j] = 0;
        }
        pthread_create(&tid3, NULL, multiplication, NULL);
        pthread_join(tid3, NULL);
    }

    printf("Hasil perkalian :\n");
    for (i = 0; i < row1; i++)
    {
        for (j = 0; j < col2; j++)
        {
            printf("[%d]\t", value[i][j]);
        }
        printf("\n");
    }

    return 0;
}
