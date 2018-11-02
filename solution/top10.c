// example of running 'cat top10.c | grep if'
// to compile type 'gcc top10.c -o top10'
// to run type './top10'
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {

   int A[2]; 
   pipe(A);
   int pid1 = fork(); 

   if (!pid1){
      dup2(A[1], 1);
      close(A[0]);
      close(A[1]);
      execlp("cut", "cut", "-d ", "-f1,10", "log.txt", NULL);
      exit(EXIT_FAILURE);
   }

   int B[2]; 
   pipe(B);

   int pid2 = fork();                                       
   if (!pid2){ 
      dup2(A[0], 0);
      close(A[0]);
      close(A[1]); 

      dup2(B[1], 1);
      close(B[0]);
      close(B[1]);      

      execlp("grep", "grep", "-", "-v", NULL);
      exit(EXIT_FAILURE);                            
   }

   close(A[0]);
   close(A[1]);

   int C[2]; 
   pipe(C);

   int pid3 = fork();                                       
   if (!pid3){ 
      dup2(B[0], 0);
      close(B[0]);
      close(B[1]);

      dup2(C[1], 1);
      close(C[0]);
      close(C[1]);

      execlp("awk", "awk", "{arr[$1]+=$2 ; bytes+=$2 } END {for (i in arr) {print i,\"-\",arr[i] ,\"-\", arr[i] * 100 / bytes , \"%\" } }", NULL);
      exit(EXIT_FAILURE);    
   }

   close(B[0]);
   close(B[1]);

   int D[2]; 
   pipe(D);
   int pid4 = fork(); 

   if (!pid4){ 
      dup2(C[0], 0);
      close(C[0]);
      close(C[1]);

      dup2(D[1], 1);
      close(D[0]);
      close(D[1]);
      execlp("sort", "sort", "-k3", "-nr", NULL);
      exit(EXIT_FAILURE);                            
   }

   close(C[0]);
   close(C[1]);

   int E[2]; 
   pipe(E);
   int pid5 = fork(); 

   if (!pid5){ 
      dup2(D[0], 0);
      close(D[0]);
      close(D[1]);

      dup2(E[1], 1);
      close(E[0]);
      close(E[1]);
      execlp("head", "head", "-10", NULL);
      exit(EXIT_FAILURE);                            
   }

   close(D[0]);
   close(D[1]);


   int F[2]; 
   pipe(F);

   int pid6 = fork(); 

   if (!pid6){ 
      dup2(E[0], 0);
      close(E[0]);
      close(E[1]);

      dup2(F[1], 1);
      close(F[0]);
      close(F[1]);
      execlp("cat", "cat", "-n", NULL);
      exit(EXIT_FAILURE);                            
   }

   close(E[0]);
   close(E[1]);

   FILE* Cin = fdopen(F[0], "r"); 
   char line[1000];                               

   while (fgets(line, 2, Cin)) {
      printf("%s", line); 
   }  

   close(E[0]);
   close(E[1]);    
   waitpid(pid6, 0, 0);                                  
   return 0;
}
