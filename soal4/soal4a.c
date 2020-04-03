LATIHAN MODUL 3 ------------------------------------------------
SOAL 1 -----------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //library thread

void *print_message_function( void *ptr );
void *calculateMatrix( void *ptr );

// Source matrices
// Make sure it fulfils the multiplication rules of matrices
int matrix1[3][3] = {
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
};

int matrix2[3][2] = {
  1, 2,
  4, 5,
  7, 8
};

// size of matrice
int mat1I = 3, mat1J = 3;
int mat2I = 3, mat2J = 2;
// values to be returned by thread processes
// int retVals[mat1I][mat2J];
int retVals[3][2];
// semaphone for retMatrix
int retMatLock = 0;


int main()
{
  pthread_t thread[mat1I][mat2J];//inisialisasi awal
  int iret[mat1I][mat2J];
  

  // Dispatch a single thread for every element in result matrix
  int i, j;
  for (i = 0; i < mat1I; i++) {
    for (j = 0; j < mat2J; j++) {
      int *message;
      message = malloc(sizeof(int) * 2);
      message[0] = i;
      message[1] = j;
      void *msgPtr = (void *)message;
      iret[i][j] = pthread_create( &thread[i][j], NULL, calculateMatrix, msgPtr); //membuat thread pertama
      if(iret[i][j]) //jika eror
      {
          fprintf(stderr,"Error - pthread_create() return code: %d\n",iret[i][j]);
          exit(EXIT_FAILURE);
      }

      // printf("pthread_create() for thread 1 returns: %d\n", iret[i][j]);
    }
  }

  for (i = 0; i < mat1I; i++) {
    for (j = 0; j < mat2J; j++) {
      pthread_join( thread[i][j], NULL );
      printf("thread joined\n");
    }
  }

  for (i = 0; i < mat1I; i++) {
    printf("%d", retVals[i][0]);
    for (j = 1; j < mat2J; j++) {
      printf(", %d", retVals[i][j]);
    }
    printf("\n");
  }
  exit(EXIT_SUCCESS);
}

void *calculateMatrix( void *temp) {
  int *message;
  message = (int *)temp;

  int iMat1 = message[0];
  int jMat2 = message[1];
  free(message);
  int sum = 0;
  int i;
  for(i = 0; i < mat1J; i++) {
    printf("%d,%d multi: %d * %d = %d\n", 
    iMat1, jMat2, matrix1[iMat1][i], matrix2[i][jMat2], sum);
    sum += matrix1[iMat1][i] * matrix2[i][jMat2];
  }

  // while(retMatLock) {
  //   _sleep(1);
  // }
  // retMatLock = 1;
  retVals[iMat1][jMat2] = sum;
  // retMatLock = 0;
}

void *print_message_function( void *ptr )
{
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);
  int i;
    for(i=0;i<10;i++){
        printf("%s %d \n", message, i);
    }
}
END OF SOAL 1 ----------------------------------
START OF SOAL 2 -----------------------------------
server.c -------------------------------------
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080


int main(int argc, char const *argv[]) {
	int keep_connection = 1;
	int server_int = 1;
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	// Recieve buffer
	char buffer[1024] = {0};

	// create socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// If socket opt permissionss error
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// socket setup
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// bind socket to addr
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket bound\n");

	printf("Socket listening\n");
	// socket listen from addr
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Socket listened success\n");

	// accept addr
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Socket Accepted\n");

	while(keep_connection) { 
		// Read request
		memset(buffer, 0, sizeof(buffer));
		valread = read( new_socket, buffer, 1024);
		printf("recieved message: %s\n", buffer);
		// Start switch case
		if (strcmp(buffer, "tambah") == 0) {
			printf("tambah switch\n");
			server_int++;
			char message[] = "Penambahan berhasil";
			send(new_socket, message, strlen(message), 0 );
		} else if (strcmp(buffer, "cek") == 0) {			
			printf("cek switch\n");
			// Message to send
			char message[1000];
			// memset(message, 0, sizeof(message));
			sprintf(message, "%d", server_int);
			send(new_socket, message, strlen(message), 0 );
		}
		// printf("%s\n", buffer );
		// Send messsage to client
		// send(new_socket, message, strlen(message), 0 );
		// printf("Hello message sent\n");
	}
	return 0;
}
end of server.c ---------------------------------------
client.c ----------------------------------
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
	int keep_connection = 1;
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	// message to send from client
	char *message = "message from client";
	// recieve buffer
	char buffer[1024] = {0};
	// create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
  
	memset(&serv_addr, '0', sizeof(serv_addr));
  
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// addr
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	printf("inet_pton done\n");

  // connect to server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	printf("Socket connected\n");

	while(keep_connection) { 
		char message[1000];
		memset(message, 0, sizeof(message));
		scanf("%s", message);
		send(sock, message, strlen(message), 0 );
		// read response
		memset(buffer, 0, sizeof(buffer));
		valread = read(sock, buffer, 1024);
		printf("%s\n",buffer );
	}
	return 0;
}

end of client.c ------------------------------------
END OF SOAL 2 ------------------------------------
SOAL 3 ------------------------------
ls -l | head -n 4 | tail -n 1
END OF SOAL 3 -----------------------------
END OF LATIHAN MODUL 3-------------------------------------------------------

SOAL 1 -----------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>

int main(int argc,char *argv[]) {
  int detik, menit, jam, detik_sk, menit_sk, jam_sk;
  char *stringToLongPtr;

  if (argc != 5) {
    printf ("jumlah input salah\n");
    return -1;
  }

  if(strcmp(argv[1],"*")==0) {
    detik = -1;
  } else {
    detik = strtol(argv[1], &stringToLongPtr, 10);// && argv[1] >= 0 && argv[1] < 23 && argv[1] == 60 )
    if (stringToLongPtr == argv[1]) {
      printf("input detik harus integer atau '*'\n");
      return -1;
    } else if (!(detik <= 59 && detik >= 0)) {
      printf("Detik tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  } 


  if(strcmp(argv[2],"*")==0) {
    menit = -1;
  } else {
    menit = strtol(argv[2], &stringToLongPtr, 10);
    if (stringToLongPtr == argv[2]) {
      printf("input menit harus integer atau '*'\n");
      return -1;
    } else if (!(menit <= 59 && menit >= 0)) {
      printf("Menit tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  }
  if (strcmp(argv[3],"*")==0){
    jam = -1;
  } else {
    jam = strtol(argv[3], &stringToLongPtr, 10);
    if (stringToLongPtr == argv[3]) {
      printf("input jam harus integer atau '*'\n");
      return -1;
    } else if (!(jam <= 23 && jam >= 0)){
      printf("Jam tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  }


  pid_t pid, sid;        // Variabel untuk menyimpan PID
  pid = fork();     // Menyimpan PID dari Child Process
  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/zenados/Documents/2shift")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  signal(SIGCHLD, SIG_IGN);
  while (1) {
    time_t sk = time(NULL);
    struct tm *sk_tm = localtime(&sk);

    detik_sk = sk_tm->tm_sec;
    menit_sk = sk_tm->tm_min;
    jam_sk = sk_tm->tm_hour;

    printf("jam_sk: %d; menit: %d; detik: %d\n", jam_sk, menit_sk, detik_sk);
    if((jam_sk == jam || jam == -1) && (menit_sk == menit || menit == -1) && (detik_sk == detik || detik == -1)){
      pid_t child;
      child = fork();

      int status;

      if (child == 0){
        char *tempString = malloc(sizeof(char)*200);
        char *bashArgv[] = {"bash", tempString, NULL};
        strcpy(tempString, argv[4]);
        execv("/bin/bash", bashArgv);
      }
    }
    sleep(1);
  }
}
END OF SOAL 1 ----------------------------------------------
SOAL 2 ---------------------------------------------------
soal2.c -----------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <limits.h>

/* void handlerFunc(int signum) 
{ 
  wait(NULL); 
} */

int presistChild = 0;

void deleteFolder(char timeString[])
{
  // Download pics
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  if (anak_id == 0)
  {
    char *rmArgv[] = {"rm", "-r", timeString, NULL};
    execv("/bin/rm", rmArgv);
  } else {
    wait(NULL);
  }
}

void zipDirectory(char timeString[])
{
  // Download pics
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  if (anak_id == 0)
  {
    // If folder is not included, append
    // NOTE: if folder is included use this
    // char *zipArgv[] = {"zip", "-r", timeString, timeString, NULL};
    char dest[100];
    memset(dest, 0, sizeof(dest));
    char *relativeAdd = "../";
    strcpy(dest, relativeAdd);
    strcat(dest, timeString);
    printf("dest is: %s\n", dest);
    char *zipArgv[] = {"zip", "-r", dest, ".", NULL};
    execv("/usr/bin/zip", zipArgv);
  } else {
    wait(NULL);
  }
}

void downloadPicsumNow()
{
  // Download pics
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }
  
  if (anak_id == 0)
  {
    // Download with (t%1000)+100 width (square) where t = time(NULL);
    char time_string_file[80];
    int time_int_file = time(NULL);
    // get time string for folder name
    time_t t_time_t_file = time_int_file;
    struct tm time_struct_file = *localtime(&t_time_t_file);
    snprintf(
      time_string_file, 
      sizeof(time_string_file), 
      "%d-%02d-%02d_%02d:%02d:%02d", 
      time_struct_file.tm_year + 1900, 
      time_struct_file.tm_mon + 1, 
      time_struct_file.tm_mday, 
      time_struct_file.tm_hour, 
      time_struct_file.tm_min, 
      time_struct_file.tm_sec
    );
    time_int_file = time_int_file%1000 + 100;
    // download from https://picsum.photos/width to time_string_file file
    // Gambar tersebut diberi nama dengan format timestamp [YYYYmm-dd_HH:ii:ss]
    char download_src[80];
    snprintf(download_src ,sizeof(download_src), "https://picsum.photos/%d", time_int_file);
    char *wgetArgv[] = {"wget", "-q", "-O",  time_string_file, download_src, NULL};
    // char *wgetArgv[] = {"wget", download_src, NULL};
    execv("/usr/bin/wget", wgetArgv);
  } 
}

int main(int argc, char *argv[]){
  if (argc == 2) {
    if (strcmp (argv[1], "-a") == 0) {
      presistChild = 0;
    } else if (strcmp (argv[1], "-b") == 0) {
      presistChild = 1;
    }
  }
  pid_t anak_id;
  char *workingDir = "/home/zenados/Documents/2shift";
  // Create child process
  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  // Kill parent if we're done creating child process
  if (anak_id > 0) {
    // exit_sucess constant is equal to 0
    exit(0);
  }
  // from now on it's child process

  // Random placement of closing file flags STDIN, STDOUT, STDERR
  close(STDOUT_FILENO);

  int sid = setsid();
  // If there's an error while assigning new sid
  // exit_failure constant is equal to 8
  if (sid < 0) exit(8);

  // Random placement of closing file flags STDIN, STDOUT, STDERR
  close(STDIN_FILENO);

  int chdirStat = chdir(workingDir);
  // if there's an error while changind directory
  if (chdirStat < 0) 
  // exit_failure constant is equal to 8
    exit(8);

  // Random placement of closing file flags STDIN, STDOUT, STDERR
  close(STDERR_FILENO);

  // change file permissions created by daemon process
  umask(0);

  // Create killer program
  pid_t killer_pid = fork();
  if (killer_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
  if (killer_pid == 0) {
    char *killerArg[] = {"gcc", "killer2.c", "-o", "killer2.exe", NULL};
    execv("/usr/bin/gcc", killerArg);
  }

  // Start of 1st child program
  while (1) {
    printf("Got into child daemon\n");
    // Get string of current time format
    char timeString[80];
    time_t t = time(NULL);
    struct tm timeStruct = *localtime(&t);
    snprintf(
      timeString, 
      sizeof(timeString), 
      "%d-%02d-%02d_%02d:%02d:%02d", 
      timeStruct.tm_year + 1900, 
      timeStruct.tm_mon + 1, 
      timeStruct.tm_mday, 
      timeStruct.tm_hour, 
      timeStruct.tm_min, 
      timeStruct.tm_sec
    );

    // Make new directory based on current time string
    mkdir(timeString, 0777);
    
    // Start of child child process
    pid_t anak_anak_id = fork();
    // If there's error for child creation
    if (anak_anak_id < 0) {
      // exit_failureconstant is equal to 8
      exit(8);
    }
    signal(SIGCHLD,SIG_IGN);
    // if is child child process
    if (anak_anak_id == 0) {
      signal(SIGCHLD,SIG_DFL);
      if (presistChild) {
        prctl(PR_SET_NAME,"dirChld\0",NULL,NULL,NULL);
      }

      printf("got into child process\n");
      // Change dir to new dir
      chdir(timeString);
      
      pid_t down_child_pid = fork();
      // If there's error for child creation
      if (down_child_pid < 0) {
        // exit_failureconstant is equal to 8
        exit(8);
      }
      // if is child child process
      if (down_child_pid == 0) {
        // handle all SIGCHLD requests
        // signal(SIGCHLD, handlerFunc); 
        signal(SIGCHLD,SIG_IGN);
        // Download every 5 seconds for 20 iterations
        for (int j = 0; j < 20; j++) {
        // for (int j = 0; j < 2; j++) {
          downloadPicsumNow();
          printf("Child child process waiting 5 sec\n");
          sleep(5);
        }
      } else {
        wait(NULL);
      }
      // Return signal handling to the default way
      // Wait until program is done downloading all files
      // Move to parent directory
      // Zip directory
      printf("zipping \n");
      zipDirectory(timeString);
      printf("zipped\n");
      chdir("..");
      // Delete the current folder
      printf("deleteing timestring folder\n");
      deleteFolder(timeString);
      printf("deleted timestring folder\n");
      exit(EXIT_SUCCESS);
    }
    printf("child process waiting 30 sec\n");
    sleep(30);
  }
}
end of soal2.c --------------------------------

start of killer2.c ------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <limits.h>

int main()
{
  pid_t anak_id;
  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  if (anak_id == 0)
  {
    // Exec pkill soal2.exe
    char *pkillArgv[] = {"pkill", "soal2.exe", NULL};
    execv("/usr/bin/pkill", pkillArgv);
  } else {
    // wait for child program
    wait(NULL);
    printf("running rm on file");
    execl("/bin/rm", "rm", "killer2.exe", NULL);
    printf("test, still shows after execl");
  }
  // Child program
  // delete exe self
}
end of killer2.c
END OF SOAL 2 ----------------------------------------------------------
SOAL 3 -----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <limits.h>
#include <dirent.h> 

int is_regular_file(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}


void mkdirMieSedap()
{
  pid_t mkdir_id;

  mkdir_id = fork();
  // If there's error for child creation
  if (mkdir_id < 0) 
    // exit_failure constant is equal to 8
    exit(8);

  // child process
  if (mkdir_id == 0)
  {
    pid_t indomie_id;

    indomie_id = fork();
    // If there's error for child creation
    if (indomie_id < 0) 
      // exit_failure constant is equal to 8
      exit(8);

    // child process
    if (indomie_id == 0)
    {
      char *mkdirArgs[] = {"mkdir", "-p", "indomie", NULL};
      execv("/bin/mkdir", mkdirArgs);
    }
    // Get sig child of indomie
    wait(NULL);
    sleep(5);

    pid_t sedap_id;
    sedap_id = fork();
    // If there's error for child creation
    if (sedap_id < 0) 
      // exit_failure constant is equal to 8
      exit(8);

    // child process
    if (sedap_id == 0)
    {
      char *mkdirArgs[] = {"mkdir", "-p", "sedaap", NULL};
      execv("/bin/mkdir", mkdirArgs);
    }
    // Get sig child of sedap
    wait(NULL);
    exit(EXIT_SUCCESS);
  }
}

void unzip()
{
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) 
    // exit_failure constant is equal to 8
    exit(8);

  // child process
  if (anak_id == 0)
  {
    char *argUnzip[] = {"unzip", "jpg.zip", NULL};
    execv("/usr/bin/unzip", argUnzip);
  }
}

int main() {
  // change current directory
  chdir("/home/zenados/modul2");
  // make directory
  mkdirMieSedap();
  // unzip stuffs
  unzip();
  // wait for mkdirMieSedap and unzip
  wait(NULL);
  wait(NULL);
  // waitpid example waitpid(pid, &status, 0);

  // read files and directory in directory
  struct dirent *de;  // Pointer for directory entry 

  // opendir() returns a pointer of DIR type.  
  DIR *dr = opendir("/home/zenados/modul2/jpg"); 

  if (dr == NULL)  // opendir returns NULL if couldn't open directory 
  { 
      printf("Could not open current directory" ); 
      return 0; 
  } 

  // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
  // for readdir() 
  // How many childs are there?
  int nChild = 0;
  while ((de = readdir(dr)) != NULL) 
  {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 ) {
      continue;
    }
    printf("%s\n", de->d_name); 
    char path[100];
    strcpy(path, "/home/zenados/modul2/jpg/");
    strcat(path, de->d_name);
    // If path is file
    if (is_regular_file(path))
    {
      // Move to sedaap
      // Add 1 more to the amount of children need to be waited
      nChild++;
      pid_t mover_pid = fork();
      if (mover_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
      if (mover_pid == 0) {
        char *mvArg[] = {"mv", path, "/home/zenados/modul2/sedaap", NULL};
        execv("/bin/mv", mvArg);
      }
    // If path is directory
    } else {
      // Move directory to indomie
      // Add 1 more to the amount of children need to be waited
      nChild++;
      // Dispatch dirChild
      pid_t dir_pid = fork();
      if (dir_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
      if (dir_pid == 0) {
        // Dispatch child
        pid_t mover_pid = fork();
        if (mover_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
        if (mover_pid == 0) {
          char *mvArgv[] = {"mv", path, "/home/zenados/modul2/indomie", NULL};
          execv("/bin/mv", mvArgv);
        } // End of mover child
        // get mover child sig
        wait(NULL);
        
        // Prepare touch path
        char touchPath[100];
        memset(touchPath, 0, sizeof(touchPath));
        strcpy(touchPath, "/home/zenados/modul2/indomie/");
        strcat(touchPath, de->d_name);
        // touch file coba1.txt
        pid_t touch1_pid = fork();
        if (touch1_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
        if (touch1_pid == 0) {
          // TODO: char new file
          strcat(touchPath, "/coba1.txt");
          char *touchArgv[] = {"touch", touchPath, NULL};
          execv("/bin/touch", touchArgv);
        } // End of touch1 child
        // wait 3 seconds
        sleep(3);
        // get touch1 child sig
        wait(NULL);

        // touch2 file coba2.txt
        pid_t touch2_pid = fork();
        if (touch2_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
        if (touch2_pid == 0) {
          // TODO: char new file
          strcat(touchPath, "/coba2.txt");
          char *touchArgv[] = {"touch", touchPath, NULL};
          execv("/bin/touch", touchArgv);
        } // End of touch2 child
        // get touch2 child sig
        wait(NULL);

        exit(EXIT_SUCCESS);
      }
    }
  }

  // Wait for all children
  while (nChild-- > 0) {
    wait(NULL);
  }

  closedir(dr);     
  return 0; 
}
END OF SOAL 3 -------------------------------------------------
