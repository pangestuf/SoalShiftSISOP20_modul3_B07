#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main()
{
    int matriksA[4][2]={{1,1},
                        {1,1},
                        {1,1},
                        {1,1}};
    int matriksB[2][5]={{1,1,1,1,1},
                        {1,1,1,1,1}};
    int matriksC[20];
    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            matriksC[i*5+j]=matriksA[i][0]*matriksB[0][j]+matriksA[i][1]*matriksB[1][j];
            printf("%d ",matriksC[i*5+j]);
        }
        printf("\n");
    }
    key_t key = 1234;
    int *matrix
    int shmid =shmget(key,sizeof(int)*4*5,IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid,NULL,0);
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            matrix[i*5 + j] = matriksC[i*5][j];
        }
    }
    shmdt(matrix_data);
    shmctl(shmid, IPC_RMID, NULL);
}
