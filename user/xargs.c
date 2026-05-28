#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs command [args...]\n");
    exit(1);
  }

  char buf[512];
  int i = 0;

  while(read(0, &buf[i], 1) > 0){
    if(buf[i] == '\n'){
      buf[i] = 0; // 每行結尾加 '\0'

      char *new_argv[MAXARG];
      for(int j = 1; j < argc; j++){
        new_argv[j-1] = argv[j];
      }
      new_argv[argc-1] = buf;
      new_argv[argc] = 0;

      // fork + exec
      if(fork() == 0){
        exec(argv[1], new_argv);
        fprintf(2, "exec %s failed\n", argv[1]);
        exit(1);
      } else {
        wait(0);
      }

      i = 0; // reset buffer
    } else {
      i++;
    }
  }

}
