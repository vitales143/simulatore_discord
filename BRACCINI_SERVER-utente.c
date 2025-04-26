#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//definisco la lunghezza massima per nome utente e messaggio
#define MAX_NOME 50
#define MAX_TESTO 256

//struttura del messaggio
struct Messaggio {
    char mittente[MAX_NOME];
    char info[MAX_TESTO];
};

int main(int argc, char* argv[]) {
    int fd_in, fd_out;
    struct Messaggio msg;
    char pipe_in[100], pipe_out[100];

    //controllo che venga passato il nome utente
    if (argc!= 2){
        printf("uso: %s <nome_utente>\n", argv[0]);
        exit(1);
    }

    //compongo i nomi delle pipe 
    sprintf(pipe_out, "%s_out", argv[1]);  //pipe dove scrive l'utente
    sprintf(pipe_in, "%s_in", argv[1]);    //pipe dove legge l'utente

    //apro pipe in scrittura per inviare messaggi al server
    fd_out=open(pipe_out, O_WRONLY);
    if (fd_out < 0){
        perror("errore apertura pipe out");
        exit(1);
    }

    //apro pipe in lettura per ricevere messaggi dal server
    fd_in=open(pipe_in, O_RDONLY);
    if (fd_in <0){
        perror("errore apertura pipe in");
        exit(1);
    }

    //fork per separare lettura e scrittura
    if (fork()== 0){
        
        //processo figlio: riceve e stampa i messaggi
        while (read(fd_in, &msg, sizeof(msg))> 0){
            printf("[%s] %s\n", msg.mittente, msg.info);
        }
        exit(0);
    }

    //processo padre: legge da tastiera e invia al server
    while (1){
        
        //fgets legge una riga da tastiera (stdin = standard input = tastiera)
        fgets(msg.info, MAX_TESTO, stdin);

        //strcspn trova la posizione del '\n' e lo sostituisce con '\0' (fine stringa)
        msg.info[strcspn(msg.info, "\n")] = '\0';

        //strcpy copia il nome utente nel campo mittente del messaggio
        strcpy(msg.mittente, argv[1]);

        //scrive l'intera struct nella pipe per inviare il messaggio al server
        write(fd_out, &msg, sizeof(msg));
    }

    return 0;
}
