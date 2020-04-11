#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
#define MAX 256

struct v
{
    char *x[1];
    char *y[1];
    char *z[1];
    char *w[1];
};

int status;
pthread_t tid[3];

static void StripFileName(char *FileName, char *NewFileName, char *extension){
    int PLocation;
    int i, j, x, y, z;

    i = strlen(FileName) - 1;

    while (i && FileName[i] != '.')
    {
        i--;
    }

    if (i)
    {
        PLocation = i;
    }
    else
    {
        PLocation = strlen(FileName);

        i = strlen(FileName) - 1;
    }

    while (i && FileName[i] != '/')
    {
        if (FileName[i] != '/')
            i--;
    }

    if (FileName[i] == '/')
        i++;

    x = 0;

    while (i < PLocation)
    {
        NewFileName[x] = FileName[i];
        x++;
        i++;
    }

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

void* makedir(void *argue){
    struct v *arg = (struct v*) argue;

    status = 1;

    mkdir(*arg->y, 0777);

        return NULL;
}

void* move(void *argue){
    struct v *arg = (struct v*) argue;

    while (status != 1)
    {

    }

    int ch;
    FILE *fp1, *fp2;

    fp1 = fopen(*arg->z, "r");
    fp2 = fopen(*arg->w, "w");

    while ((ch = fgetc(fp1)) != EOF) {
            fputc(ch, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    remove(*arg->z);

        return NULL;
}

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

    if (strcmp (argv[1],f) == 0)
    {
        for (; i < argc; i++)
        {
            memset(FileName, 0, sizeof(FileName));
            memset(NewFileName, 0, sizeof(NewFileName));
            memset(extension, 0, sizeof(extension));
            memset(awal, 0, sizeof(awal));
            memset(akhir, 0, sizeof(akhir));
		
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

            
            struct v *data = (struct v*) malloc (sizeof (struct v));
            *data -> x = NewFileName;
            *data -> y = extension;
            *data -> z = awal;
            *data -> w = akhir;

            int k = 0;
            status = 0;

            
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
