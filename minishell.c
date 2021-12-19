/* Auteur: Lam Cao Tho TRINH
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 100
#define MAXARGS 16

/* Fonction prototype */
int parse_line(char *s, char **argv);
void redirection(int count, char *argv[]);

/* ----------- MAIN PROGRAM ------------ */

int main(void) {
    
    char cmdline[MAXLINE]; //buffer pour fgets
    char *argv[MAXARGS];
    char path[1024];
    int argc;
    int status;
    pid_t pid;

    printf("=====>>> TM-MiniShell <<<===== \n");

    /* Boucle while infinie pour attendre l'utilisateur rentre une commande -> executer commande*/
    while(1){
        /* Nom de cet mini shell */
        
        printf("$ ");

        /* Lire entrer command line*/
        fgets(cmdline, MAXLINE, stdin);

        argc = parse_line(cmdline, argv);

        /* Si la commande passée est exit, alors le shell devra s'arrêter. */
        if(strcmp(argv[0], "exit") == 0){
            printf("Byebye!\n");
            return EXIT_SUCCESS;
        }

        if(argc == 0) {
                continue;
        } else {
            pid = fork();
            if(pid < 0) {
                perror("Fork erreur!");
                exit(EXIT_FAILURE);
            } else if(pid == 0){ //child process
                int out;

                redirection(argc, argv);
                out = execvp(argv[0],argv);

                if (out < 0) {
                    fprintf(stderr, "Exec erreur.\n");
                    exit(0);
                }
            } else { //Parent, shell continue
                if (wait(&status) == -1){
                    perror("Parent fork erreur!!");
                } else {
                    wait(&pid);
                }
            }
        }

        for (int i = 0; i <= argc; i++){
            printf("Argv[%d] = %s\n", i, argv[i]);
        }     
    }
}
            
int parse_line(char *s, char **argv){
        int count = 0;
        char *spaceEnterTab = " \n\t";

        argv[count] = strtok(s, spaceEnterTab);

        while ((argv[count] != NULL) && (count+1 < MAXARGS)){
            argv[++count] = strtok((char *) 0, spaceEnterTab);
        }
        printf("Count =%d\n", count);
        return count;
    }

void redirection(int argc, char *argv[]){
    int out = 0;
    int i;
    int cleanid = 0;
    char *args_clean[argc];

    for(i = 0; i < argc; i++){
        if(!strcmp(argv[i], ">")){
            ++i;
            int out_fd = open(argv[i], O_WRONLY | O_CREAT| O_TRUNC, 0755);
            if(out_fd < 0){
                perror("Redirection erreur");
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
            //argv[out] = NULL;
            continue;
        } 
        args_clean[cleanid++] = argv[i];
    }

    args_clean[cleanid] = NULL;
    execvp(args_clean[0], args_clean);
    fprintf(stderr, "Child erreur exécution.\n");
    exit(0);
}