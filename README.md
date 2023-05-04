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
- https://www.youtube.com/watch?v=YwHErWJIh6Y Il tipo che implementa in C++ le cose che sono in [questo](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa "Ma che fico, si possono far apparire cose quando ci passi sopra con il mouse!") articolo

<h3>HTTP</h3>

Un server HTTP (Hypertext Transfer Protocol) è un software che gestisce le richieste di connessione e i trasferimenti di dati tra i client (ad esempio, un browser web) e i vari siti web. Il server HTTP è il componente principale dell'architettura client-server utilizzata sul web.

Quando un client richiede una pagina web, il server HTTP riceve la richiesta, recupera il contenuto richiesto e lo invia al client attraverso il protocollo HTTP. Il server HTTP può anche gestire altri tipi di richieste, come il caricamento di file o l'accesso a servizi web.

In pratica, il server HTTP è il "motore" che consente ai siti web di funzionare e di essere accessibili via Internet.

------------------------------------------------------------------------------------

<h3>Socket</h3>

Socket è un oggetto software che consente la comunicazione tra due processi (o programmi) su una rete, come Internet.

In pratica, un socket funziona come un canale di comunicazione attraverso il quale i processi possono inviare e ricevere dati. Ad esempio, quando un client web richiede una pagina ad un server web, il client e il server utilizzano dei socket per scambiarsi i dati della richiesta e della risposta.

I socket possono essere creati e utilizzati da diversi tipi di programmi, come browser web, server web, programmi di chat, programmi di file sharing, e così via. I socket consentono la comunicazione tra questi programmi su una rete, che può essere Internet o una rete locale.

In sintesi, i socket sono degli strumenti fondamentali per la comunicazione tra programmi su una rete, e consentono di scambiare dati in modo efficiente e sicuro.

--------------------------------------------------------------------------

<h3>CGI</h3>

CGI (Common Gateway Interface) è un protocollo che consente ad un server web di interagire con programmi esterni per generare pagine web dinamiche.

In pratica, quando un browser web richiede una pagina che richiede dati dinamici o interattivi, il server web può utilizzare CGI per inviare una richiesta ad un programma esterno, chiamato script CGI, che elabora i dati e restituisce una risposta al server web. Il server web quindi utilizza questa risposta per generare la pagina web dinamica richiesta dal browser.

Gli script CGI possono essere scritti in diversi linguaggi di programmazione, come Perl, Python, Ruby e altri, e possono essere utilizzati per gestire attività come la generazione di pagine web dinamiche, la raccolta di dati dal browser o la creazione di interfacce utente avanzate.

In sintesi, CGI è un protocollo che consente ad un server web di interagire con programmi esterni per generare pagine web dinamiche e interattive, rendendo possibile l'utilizzo di script CGI per elaborare dati e gestire attività avanzate su un sito web. 

---------------------------------------------------------------------------

<h3>Telnet</h3>

Telnet è un protocollo di rete utilizzato per connettersi e comunicare con un altro computer tramite Internet o una rete locale.

In pratica, Telnet consente di accedere a un computer remoto e di utilizzarlo come se ci si trovasse fisicamente davanti ad esso, consentendo di inviare comandi e ricevere informazioni. Ad esempio, un utente può utilizzare Telnet per connettersi a un server web e recuperare una pagina web.

Telnet è stato ampiamente utilizzato in passato come mezzo di accesso remoto ai computer, ma oggi è stato in gran parte sostituito da protocolli più sicuri, come SSH (Secure Shell).

In sintesi, Telnet è un protocollo di rete che consente di connettersi e comunicare con un altro computer tramite Internet o una rete locale, ma non è considerato un protocollo sicuro per la comunicazione remota.
