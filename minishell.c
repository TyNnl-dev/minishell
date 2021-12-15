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
#define MAX_PATH_LENGTH 1024


/* Fonction prototype */
int parse_line(char *s, char **argv);

/* ----------- MAIN PROGRAM ------------ */

int main(void) {
    
    char  cmdline[MAXLINE];
    char *argv[MAXARGS];
    char path[MAX_PATH_LENGTH];
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

        if(argc == 0) {
            continue;
        }

        /* Si la commande passée est exit, alors le shell devra s'arrêter. */
        if(strcmp(argv[0], "exit") == 0){
            return EXIT_SUCCESS;
        }

        /* Si la commande passée est pwd, alors le shell devra afficher le répertoire de travail courrnt */
        if(strcmp(argv[0], "pwd") == 0){
            if (getcwd(path, MAX_PATH_LENGTH)){
                printf("%s\n", path);
            } else {
                printf("Ne pas afficher le répertoire de travail courant.\n");
            }
            continue;
        }




    }

    
}

int parse_line(char *s, char **argv){
        int count = 0;
        char *spaceEnterTab = " \n\t";

        argv[count] = strtok(s, spaceEnterTab);

        while ((argv[count] != NULL) && (count < MAXARGS -1)){
            argv[++count] = strtok((char *) 0, spaceEnterTab);
        }

        argv[MAXARGS-1] = NULL;

        for (int i = 0; i <= count; i++){
            printf("argv[%d] = %s\n" , i, argv[i]);
        }

        return count;
    }
