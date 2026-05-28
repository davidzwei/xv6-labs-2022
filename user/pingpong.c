#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int pipe1[2]; // parent -> child
    int pipe2[2]; // child -> parent

    char ch = 'x';
    char buffer;

    if(pipe(pipe1) < 0 || pipe(pipe2) < 0){
        printf("pipe creation failed\n");
        exit(1);
    }

    int pid = fork();
    if(pid < 0){
        exit(1);
    }

    if(pid == 0){
        // child
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], &buffer, 1);
        printf("%d: received ping\n", getpid());
        write(pipe2[1], &ch, 1);

        close(pipe1[0]);
        close(pipe2[1]);
        
        exit(0);
    }
    else{
        // parent
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], &ch, 1);
        read(pipe2[0], &buffer, 1);
        printf("%d: received pong\n", getpid());

        close(pipe1[1]);
        close(pipe2[0]);

        wait(0);
        exit(0);
    }

    exit(0);
}