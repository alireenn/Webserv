# Webserv

Il programma deve accettare un file di configurazione come argomento o utilizzare un percorso predefinito.
- Non si può eseguire un altro server web.
- Il server non deve mai bloccarsi e il client può essere rimbalzato correttamente se necessario.
- Non deve essere bloccante e deve usare solo 1 poll() (o equivalente) per tutte le operazioni di I/O
tra il client e il server (ascolto incluso).
- poll() (o equivalente) deve controllare lettura e scrittura allo stesso tempo.
- Non si deve mai eseguire un'operazione di lettura o scrittura senza passare per poll() (o equivalente).
- È assolutamente vietato controllare il valore di errno dopo un'operazione di lettura o scrittura.
- Non è necessario usare poll() (o equivalente) prima di leggere il file di configurazione.

Dato che si devono usare descrittori di file non bloccanti, è
possibile usare le funzioni read/recv o write/send senza poll()
(o equivalente) e il server non si bloccherebbe.
Ma consumerebbe più risorse di sistema.
Pertanto, se si tenta di leggere/recuperare o scrivere/inviare in un qualsiasi descrittore di file senza usare poll() (o una funzione equivalente), il voto sarà 0.

- È possibile utilizzare tutte le macro e le definizioni come FD_SET, FD_CLR, FD_ISSET, FD_ZERO (capire cosa e come fanno è molto utile).
- Una richiesta al vostro server non deve mai bloccarsi per sempre.
- Il vostro server deve essere compatibile con il browser web di vostra scelta.
- Consideriamo che NGINX è compatibile con HTTP 1.1 e può essere usato per confrontare le intestazioni e le risposte.
- I codici di stato delle risposte HTTP devono essere accurati.
- Il server deve avere pagine di errore predefinite, se non ne vengono fornite.
- Non si può usare il fork per qualcosa di diverso da CGI (come PHP, Python e così via).
- Dovete essere in grado di servire un sito web completamente statico.
- I client devono essere in grado di caricare i file.
- Sono necessari almeno i metodi GET, POST e DELETE.
- Testate il vostro server. Deve rimanere disponibile a tutti i costi.
- Il server deve essere in grado di ascoltare più porte (vedere il file di configurazione).

<h2>LINK UTILI</h2>

- https://github.com/cclaude42/webserv  utile per studiare la teoria
- https://github.com/Mounadi05/42-Webserv  unica repo che va su linux da 125 che sono riuscito a far fungere
