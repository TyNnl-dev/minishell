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
#define MAXARGS 20
#define MAX_PATH_LENGTH 5000


/* Fonction prototype */
int parse_line(char *s, char **argv);

/* ----------- MAIN PROGRAM ------------ */

int main(void) {
    
    char  cmdline[MAXLINE];
    char *argv[MAXARGS];
    int path[MAX_PATH_LENGTH];
    int argc;
    int status;
    pid_t pid;

    /* Boucle while infinie pour attendre l'utilisateur rentre une commande -> executer commande*/
    while(1){
        /* Nom de cet mini shell */
        printf("TM-minishell > \n");
        printf("$ ");

        /* Lire entrer command line*/
        fgets(cmdline, MAXLINE, stdin);

        argc = parse_line(cmdline, argv);

        if(argc == 0) {
            continue;
        }

        if(strcmp(argv[0], "exit") == 0){
            return EXIT_SUCCESS;
        }

        if(strcmp(argv[0], "pwd") == 0){
            if (getcwd(path, MAX_PATH_LENGTH)){
                printf("%s\n", path);
            }else {
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

        return count;
    }
