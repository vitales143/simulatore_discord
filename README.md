# Simulatore Discord Locale

Progetto in C che simula una chat locale ispirata a Discord, usando processi separati e pipe nominate (FIFO).

## Struttura
- Un processo server che riceve e inoltra i messaggi.
- Ogni utente è un processo indipendente.

## Compilazione
gcc server.c -o server gcc utente.c -o utente


## Esecuzione
1. Creare le pipe con `mkfifo`.
2. Avviare il server: `./server`
3. Avviare gli utenti: `./utente <nome>`

## Note
- Ogni utente si avvia con il proprio nome (`mario`, `luigi`, `peach`).
- I messaggi sono pubblici sul canale generale.

## Documentazione
Vedi il file `BRACCINI_server_manuale`.
