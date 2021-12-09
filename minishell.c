#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE  4096

int main()
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    while (1){//boucle infinitive
        printf("$ ");  /* print prompt (printf requires %% to print %) */
    if (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0; /* replace newline with null */

        if ((pid = fork()) < 0) {
            perror("fork error");
        } else if (pid == 0) {      /* child */
            execlp(buf, buf, (char *)0);
            perror("couldn′t execute: %s", buf);
            exit(127);
        }

        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            perror("waitpid error");
        printf("%% ");
        }
        exit(0);
    }
    
}
