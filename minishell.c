/* Auteur: Lam Cao Tho TRINH
 * Groupe 22 - Lam Cao Tho TRINH - Quang Minh PHAM
 * No étudiant: Lam Cao Tho TRINH - U21812378
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h> 

#define MAXLINE 100
#define MAXARGS 16

/* Fonction prototype */
int parse_line(char *s, char *argv[]);
void redirection(int count, char *argv[]);
void handler(int num);
void pipe_simple(int count, char *argv[]);

//int pipe_fd[2];

/* ----------- MAIN PROGRAM ------------ */

int main(void) {
    
    char cmdline[MAXLINE]; //buffer pour fgets
    char *argv[MAXARGS];
    int argc;
    int status;
    int i, j, k, m;
    pid_t pid;
    int fd[2];
    pipe(fd);

    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags =  0;
    // //sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);

    printf("\n\t\t\t<=================================================================================================>\n");
    printf("\t\t\t\t                             Groupe 22 - Simple SHELL \n");
    printf("\n");
    /* Boucle while infinie pour attendre l'utilisateur rentre une commande -> executer commande*/
    while(1){
        /* Nom de cet mini shell */
        
        printf("$ ");

        /* Lire entrer command line*/
        fgets(cmdline, MAXLINE, stdin);

        argc = parse_line(cmdline, argv);

        for (int i = 1; i < argc-1; i++) {
            if(!strcmp(argv[i], "|")){
                printf("i = %d\n", i);
                j = i;
            }
        }

        /* Si la commande passée est exit, alors le shell devra s'arrêter. */
        if(strcmp(argv[0], "exit") == 0){
            printf("Byebye!\n");
            exit(1);
        }

        //Simple implement cd
        if(strcmp(argv[0], "cd") == 0){
            //Si unique "cd" -> HOME
            if(argv[1] == NULL){
                chdir(getenv("HOME"));
                continue;
            } else {
                //cd .. et cd dir ok
                if(chdir(argv[1]) == -1){
                    printf("%s: No such directory\n", argv[1]);
                }
                continue;
            }
        }

        if(argc == 0) {
            continue;
        } else {
            pid = fork();

            if(pid < 0){
                perror("Fork erreur!");
                exit(EXIT_FAILURE);
            }            
          
            if(pid == 0){ //child process
                int out;
                
                redirection(argc, argv);
                out = execvp(argv[0],argv);

                if (out < 0) {
                    fprintf(stderr, "Exec erreur.\n");
                    exit(0);
                }
            } else { //Parent, shell continue
                if (wait(&status) == -1){
                    perror("Parent fork erreur!!\n");
                } else {
                    wait(&pid);
                }
            }

            for (int i = 0; i <= argc; i++){
                printf("Argv[%d] = %s\n", i, argv[i]);
            }     
        }
    }     
}
            
int parse_line(char *s, char *argv[]){
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
            //argv[i] = NULL;
            continue;
        } 
        args_clean[cleanid++] = argv[i];
    }

    args_clean[cleanid] = NULL;
    printf("clean id = %d", cleanid);
    /* Exécution au parent*/
    execvp(args_clean[0], args_clean); 
    fprintf(stderr, "Mauvaise commande!.\n");
    exit(0);
}

void pipe_simple(int argc, char *argv[]){
    int fd[2];
    pipe(fd);
    pid_t pid1;
    int i, j =0, k = 0;
    bool a = false;
    char *lCmd[argc];
    char *rCmd[argc];
    for(i = 0; i < argc; i++){
        if(!strcmp(argv[i], "|")){
            a = true;
            ++i;
            rCmd[j] = argv[i];
            printf("rCmd[%d] = %s\n", j, rCmd[j]);
            ++j;  
            continue;
        }
        if (!a){
            lCmd[i] = argv[i];        
            printf("lCmd[%d] = %s\n", i, lCmd[i]);
        } else {
            rCmd[j] = argv[i];
            printf("rCmd[%d] = %s\n", j, rCmd[j]);
        }
    }
    
    
    if(a){
        pid1 = fork();

        if (pid1 == 0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execlp(lCmd[0], lCmd[0], lCmd, (char*) NULL);
            fprintf(stderr, "Failed to execute '%s'\n", lCmd[0]);
            exit(1);
        } else {
            pid1 = fork();

            if(pid1==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[1]);
                close(fd[0]);
                execlp(rCmd[0], rCmd[0], rCmd, (char*) NULL);
                fprintf(stderr, "Failed to execute '%s'\n", rCmd[0]);
                exit(1);
            } else {
                int status;
                close(fd[0]);
                close(fd[1]);
                waitpid(pid1, &status, 0);
            }

        }
    }
}

void handler(int num){
    printf("Ignore Ctrl-C!\n");
}

