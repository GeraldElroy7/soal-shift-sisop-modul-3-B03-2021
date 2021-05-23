# soal-shift-sisop-modul-3-B03-2021

### Anggota Kelompok
1. Nur Putra Khanafi     05111940000020
2. Gerald Elroy Van Ryan 05111940000187
3. Husnan                05111940007002

## Soal 2

Pada soal no 2, *user* diminta membantu Crypto untuk membuatkan program matriks, yang nantinya matriks ini dapat melakukan perkalian (seperti pada umumnya) dan perkalian dengan kriteria khusus, yang mana nanti sistemnya seperti permutasi. 

### Cara Pengerjaan 2A

*User* diminta untuk membuat program perkalian matrix biasa untuk matrix 4x3 dengan 3x6 dan menampilkan angkanya. Soal ini akan menggunakan **shared memory**, agar hasil perkalian matrix yang baru dapat dipakai kembali oleh program lainnya untuk melanjutkan soal berikutnya. Inisiasi nilai `value` sebagai tempat menyimpan hasil perkalian matrix nanti, seperti berikut ini:

```c
key_t key = 1234;
   int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
   int *value = (int *)shmat(shmid, NULL, 0);
```

Pertama, dibuat terlebih dahulu matrix yang diimplementasikan melalui array dua dimensi. Angka-angka akan diinput ke array dengan cara seperti di bawah ini: 

```c
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
```
Ini adalah contoh untuk menginput angka ke dalam matrix 3x6.

Setelah itu, lakukan perkalian matrix menggunakan perulangan biasa. Program akan memanggil fungsi `multiplication` untuk melakukan perkalian dan menghasilkan matrix baru.

```c
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
```

Terakhir, program akan *print* hasil dari perkalian matriks yang sudah disimpan ke `value` agar dapat dipakai di program berikutnya. `Value` akan berjumlah 24 nilai, sesuai dengan banyaknya kotak pada matrix 4x6. Sertakan juga *command* `sleep()` agar **shared memory** dapat berjalan.

```c
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
```  

### Cara Pengerjaan 2B
