#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "sys/wait.h"

// Macros
#define LSH_TOK_BUFFSIZE 64
#define LSH_TOK_DELIM " \t\n\a\r"

// Function declarations
void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_parse_line(char *line);
int lsh_execute(char **args);
int lsh_launch(char **args);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

int main(int argc, char **argv){

  lsh_loop();

  return EXIT_SUCCESS;
}

void lsh_loop(void){
  char *line;   // Store the input line (String)
  char **args;  // Store the args of the line
  int status;   // Status

  do {
    printf("$ ");
    line = lsh_read_line();
    args = lsh_parse_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

// Line reader
char *lsh_read_line(void){
  char *line = NULL;    
  ssize_t buffsize = 0;  

  if (getline(&line, &buffsize, stdin) == -1){
    if (feof(stdin)){
      exit(EXIT_SUCCESS); // EOF received / Program exited
    } else {
      perror("readline");
      exit(EXIT_FAILURE); // Error
    }
  }
  return line;
}

// Line parser
char **lsh_parse_line(char *line){
  int position = 0;
  int buffsize = LSH_TOK_BUFFSIZE;
  char **tokens = malloc(buffsize * sizeof(char*));
  char *token;

  if (!tokens){
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL){
    tokens[position] = token;
    position++;

    if (position >= buffsize){
      buffsize += LSH_TOK_BUFFSIZE;
      tokens = realloc(tokens, buffsize * sizeof(char*));
      if (!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, LSH_TOK_DELIM);
  }

  tokens[position] = NULL;
  return tokens;
}

// Launch shell
int lsh_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0){
    // Child process
    if (execvp(args[0], args) == -1){
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } 
  else if(pid < 0){
    // Fork error
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

// Built-in commands
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_builtin_nums(){
  return sizeof(builtin_str) / sizeof(char *);
}

// Builtin function implementation
int lsh_cd(char **args){
  if (args[1] == NULL){
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_help(char **args){
  printf("Shaurya Tamang's LSH\n");
  printf("The following are built in:\n");

  for (int i = 0; i < lsh_builtin_nums(); i++){
    printf(" %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.");
  return 1;
}

int lsh_exit(char **args){
  return 0;
}

// Execute
int lsh_execute(char **args){
  if (args[0] == NULL){
    return 1;
  } 

  for (int i = 0; i < lsh_builtin_nums(); i++){
    if (strcmp(args[0], builtin_str[i]) == 0){
      return (*builtin_func[i])(args);
    }
  } 

  return lsh_launch(args);
}