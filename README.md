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

Hasil perkalian matrix yang didapat dari 2A akan kembali digunakan pada soal ini. Matrix dari 2A akan melakukan perhitungan dengan matrix 4x6 yang baru, namun dengan cara yang berbeda, perhitungannya mirip seperti permutasi. Penjelasannya sebagai berikut,

![image](https://user-images.githubusercontent.com/64303057/119258070-58ba0d80-bbf2-11eb-882e-806ef128edf3.png)

di mana matriks A (dari soal 2A) menjadi angka untuk faktorial, lalu cell dari matrix B (yang baru) menjadi batas maksimal faktorialnya. Setiap perhitungan wajib menggunakan **thread**. 

Awalnya, buat terlebih dulu matrix 4x6 yang baru beserta **struct** yang nanti akan menyimpan nilai A dan B.

```c
struct matrix_arg
{
   int cell1;
   int cell2;
};
```

Karena perhitungannya nanti wajib menggunakan thread pada setiap cellnya, maka inisiasi id thread beserta `value` agar dapat mengambil nilai matrix pada program 2A.

```c
pthread_t tid[24];
 
key_t key = 1234;
 
int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
int *value = (int *)shmat(shmid, NULL, 0);
```

Kemudian, program akan meminta *user* untuk mengisi matrix yang baru, caranya sama dengan yang ada di program 2A.

Setelah matrix yang baru telah ada, lakukan perbandingan antar cellnya dengan matrix yang lama. Jika ada ditemukan angka 0 dari keduanya, maka simpan angka 0 untuk tiap variabel dalam **struct**. Begitu juga dengan kondisi lainnya, sesuai dengan ketentuan faktorial tadi. Perhitungan dilakukan sambil membuat thread yang baru, dan memanggil fungsi yang akan melanjutkan perhitungan. 

```c
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
   ```
   
Fungsi `func` akan meneruskan nilai yang sudah disimpan dalam **struct** ke fungsi berikutnya, yaitu print angka sesuai yang diminta pada soal.
```c
void *func(void *arguments)
{
   struct matrix_arg *cell = (struct matrix_arg *)arguments;
 
   outputMatrix(cell->cell1, cell->cell2);
}
```

Fungsi `outputMatrix` akan melakukan print angka sesuai dengan yang diminta pada soal. Parameternya adalah dua angka integer yang tadi sudah disimpan dalam **struct**. Soal meminta *user* untuk print angka secara berderet, bukan hasil perkalian dari faktorialnya. Contohnya sebagai berikut:

![image](https://user-images.githubusercontent.com/64303057/119258977-3629f380-bbf6-11eb-8f43-2345fd36c158.png)

Agar memudahkan untuk dibaca, setiap cell akan dipisahkan dengan karakter "[ ]". Jika angka A & B sama, atau A < B, maka akan lakukan print angka dari `A!` dan diselingi karakter * pada tiap angkanya. Sedangkan untuk A > B, maka akan lakukan print angka dari rumus `A!/(A-B)!`.

```c
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
```

Terakhir, pada akhir program, lakukan `pthread_join` untuk tiap id agar thread dapat berjalan dengan baik.

```c
for(int k = 0; k < index; k++)
{
   pthread_join(tid[k], NULL);
}
 
shmdt(value);
shmctl(shmid, IPC_RMID, NULL);
```

