#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int calculateMatrix(int,int*);
void main()
{
    pthread_t tid[20];//inisialisasi awal
        key_t key = 1234;
    int *matrix;
    int iret[20];
    int shmid =shmget(key,sizeof(int)*4*5,IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid,NULL,0);
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            printf("%d ",matrix[i*5 + j]);
        }
        printf("\n");
    }
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

  for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            printf("%d ",matrix[i*5 + j]);
        }
        printf("\n");
    }
    shmdt(matrix);
    shmctl(shmid, IPC_RMID, NULL);
}
void calculateMatrix(int* matrix){
    int x=1;
    for(int i=2;i<=matrix;i++){
        x*=i;
    }
    matrix=x;
}
