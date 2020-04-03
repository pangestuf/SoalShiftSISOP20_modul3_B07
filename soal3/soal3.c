#include<stdio.h>
#include<string.h>
#include <dirent.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define MAX 256

int ch;
FILE *file1, *file2;
char source[MAX], dest[MAX];
DIR *d;
struct dirent *dir;

pthread_t tid[3];

const char *cari_ekstensi(const char *);
void *bikin_folder(void *);
void *list_folder(void *);

const char *cari_ekstensi(const char *nama_file){
	char *ekstensi = strrchr (nama_file, '.');
	if (ekstensi == null)
	return "unknown";
	return ekstensi + 1;
}

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
