#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#include "utils.h"

#define SHELL_BUFFER_SIZE 1024
#define SHELL_TOKEN_BUFFER_SIZE 64
#define SHELL_TOKEN_DELIM " \n"


void shell_loop(void);
char *shell_read_line(void);
char **shell_arguments(char *read_line);
int shell_exec_command(char **args);
int run_shell_command(char **args);

int shell_exit(char **args);

char *builtin_commands[] = { "exit" };

int (*builtin_functions[]) (char **) = { &shell_exit };

int number_shell_builtin_functions() {

  return sizeof(builtin_commands) / sizeof(char *);

}

int arg_size;

int main(int argc, char** argv){
  
  if(argc >  1){
    fprintf(stderr, "%s\n",  "Simple Shell: Program does not accept any command line arguments \n");
    for(int i = 1; i < argc; i++){
      fprintf(stderr, "%s is not a valid argument\n", argv[i]);
    }
	      
    exit(1);
  }

  else {

    shell_loop();
    exit(0);

  }

  exit(0);

}

void shell_loop(void){

  int running;
  char *read_line;
  char *unescape_read_line;
  char **args;

  do {
    arg_size = 0;
    printf("Enter shell Command Please: ");
    
    read_line = shell_read_line();
    
    unescape_read_line = unescape(read_line, stderr);

    free(read_line);
    
    args = shell_arguments(unescape_read_line);
    
    running = run_shell_command(args);
    
    free(unescape_read_line);
    free(args);

  }while (running);
  
}

char *shell_read_line(void){

  char *read_line = NULL;
  size_t buffer_size = 0;

  
  
  if(getline(&read_line, &buffer_size, stdin) == -1) {
        
    if(feof(stdin)) {

      exit(0);
    }

    else {

      fprintf(stderr, "Simple Shell: shell_read_line() error \n");
      exit(1);
    }
  }

  return read_line;
  
}

// TODO: Only parses arguments with white space right now
// Need to understand utils.c and implement better parsing
char **shell_arguments(char* read_line){

  int curr_pos = 0;
  int buffer_size = SHELL_TOKEN_BUFFER_SIZE;
  char **tokens = malloc(buffer_size * sizeof(char*));
  char *curr_token;

  if(!tokens) {

    fprintf(stderr, "Simple Shell: Allocation error \n");
    exit(1);

  }

  curr_token = strtok(read_line, SHELL_TOKEN_DELIM);

  while (curr_token != NULL) {

    tokens[curr_pos] = curr_token;
    curr_pos++;

    if(curr_pos >= buffer_size) {

      buffer_size += SHELL_TOKEN_BUFFER_SIZE;
      tokens = realloc(curr_token, buffer_size * sizeof(char*));

      if(!tokens) {

	fprintf(stderr, "Simple Shell: Allocation error \n");
	exit(1);
      }
    }
    curr_token = strtok(NULL, SHELL_TOKEN_DELIM);
    arg_size++;
  }
  
  tokens[curr_pos] = NULL;
  return tokens;
}

int shell_exec_command(char **args){

  pid_t pid;
  pid_t wpid;
  int running;

  pid = fork();

  if(pid == 0){

    // This should be the child process

    if(execvp (args[0], args) == -1) {

      fprintf(stderr, "Simple Shell: Execute command error \n");
      
      return 0;
    }

    exit(1);

  }

  else if(pid < 0) {
    // This would only happen if the fork errored 
    fprintf(stderr, "Simple Shell: Execute command error \n");

  }

  else {
    
    // This should be the parent process

    do{
      wpid = waitpid(pid, &running, WUNTRACED);

    } while (!WIFEXITED(running) && !WIFSIGNALED(running));
  }

  return 1;

}

int shell_exit(char **args) {
  int digit_bool = 1;
  
  
  // Exits with zero arguments
  if(arg_size == 1) {
     for(int i = 0; i < 1; i++){
	free(args[i]);
      }

    free(args);
    exit(0);
  }

  //Exits with one argument
  else if(arg_size == 2){
    

    if(args[1][0] == '-'){
      fprintf(stderr, "Simple Shell: Exit command cannot be a negative number \n");
    }
    
    if(!isdigit(*args[1])){
      
      for(int i = 0; i < 1; i++){
	free(args[i]);
      }
      
      
      free(args);
      shell_loop();
      
    }
    else {
      
      int return_num = atoi(args[1]);
      
      for(int i = 0; i < 1; i++){
	free(args[i]);
      }
      free(args);
      exit(return_num);
      
      
    }
    
     
  }
  for(int i = 0; i < 1; i++){
      free(args[i]);
  }
  free(args);
  
  shell_loop();
  return 0;
}
  


int run_shell_command(char **args) {

  
  if(args[0] == NULL) {

    return 1;

  }

  for(int i = 0; i < number_shell_builtin_functions(); i++){

    if (strcmp(args[0], builtin_commands[i]) == 0) {

	return (*builtin_functions[i])(args);

    }
  }

  return shell_exec_command(args);
  
}
