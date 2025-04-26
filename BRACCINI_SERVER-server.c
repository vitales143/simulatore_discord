#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//definisco il numero massimo di utenti, la lunghezza del messaggio e del nome utente
#define MAX_UTENTI 3
#define MAX_NOME 50
#define MAX_TESTO 256

//struttura del messaggio
struct Messaggio {
    char mittente[MAX_NOME];
    char info[MAX_TESTO];
};

//lista fissa utenti (nomi a caso)
char* utenti[MAX_UTENTI]= {"mario", "luigi", "peach"};

int main(){
    int in_fd[MAX_UTENTI];   //pipe dove l'utente scrive e il server legge
    int out_fd[MAX_UTENTI];  //pipe dove l'utente legge e il server scrive

    struct Messaggio msg;

    //apro le pipe di ogni utente
    for (int i= 0; i<MAX_UTENTI; i++){
        char nome_pipe_out[100]; 
        char nome_pipe_in[100];

        //sprintf prende la stringa scritta e la mette in una variabile
        //l'utente inserisce il proprio nome
        sprintf(nome_pipe_out, "%s_out", utenti[i]);
        
        //apre il lato di lettura
        in_fd[i]= open(nome_pipe_out, O_RDONLY);
        
        //se il file descriptor è minore di 0, c'è un errore
        if (in_fd[i] < 0) {
            perror("errore apertura pipe out");
            exit(1);
        }

        //stessa cosa di prima ma per scrittura
        sprintf(nome_pipe_in, "%s_in", utenti[i]);
        out_fd[i]=open(nome_pipe_in, O_WRONLY);
        
        if (out_fd[i] <0){
            perror("errore apertura pipe in");
            exit(1);
        }
    }

    //legge dagli utenti e inoltra il messaggio agli altri
    while (1){
        for (int i=0; i <MAX_UTENTI; i++){
            
            //prende in input il messaggio
            int n=read(in_fd[i], &msg, sizeof(msg));
            
            //controlla se non è vuoto
            if (n> 0){
                printf("server ha ricevuto da %s: %s\n", msg.mittente, msg.info);

                //inoltra a tutti 
                for (int j=0; j<MAX_UTENTI; j++){
                    
                    //controlla di non inoltrare a chi ha scritto il messaggio
                    if (j!=i){
                        write(out_fd[j], &msg, sizeof(msg));
                    }
                }
            }
        }
    }

    return 0;
}
