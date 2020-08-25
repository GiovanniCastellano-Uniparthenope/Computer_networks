#include "clientheader.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        RED printf("Need M-node host IP as parameter\n");
        exit(1);
    }
    int choice=1, request, iterations=1, disconnect=FALSE, n;
    matchblock IPPort;
    char input[2];
    block received;
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, Exit);
    signal(SIGQUIT, Exit);
    sockfd=TCPConnection(15000, argv[1]);
    if(sockfd<0)
    {
        YELLOW printf("Waiting for an M-node\n");
        do
        {
            sleep(2);
            sockfd=TCPConnection(15000, argv[1]);
        }while(sockfd<0);
    }
    GREEN printf("Connection successful!\n");
    while(choice>=1&&choice<=4)
    {
        WHITE
        WHITE printf("==============================%d===============================\n", iterations++);
        BLUE printf("Selezionare una scelta (1-4)\n");
        WHITE printf("1) Visualizza le ultime N transazioni\n"
               "2) Visualizza info di una transazione\n"
               "3) Visualizza il totale transitato\n"
               "4) Visualizza tutte le transazioni di un IP\n"
               "?) Qualsiasi altro valore chiude il client\n"
               "Scelta: ");
        fflush(stdin);
        scanf("%d", &choice);
        switch(choice)
        {
            case 1 : 
                request=NTRANSACTIONS;
                BLUE printf("Inserire quante transazioni si vuole richiedere: ");
                fflush(stdin);
                scanf("%d", &n);
                if(n<1)
                {  
                    RED printf("Non ci sono transazioni da mostrare\n");
                    break;
                }
                if(Write(sockfd, &request, sizeof(int))<0)
                    disconnect=TRUE;
                if(Write(sockfd, &n, sizeof(int))<0)
                    disconnect=TRUE;
                if(Read(sockfd, &n, sizeof(int))<0)
                        disconnect=TRUE;
                if(n>0)
                {
                    BLUE printf("ID-TM-TRANSACTION (blocchi disponibili %d)\n", n);
                    for(int i=0; i<n; i++)
                    {
                        if(Read(sockfd, &received, sizeof(block))<0)
                            disconnect=TRUE;
                        WHITE printf("%2d-%2d-%s\n", received.id, received.time, received.str);
                    }
                }
                else
                {RED printf("Non ci sono transazioni da mostrare\n");}
                break;
            case 2 : 
                request=IDINFO;
                BLUE printf("Inserire l'ID della transazione da richiedere: ");
                fflush(stdin);
                scanf("%d", &n);
                if(n<1)
                {
                    RED printf("ID invalido (accettati valori 1+)\n");
                    break;
                }
                if(Write(sockfd, &request, sizeof(int))<0)
                    disconnect=TRUE;
                if(Write(sockfd, &n, sizeof(int))<0)
                    disconnect=TRUE;
                if(Read(sockfd, &received, sizeof(block))<0)
                    disconnect=TRUE;
                BLUE printf("ID-TM-TRANSACTION\n");
                WHITE printf("%2d-%2d-%s\n", received.id, received.time, received.str);
                break;
            case 3 : 
                request=TOTAL;
                if(Write(sockfd, &request, sizeof(int))<0)
                    disconnect=TRUE;
                if(Read(sockfd, &n, sizeof(int))<0)
                    disconnect=TRUE;
                BLUE printf("Il totale transitato e': %d\n", n);
                break;
            case 4 : 
                BLUE printf("Inserire l'IP:Porta (es: 255.255.255.255:99999:)\n");
                BLUE printf("Per evitare un esito negativo nel confronto, si prega di rispettare entrambi i \":\"\nfondamentali al server per identificare la corrispondenza\n");
                WHITE getchar();
                fgets(IPPort.IP, 24, stdin);
                IPPort.IP[24]='\0';
                WHITE printf("Letto %s \ncorretto? (S/n)\n", IPPort.IP);
                fflush(stdin);
                fgets(input, 2, stdin);
                if((input[0] DOWNCASE)=='s')
                {
                    request=IPPORTINFO;
                    if(Write(sockfd, &request, sizeof(int))<0)
                        disconnect=TRUE;
                    if(Write(sockfd, &IPPort, sizeof(matchblock))<0)
                        disconnect=TRUE;
                    if(Read(sockfd, &n, sizeof(int))<0)
                        disconnect=TRUE;
                    if(n>0)
                        {BLUE printf("ID-TM-TRANSACTION\n");}
                    else
                        {RED printf("Nessuna transazione trovata\n");}
                    for(int i=0; i<n; i++)
                    {
                        if(Read(sockfd, &received, sizeof(block))<0)
                                disconnect=TRUE;
                        WHITE printf("%2d-%2d-%s\n", received.id, received.time, received.str);
                    }
                }
                else
                {
                    RED printf("Operazione annullata\n");
                }
                break;
            default: 
                request=DISCONNECT;
                Write(sockfd, &request, sizeof(int)); 
                WHITE printf("==============================================================\n");
                break;
        }
        if(disconnect==TRUE)
        {
            RED ("M-node unreachable\n");
            YELLOW printf("Waiting for a new M-node\n");
            do
            {
                sleep(2);
                sockfd=TCPConnection(15000, "127.0.0.1");
            }while(sockfd<0);
            disconnect=FALSE;
            GREEN printf("Connected!\n");
        }
    }
}









