# SoalShiftSISOP20_modul3_B07

## Soal 3

### Fungsi untuk mencari ekstensi dari suatu file :
```
const char *cari_ekstensi(const char *nama_file){
	char *ekstensi = strrchr (nama_file, '.');
	if (ekstensi == null)
	return "unknown";
	return ekstensi + 1;
}
```

### Fungsi untuk memindahkan file ke lokasi lain :
```
void *bikin_folder(void *arg){

fgets(source, MAX, stdin);
source[strlen(source)- 1] = '\0';

fgets(dest, MAX, stdin);
dest[strlen(target) - 1] = '\0';

file1 = fopen(source, "r");
file2 = fopen(dest, "w");

while((ch = fgetc(file1)) != EOF){
	fputc(ch, file2);
}

fclose(file1);
fclose(file2);

return 0;
}
```

### Fungsi untuk menampilkan file di suatu directory :
```
void *list_folder (void *arg){

d = opendir(".");

if (d){
while ((dir = readdir(d)) != NULL){
	printf("%s\n", dir->d_name);
}
closedir(d);
}
return(0);
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
