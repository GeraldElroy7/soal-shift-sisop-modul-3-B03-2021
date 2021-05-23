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


## Soal 3

Pada soal 3, *user* diminta untuk membantu Alex dalam mensortir file-file sesuai dengan ekstensinya, baik file yang dipilih, semua file dalam folder, atapun file-file yang berada di *working directory*.

### Cara Pengerjaan 3A

*User* diminta untuk mensortir file-file yang dipilih dengan memasukkan argumen -f dan path filenya

```c
...
if(strcmp(argv[1], "-f") == 0)
  {
    int id_count = 0;
    while (i < argc){
        ...
            pthread_create(&(tid[id_count]), NULL, file_arg, (void *)argv[i]);
            id_count++;
            i++;
        }
        else {
            return 0;
        }
    }
     for(int k = 0; k < id_count; k++){
        pthread_join(tid[k],NULL);
    }
  }
...
```

ketika memasukkan argumen '-f', maka argumen selanjutnya akan dicek sampai argument terakhir. kemudian tiap pathfile dari argumen akan di proses pada fungsi `file_arg` dengan menggunakan thread pada setiap prosesnya.

```c
void *file_arg(void *arg)
{
  pthread_mutex_lock(&lock);
  namafile = strrchr((char *)arg, '/') + 1;
  alamat_file = (char *)arg;
  if(!is_regFile((char *)arg))
  {
    printf("%s : Sad, gagal :(\n", namafile);
    pthread_mutex_unlock(&lock);
    return NULL;
  }
  else{
	...
	cek_folder(...)
	move_file(...);
	...
    pthread_mutex_unlock(&lock);
    return NULL;
  }
}
```

namefile dari hasil argumen akan dicek terlebih dahulu apakah termasuk kedalam jenis file pada umumnya atau tidak ada fungsi `is_regFile` dengan menggunakan `S_ISREG(s_path.st_mode)`. Setelah itu, jika file bukanlah suatu file, maka tidak akan bisa di proses. Sebaliknya jika memang merupakan file, maka akan di move ke folder ekstesinya dengan membuat foldernya telebih dahulu.

```c
void* cek_folder(char *foldername)
{
  int len = 0;
  char tipe[100];
  strcpy(tipe, foldername);
  ...
  else if((foldername[strlen(foldername)-1] >= 'a' && foldername[strlen(foldername)-1] <= 'z')
          || (foldername[strlen(foldername)-1] >= 'A' && foldername[strlen(foldername)-1] <= 'Z')){
    for(int i = 0; tipe[i]; i++)
    {
      tipe[i] = tolower(tipe[i]);
      len++;
    }
  }
  foldername = tipe;
  int exist = 0;
  ...
  if (!exist)
  {
    mkdir(foldername, 0700);
  }
}
```
nama folder akan dibuat dari huruf kecil, sehingga perbedaan antara lowercase dan uppercase tidak ada. Pada proses ini, folder akan dibuat.

```c
void* move_file(char *pathc, char *ekstensi, char *namafile, char *alamat_asal)
{
  sprintf(path_tujuan, "%s%s/%s", pathc, ekstensi, namafile);
  if (rename (alamat_asal, path_tujuan))
  {
    ...
  }
}
```

Selanjutya file akan dipidahkan dengan fungsi `rename()`.

#### Output
##### Mensortir file * dan nako.zip
![3 -f1](https://user-images.githubusercontent.com/65794806/119257832-b30a9c80-bbf9-11eb-86cf-4b9924b1cd52.png)

##### Hasil setelah di sortir
![3 -f2](https://user-images.githubusercontent.com/65794806/119257878-df261d80-bbf9-11eb-8ff7-16f0e01de8cf.png)

### Cara Pengerjaan 3B

Kali ini, *user* diminta untuk mensortir semua file dalam folder yang dipilih dengan argumen -d dan memasukkan path foldernya.

```c
...
else if(strcmp(argv[1], "-d") == 0)
  {
        if (argc > 3)
        {
          exit(EXIT_FAILURE);
        }
        ...
            list_file(argv[2]);
            read_path(argv[0]);
        }
        else{
            return 0;
        }
  }
...
```
argumen pemasukan path folder hanya sekali, sehingga jika lebih maka akan di EXIT. Argumen diterima yang berisi path folder akan di proses pada `list_file` untuk me-list semua file dalam folder, kemudian file akan dipindahkan pada fungsi `read_path`.

```c
void list_file(char *path){
  FILE *f = fopen("proses.txt", "w");
  if (f == NULL)
  {
    exit(1);
  }
  DIR *d;
  struct dirent *dir;
  d = opendir(path);
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      if (dir->d_type == DT_REG)
      {
        fprintf(f, "%s%s\n", path, dir->d_name);
        index_file++;
      }
    }
    closedir(d);
    fclose(f);
  }
}
```

File-file tersebut akan di list pada sebuah file teks yang akan membantu dalam proses read ketika memindahkannya. Jumlah file yang ada di dalam folder di simpan dalam variabel `index_file`.


```c
void read_path(char *arg){
  FILE* filepointer;
  int bufLength = 255;
  char buffer[bufLength];
  char *temp;
  temp = strrchr(arg, '/') + 1;
  strcat(temp, "\n");
  filepointer = fopen("proses.txt", "r");
  if(index_file == 0){
    puts("Yah, gagal disimpan :(");
    exit(EXIT_FAILURE);
  }
  else{
    puts("Direktori sukses disimpan!");
  }
  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    exit(EXIT_FAILURE);
  }
  // Create Thread
  int count_id = 0;
  while(fgets(buffer, bufLength, filepointer))
  {
    namafile = strrchr(buffer, '/') + 1;
    if (strcmp(namafile, "proses.txt\n") == 0 || strcmp(namafile, temp) == 0)
    {
      continue;
    }
    else{
      buffer[strcspn(buffer, "\r\n")] = 0;  // buat hapus enter di buffer
      pthread_create(&(tid[count_id]), NULL, file_arg, (void *)buffer);
      pthread_join(tid[count_id],NULL);
    }
  }
  fclose(filepointer);
  if (remove("proses.txt") == 0);
  else
    puts("proses.txt error");
}
```

Isi folder dapat disorti jika terdapat sebuah file sehingga `index_file` tidak boleh bernilai 0. Selanjutnya setiap file akan diproses kembali sesuai dengan algoritma soal3a melalui fungsi `file_arg`. Pada bagian akhir, file teks pembantu yaitu "proses.txt" akan dihapus.

#### Output
##### Mensortir isi folder tescase
![3 -d1](https://user-images.githubusercontent.com/65794806/119258232-67f18900-bbfb-11eb-9b18-5435cc5ff6d0.png)

##### Hasil sortir
![3 -d2](https://user-images.githubusercontent.com/65794806/119258267-86578480-bbfb-11eb-9b76-cd34af8981ff.png)

##### Semua file pada folder telah dipindah
![3 -d3](https://user-images.githubusercontent.com/65794806/119258313-b69f2300-bbfb-11eb-94e4-be85e7d4a0b3.png)


### Cara Pengerjaan 3C

Pada soal 3C, *user* diminta untuk mensortir file-file yang ada di working directory dengan memasukkan argumen * saja.

```c
...
else if(strcmp(argv[1], "*") == 0)
{
        if (argc > 2)
        {
          exit(EXIT_FAILURE);
        }
        list_file(path);
        read_path(argv[0]);
}
...
```

Pada algoritma ini, hanya berbeda sedikit dengan algoritma 3B yang mana argumen yang dimasukkan hanya * saja, sehingga jika ada argumen lain, maka akan di EXIT. Selain itu, fungsi `list_file` menggunakan parameter *path* yaitu letak working directorynya.

```C
void* move_file(char *pathc, char *ekstensi, char *namafile, char *alamat_asal)
{
  sprintf(path_tujuan, "%s%s/%s", pathc, ekstensi, namafile);
  if (rename (alamat_asal, path_tujuan))
  {
    if (errno == EXDEV) {
    }
    else {
            mkdir("Unknown", 0700);
            sprintf(path_file_ex, "%s/%s", pathc, ekstensi);
            sprintf(path_folder_unk, "%s/Unknown/%s", pathc, ekstensi);
          if (rename (path_file_ex, path_folder_unk))
          {
            if (errno == EXDEV) {
            }
            else {
                //printf("Rename Error\n");
                exit(EXIT_FAILURE);
            };
          }
        mkdir(ekstensi, 0700);
        move_file(pathc, ekstensi, namafile, alamat_asal);
    };
  }
}
```

Akan tetapi, khusus untuk 3C, akan ada tambahan case dimana jika terdapat file yang bernama ekstensi folder, maka akan kita proses file tersebut terlebih dahulu agar tidak terjadi error dalam proses mkdir karena adanya nama yang *exist*. File yang dapat mempunyai nama yang sama dengan nama ekstensi folder pastinya berjenis "Unknown", oleh karena itu folder "Unknown" akan dibuat dan file yang beresiko tersebut dipindahkan terlebih dahulu. Kemudian setelah itu, dibuatlah folder yang diinginkan.

#### Output
##### Kondisi sebelum argumen dijalankan
![3 star1](https://user-images.githubusercontent.com/65794806/119258631-1a761b80-bbfd-11eb-8595-ac44d7948464.png)

##### Hasil sortir
![3 star2](https://user-images.githubusercontent.com/65794806/119258657-3679bd00-bbfd-11eb-9b96-40513b55a60c.png)

### Cara Pengerjaan 3D

Pada *problem* 3D, *user* juga diminta untuk melakukan sortir terhadap file-file yang berjenis "Unknown" dan "Hidden".

```c
void *file_arg(void *arg)
{
  ...
  if(!is_regFile((char *)arg))
  {
	...
  }
  else{
      printf("%s : Berhasil Dikategorikan\n", namafile);
      if(cek_ekstensi(namafile))
      {
        ...
      }
      else
      {
        if(namafile[0] == '.'){
            cek_folder("Hidden");
            move_file(path, "Hidden", namafile, alamat_file);
        }
        else{
            cek_folder("Unknown");
            move_file(path, "Unknown", namafile, alamat_file);
        }
      }
    ...
  }
}
```

Pada fungsi `file_arg`, file yang tidak memiliki ekstensi adalah file yang tidak mempunya tanda titik '.' penamaannya(kecuali titik pada awal nama). "Hidden" adalah file yang memiliki simbol titik '.' pada awal namanya sedangkan case selain itu akan dimasukkan ke kategori "Unknown".


#### Output
**Folder Hidden dan Unknown**
![3D1](https://user-images.githubusercontent.com/65794806/119258930-65446300-bbfe-11eb-863e-88003f252029.png)

**Isi folder Unknown**
![3Dunk](https://user-images.githubusercontent.com/65794806/119258943-72f9e880-bbfe-11eb-8913-5b8a5847addc.png)

**Isi folder Hidden**
![3Dhidden](https://user-images.githubusercontent.com/65794806/119258961-81480480-bbfe-11eb-8aff-71d49e08b4b7.png)


### Cara Pengerjaan 3E

Pada soal 3E, *user* diminta untuk memproses kategorisasi 1 file oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat

```c
...
            pthread_create(&(tid[id_count]), NULL, file_arg, (void *)argv[i]);
            id_count++;
            i++;
        }
        else {
            return 0;
        }
    }
     for(int k = 0; k < id_count; k++){
        pthread_join(tid[k],NULL);
    }
...
```

File-file yang diproses dilakukan dengan thread yang jumlah tid-nya terus bertambah untuk memanggil fungsi `file_arg`. Setelah looping seleai, semua thread akan di join ke NULL.

### Kendala Selama Pengerjaan

1. Problem pada 3C mengenai mkdir yang ternyata tidak dapat dibuat jika ada suatu folder atau file yang memiliki nama yang sama dengan folder yang ingin dibuat.
