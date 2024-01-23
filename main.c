#include <stdio.h>
#include <stdlib.h>
#define EXIT_SUCCESS 0 
#define EXIT_FAILURE 0 
#define LSH_RL_BUFSIZE 1024

char* lsh_read_line(){
  int bufsize = LSH_RL_BUFSIZE;
  int pos = 0;
  char* buffer = malloc( sizeof(char) * bufsize );
  int c;
  
  while(1){
    c = getchar();
    if( c == EOF || c == '\n' ){
      c = '\0';
      return buffer;
    } else {
      buffer[pos] = c;
    }
    pos++;
    
    if (pos > bufsize){
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if(!buffer){
        fprintf(stderr, "LSH: REACLLOC ERROR!\n");
        exit(EXIT_FAILURE);
      }

    }



  }





  
}

(void) lsh_loop(void){
  char *line;
  char **args;
  int status;

  do{
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while(status);

}

int main(){

  /*load config files*/

  /*run comand loop*/
  lsh_loop();

  /*perform any shut-down or cleanup*/


  return EXIT_SUCCESS;
}

