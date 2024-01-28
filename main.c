#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#define EXIT_SUCCESS 0 
#define EXIT_FAILURE 0 
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELEIM " \t\n\a\r"

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char* lsh_read_line(){
  int bufsize = LSH_RL_BUFSIZE;
  int pos = 0;
  char* buffer = malloc( sizeof(char) * bufsize );
  int c;
  
  while(1){
    c = getchar();
    if( c == EOF || c == '\n' ){ //EOF IS AN INTEGER THATS WHY WE DECLARED c AS SUCH.
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

char ** lsh_split_line(char *line){
  int bufsize = LSH_TOK_BUFSIZE, pos = 0;
  char **tokens = malloc( bufsize * sizeof(*char));
  char *token;

  if(!tokens){
    fprintf(stderr, "ALLOCATION FAILURE!\n");
    exit(EXIT_FAILURE);
  }
  token = strtok( line, LSH_TOK_DELEIM);
  while( token != NULL ){ 
    tokes[pos] = token;
    pos++;
    if( pos >= bufsize ){
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc( tokens, bufsize * sizeof( * char ));
      if( !tokens ){
        fprintf(stderr, "%LSH: ERROR ALLOCATION\n")
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, LSH_TOK_DELEIM);
  }
  token[pos] = NULL;
  return tokens;
}

int lsh_launch(char **args){
  pid_t pid, wpid;
  int status;
  
  pid = fork();
  if ( pid == 0 ){
    if ( execvp(args[0], args) == -1){
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if ( pid < 0 ) {
    //forking error
    perror("lsh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACTED)
    } while ( !WIFEXITED(status) && !WIFSIGNALED(status) )
  }
  return 1;
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

