# SoalShiftSISOP20_modul3_B07

## Soal 3
#### Inisiasi Struct
```
struct v
{
    char *x[1];
    char *y[1];
    char *z[1];
    char *w[1];
};
```
#### Variabel Global
```
int status;
pthread_t tid[3];
```
#### Fungsi untuk mencari nama file dan ekstensi
```
static void StripFileName(char *FileName, char *NewFileName, char *extension){
    int PLocation;
    int i, j, x, y, z;

    i = strlen(FileName) - 1;
    
/* Mencari awal nama file atau titik */
    while (i && FileName[i] != '.')
    {
        i--;
    }
    
/* Jika i > 1, nama file memiliki "." (titik) */
    if (i)
    {
        PLocation = i;
    }
    else
    {
/* Jika nama file tidak memiliki "." (titik) */    
        PLocation = strlen(FileName);

/* Me-reset nilai i */ 
        i = strlen(FileName) - 1;
    }

    while (i && FileName[i] != '/')
    {
        if (FileName[i] != '/')
            i--;
    }

 /* Jika nama file memiliki separator, longkap 1 character */
    if (FileName[i] == '/')
        i++;

    x = 0;

    while (i < PLocation)
    {
        NewFileName[x] = FileName[i];
        x++;
        i++;
    }
  
  /* Meng-copy ekstensi */
    y = 0;
    j = strlen(FileName) - 1;
    while (i<j)
    {
        extension[y] = FileName[i+1];
        y++;
        i++;
    }

    NewFileName[x] = '\0';
    extension[y] = '\0';
}
```
#### Fungsi untuk membuat directory
```
void* makedir(void *argue){
    struct v *arg = (struct v*) argue;

    status = 1;

    mkdir(*arg->y, 0777);

        return NULL;
}
```
#### Fungsi untuk memindahkan file
```
void* move(void *argue){
    struct v *arg = (struct v*) argue;

    while (status != 1)
    {

    }

    int ch;
    FILE *fp1, *fp2;

    fp1 = fopen(*arg->z, "r");
    fp2 = fopen(*arg->w, "w");

/* Meng-copy isi dari file awal ke file tujuan */
    while ((ch = fgetc(fp1)) != EOF) {
            fputc(ch, fp2);
    }
    
/* Menutup file */
    fclose(fp1);
    fclose(fp2);
    
/* Remove file lama */
    remove(*arg->z);

        return NULL;
}
```
#### Fungsi Main
```
int main(int argc, char* argv[]){
    char FileName[31];
    char NewFileName[31];
    char extension[5];
    char awal[50];
    char akhir[50];
    char bintang[] = "*";
    char f[] = "-f";
    char d[] = "-d";
    int i = 2;

/* Untuk argumen "-f" */
    if (strcmp (argv[1],f) == 0)
    {
        for (; i < argc; i++)
        {
	
	/* memset */
            memset(FileName, 0, sizeof(FileName));
            memset(NewFileName, 0, sizeof(NewFileName));
            memset(extension, 0, sizeof(extension));
            memset(awal, 0, sizeof(awal));
            memset(akhir, 0, sizeof(akhir));
	
	/* mencari path, nama file, dan ekstensi */
            strcpy(FileName, argv[i]);
            StripFileName(FileName, NewFileName, extension);
            strcat(NewFileName, ".");
            strcat(NewFileName, extension);
            strcat(awal, "/Home/SoalShiftSISOP20_modul3_B07/soal3/soal3/soal3");
            strcat(awal, NewFileName);
            strcat(akhir, "/Home/SoalShiftSISOP20_modul3_B07/soal3/soal3/soal3");
            strcat(akhir, extension);
            strcat(akhir, "/");
            strcat(akhir, NewFileName);

            /* memindahkan kedalam struct */
            struct v *data = (struct v*) malloc (sizeof (struct v));
            *data -> x = NewFileName;
            *data -> y = extension;
            *data -> z = awal;
            *data -> w = akhir;

            int k = 0;
            status = 0;

            /* membuat thread */
            while (k<2)
            {
                if(k==0){
                    pthread_create(&(tid[k]),NULL,&makedir,(void *) data);
                } else {
                    pthread_create(&(tid[k]),NULL,&move,(void *) data);
                }
                pthread_join(tid[k],NULL);
                k++;
            }
        }
    }

    if (strcmp (argv[1],bintang) == 0)
    {

    }

    if (strcmp (argv[1],d) == 0)
    {

    }
    return 0;
}
```
## Soal 4
### Soal 4a
#### Menginisialisasi matriks
```
int matriksA[4][2]={{1,1},
                        {1,1},
                        {1,1},
                        {1,1}};
    int matriksB[2][5]={{1,1,1,1,1},
                        {1,1,1,1,1}};
    int matriksC[20];
```
#### Perkalian Matriks
```
for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            matriksC[i*5+j]=matriksA[i][0]*matriksB[0][j]+matriksA[i][1]*matriksB[1][j];
            printf("%d ",matriksC[i*5+j]);
        }
        printf("\n");
    }
```
#### Membuat shared memory
```
key_t key = 1234;
    int *matrix
    int shmid =shmget(key,sizeof(int)*4*5,IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid,NULL,0);
```
#### Memasukan nilai ke shared memory
```
for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            matrix[i*5 + j] = matriksC[i*5][j];
        }
    }
```
### Soal 4b
#### Menginisialisasi
```
 pthread_t tid[20];//inisialisasi awal
        key_t key = 1234;
    int *matrix;
    int iret[20];
    int shmid =shmget(key,sizeof(int)*4*5,IPC_CREAT | 0666);
```
#### Mengeluarkan isisnya
```
for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            printf("%d ",matrix[i*5 + j]);
        }
        printf("\n");
    }
```
#### Membuat thread untuk perhitungan
```
  for (int i = 0; i < 20; i++) {
      iret[i] = pthread_create( &tid[i], NULL, calculateMatrix, matrix[i]); //membuat thread pertama
      if(iret[i]) //jika eror
      {
         printf("\n can't create thread : [%s]",strerror(err));
      }
    }
  }
    for (int i = 0; i < 20; i++) {
        pthread_join( tid[i], NULL );
  }
```
#### Menghitung faktorial
```
void calculateMatrix(int* matrix){
    int x=1;
    for(int i=2;i<=matrix;i++){
        x*=i;
    }
    matrix=x;
}
```
#### Mengeluarkan isinya
```
for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            printf("%d ",matrix[i*5 + j]);
        }
        printf("\n");
    }
```
### Soal 4c
#### Menjalankan ls
```
else if(pid == 0){
        close(1);
        dup2(fd[1]);
        close(fd[0]);
        execlp("ls","ls",NULL);
```
#### Menjalankan wc
```
	close(0);
        dup(fd[0]);
        close(fd[1]);
        execlp("wc","wc","-l",NULL);
```
