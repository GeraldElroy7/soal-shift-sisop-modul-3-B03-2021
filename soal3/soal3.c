#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>

char path[PATH_MAX+1], *p;
char tempArgv[PATH_MAX+1];
char tempArgv2[PATH_MAX+1];
char buf[PATH_MAX+1];
char path_tujuan[1024];
void *file_arg(void *arg);
char *namafile;
char *alamat_file;
pthread_t tid;
pthread_mutex_t lock;
int index_file = 0;

void path_programC(char *str){
  FILE *fp;
  char buf[4096], *p;

  *str = '\0';
  if(!(fp = fopen("/proc/self/maps", "r"))) {
     return;
  }
  fgets(buf, sizeof(buf), fp);
  fclose(fp);

  *(p = strchr(buf, '\n')) = '\0';

  while(*p != ' ') p--;

  strcpy(str, p+1);
  str[PATH_MAX] = '\0';
}

int is_regFile(const char *path)
{
  struct stat s_path;
  stat(path, &s_path);
  return S_ISREG(s_path.st_mode);
}

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

void read_path(char *arg){
  FILE* filepointer;
  int bufLength = 255;
  char buffer[bufLength];

  char *temp;
  temp = strrchr(arg, '/') + 1;
  strcat(temp, "\n");

  filepointer = fopen("proses.txt", "r");

  //printf("Nfile: %d\n", index_file);
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
  while(fgets(buffer, bufLength, filepointer))
  {
    namafile = strrchr(buffer, '/') + 1;

    if (strcmp(namafile, "proses.txt\n") == 0 || strcmp(namafile, temp) == 0)
    {
      continue;
    }
    else{
      buffer[strcspn(buffer, "\r\n")] = 0;  // buat hapus enter di buffer

      pthread_create(&(tid), NULL, file_arg, (void *)buffer);

      pthread_join(tid, NULL);
    }
  }
  fclose(filepointer);

  if (remove("proses.txt") == 0);
  else
    puts("proses.txt error");
}

char *my_strchr(const char *str, int c){
    do
    {
        if(*str == c)
            return (char*) str;
    }while(*str++);
    return 0;
}

char *cek_ekstensi(char *filename)
{
  char *temp = filename;
  char *extension = strchr(temp, '.');

  if(!extension || extension == temp) return 0;

  int len = 0;
  char tipe[100];

  extension = extension+1;
  strcpy(tipe, extension);

  if((extension[strlen(extension)-1] >= 'a' && extension[strlen(extension)-1] <= 'z') || (extension[strlen(extension)-1] >= 'A' && extension[strlen(extension)-1] <= 'Z'))
  {
    for(int i = 0; tipe[i]; i++)
    {
      tipe[i] = tolower(tipe[i]);
      len++;
    }
  }
  extension = tipe;

  return extension;
}

void* cek_folder(char *foldername)
{
  int len = 0;
  char tipe[100];

  strcpy(tipe, foldername);

  if(!strcmp(foldername, "Unknown") || !strcmp(foldername, "Hidden")){

  }
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

  DIR *d;
  struct dirent *dir;
  d = opendir(".");

  while((dir = readdir(d)) != NULL)
  {
    if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name, "..") == 0) continue;

    if(!is_regFile(dir->d_name))
    {
      if(!strcmp(foldername, dir->d_name))
      {
        exist = 1;
      }
    }
  }
  closedir(d);

  if (!exist)
  {
    mkdir(foldername, 0700);
  }
}

void* move_file(char *pathc, char *ekstensi, char *namafile, char *alamat_asal)
{
  sprintf(path_tujuan, "%s%s/%s", pathc, ekstensi, namafile);

  if (rename (alamat_asal, path_tujuan))
  {
    if (errno == EXDEV) {
    }
    else {
        exit(EXIT_FAILURE);
    };
  }
}

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
      printf("%s : Berhasil Dikategorikan\n", namafile);
      if(cek_ekstensi(namafile))
      {
        cek_folder(cek_ekstensi(namafile));
        move_file(path, cek_ekstensi(namafile), namafile, alamat_file);
      }
      else
      {
        //printf("faile: %c\n", namafile[0]);
        if(namafile[0] == '.'){
            cek_folder("Hidden");
            move_file(path, "Hidden", namafile, alamat_file);
        }
        else{
            cek_folder("Unknown");
            move_file(path, "Unknown", namafile, alamat_file);
        }
      }
    pthread_mutex_unlock(&lock);
    return NULL;
  }
}

int main(int argc, char **argv)
{
  path_programC(buf);

  strcpy(path, buf);
  if((p = strrchr(path, '/')))
    *(p+1) = '\0';

  int i = 2;
  if (argc == 0)
  {
    puts("Argumen harus (-f, -d, atau *)");
    exit(EXIT_FAILURE);
  }

  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    return 1;
  }

  char tempTO[4];
  if(strcmp(argv[1], "-f") == 0)
  {
    while (i < argc){
        memset(tempArgv,'\0', sizeof(tempArgv));
        memset(tempTO,'\0', sizeof(tempTO));
        strcpy(tempArgv, argv[i]);

        long int tempPos;
        for(long int j=0; j < strlen(tempArgv); j++){
            if(tempArgv[j] == '/' && j != strlen(tempArgv)-1) {
                tempPos = j;
            }
        }
        tempTO[0] = tempArgv[tempPos-3];
        tempTO[1] = tempArgv[tempPos-2];
        tempTO[2] = tempArgv[tempPos-1];

        if(strcmp(tempTO, "/to") == 0){
            memset(tempArgv2,'\0', sizeof(tempArgv2));
            for(long int j=0; j < strlen(tempArgv); j++){
                if(j < tempPos-3) {
                    tempArgv2[j] = tempArgv[j];
                }
                else if(j >= tempPos){
                    tempArgv2[j-3] = tempArgv[j];
                }
            }
            strcpy(argv[i], tempArgv2);

            pthread_create(&(tid), NULL, file_arg, (void *)argv[i]);
            pthread_join(tid, NULL);
            i++;
        }
        else {
            return 0;
        }
    }
  }
  else if(strcmp(argv[1], "-d") == 0)
  {
        if (argc > 3)
        {
          exit(EXIT_FAILURE);
        }
        memset(tempArgv,'\0', sizeof(tempArgv));
        memset(tempTO,'\0', sizeof(tempTO));
        strcpy(tempArgv, argv[2]);

        long int tempPos;
        for(long int j=0; j < strlen(tempArgv); j++){
            if(tempArgv[j] == '/' && j != strlen(tempArgv)-1) {
                tempPos = j;
            }
        }
        tempTO[0] = tempArgv[tempPos-3];
        tempTO[1] = tempArgv[tempPos-2];
        tempTO[2] = tempArgv[tempPos-1];

        if(strcmp(tempTO, "/to") == 0){
            memset(tempArgv2,'\0', sizeof(tempArgv2));
            for(long int j=0; j < strlen(tempArgv); j++){
                if(j < tempPos-3) {
                    tempArgv2[j] = tempArgv[j];
                }
                else if(j >= tempPos){
                    tempArgv2[j-3] = tempArgv[j];
                }
            }
            strcpy(argv[2], tempArgv2);
            //puts(argv[2]);
            list_file(argv[2]);
            read_path(argv[0]);
        }
        else{
            return 0;
        }
  }
  else if(strcmp(argv[1], "*") == 0)
  {
        if (argc > 2)
        {
          exit(EXIT_FAILURE);
        }
        //printf("path = ");
        //puts(path);
        list_file(path);
        read_path(argv[0]);
    }

  return 0;
}
