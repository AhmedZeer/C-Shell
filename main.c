#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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

char **lsh_split_line(char *line){
  int bufsize = LSH_TOK_BUFSIZE, pos = 0;
  char **tokens = malloc( bufsize * sizeof(char*));
  char *token;

  if(!tokens){
    fprintf(stderr, "ALLOCATION FAILURE!\n");
    exit(EXIT_FAILURE);
  }
  token = strtok( line, LSH_TOK_DELEIM);
  while( token != NULL ){ 
    tokens[pos] = token;
    pos++;
    if( pos >= bufsize ){
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc( tokens, bufsize * sizeof(char*));
      if( !tokens ){
        fprintf(stderr, "LSH: ERROR ALLOCATION\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, LSH_TOK_DELEIM);
  }
  tokens[pos] = NULL;
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
      wpid = waitpid(pid, &status, WUNTRACED);
    } while ( !WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}


char *builtin_str[] = {

  "cd",
  "help",
  "exit"
};

int (*builtin_func[])(char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_sum(){
  return sizeof(builtin_str) / sizeof(char*);
}

int lsh_cd( char **args ){
  if ( args[1] == NULL ){
    fprintf(stderr, "lsh: expected argument to \"cd\" \n");
  } else {
    if( args[1] != 0 ){
      perror("lsh");
    }
  }
  return 1;
}

int lsh_help( char **args ){
  int i;
  printf("Welcome To Shell's Help.\n");
  printf("Type profram names and arguments and hit enter.\n");
  printf("These are builtins:\n");

  for( i = 0 ; i < lsh_sum() ; i++ ){
    printf("  %s\n", builtin_str[i]);
  }

  return 1;
}


int lsh_exit(char** args){
  return 1;
}

int lsh_execute( char** args ){
  int i;
  if( args[0] == NULL ){
    return 1;
  }

  for( i = 0 ; i < lsh_sum() ; i++ ){
    if( strcmp(args[0], builtin_str[i]) == 0 ){
      return (*builtin_func[i])(args);
    }
  }
  return lsh_launch(args);
}

void lsh_loop(){
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

};

int main(){


  lsh_loop();



  return EXIT_SUCCESS;
}

