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
#include <signal.h>

#define MAXLINE 100
#define MAXARGS 16

/* Fonction prototype */
int parse_line(char *s, char *argv[]);
void redirection(int count, char *argv[]);
void handler(int num);
void pipe_simple(int count, char *argv[]);
void exec1();
void exec2();

//int pipe_fd[2];

/* ----------- MAIN PROGRAM ------------ */

int main(void) {
    
    char cmdline[MAXLINE]; //buffer pour fgets
    char *argv[MAXARGS];
    int argc;
    int status;
    int i;
    pid_t pid;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags =  0;
    //sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);

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
            int pipe_fd[2];
            pipe(pipe_fd);

            if(pipe(pipe_fd) == -1){
                perror("Pipe erreur!\n");
                exit(1);
            }

            pid = fork();

            if(pid < 0) {
                perror("Fork erreur!");
                exit(EXIT_FAILURE);
            }

        // for (i = 0; i <= argc; i++) {
        // if(!strcmp(argv[i], "|")){
        //     if(!fork()){
        //         dup2(pipe_fd[1], 1);
        //         execlp("ls", "ls", NULL);
        //         perror("Exec erreur.\n");
        //         abort();
        //     }
        //         dup2(pipe_fd[0], 0);
        //         close(pipe_fd[1]);
        //     }
        //     execlp("wc", "wc", "-w", NULL);
        //     perror("Exec erreur.\n");
        // }
          
        

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
    /* Exécution au parent*/
    execvp(args_clean[0], args_clean); 
    fprintf(stderr, "Mauvaise commande!.\n");
    exit(0);
}

// void pipe_simple(int argc, char *argv[]){
//     int i;
    
// }

// void exec1(){
//     dup2(pipe_fd[1], 1);
//     close(pipe_fd[0]);
//     close(pipe_fd[1]);

//     execlp("ls", "ls", "-l", NULL);

//     perror("Exec ls erreur!\n");
//     _exit(1);

// }

// void exec2(){
//     dup2(pipe_fd[0], 0);

//     close(pipe_fd[0]);
//     close(pipe_fd[1]);


//     execlp("wc", "wc", "-w", NULL);

//     perror("Exec wc erreur!\n");
//     _exit(1);

// }


void handler(int num){
    printf("Ignore Ctrl-C!\n");
}

