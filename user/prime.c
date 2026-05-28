#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

__attribute__((noreturn))
void sieve(int left_fd)
{
    int prime;
    if(read(left_fd, &prime, sizeof(prime)) == 0){
        close(left_fd);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p[2];
    pipe(p);

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if(pid == 0){
        close(p[1]);

        close(left_fd);
        sieve(p[0]);
        close(p[0]);
        exit(0);
    }
    else{
        close(p[0]);
        
        int num;
        while(read(left_fd, &num, sizeof(num)) > 0){
            if(num % prime != 0){
                write(p[1], &num, sizeof(num));
            }
        }
        close(left_fd);
        
        close(p[1]);
        wait(0);
        exit(0);
    }
    
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    int pid = fork();
    if(pid<0){
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if(pid == 0){
        close(p[1]);
        sieve(p[0]);
        close(p[0]);
        exit(0);
    }
    else{
        close(p[0]);
        for(int i = 2; i<=35; i++){
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);

        wait(0);
        exit(0);
    }
}
