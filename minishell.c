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
int parseline(char *s, char **argv);

/* ----------- MAIN PROGRAM ------------ */

int main(void) {
    
    char  cmdline[MAXLINE];
    char *argv[MAXARGS];
    int argc;
    int status;
    pid_t pid;

    /* Boucle while infinie pour attendre l'utilisateur rentre une commande -> executer commande*/
    while(1){
        /* Nom de cet mini shell */
        printf("TM-minishell > ");
    }
}
