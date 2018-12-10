//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <iostream>
//#include <string.h>
//#include <unistd.h>

//void error(const char *msg)
//{
//    perror(msg);
//    exit(0);
//}

//int main(int argc, char *argv[])
//{
//    int sockfd, portno, n;

//    struct sockaddr_in serv_addr;
//    struct hostent *server;

//    char buffer[256];
//    if (argc < 3) {
//        fprintf(stderr,"usage %s hostname port\n", argv[0]);
//        exit(0);
//    }
//    portno = atoi(argv[2]);
//    sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (sockfd < 0)
//        error("ERROR opening socket");
//    server = gethostbyname(argv[1]);
//    printf("hostname: %s\n", argv[1]);
//    if (server == NULL) {
//        fprintf(stderr,"ERROR, no such host\n");
//        exit(0);
//    }
//    bzero((char *) &serv_addr, sizeof(serv_addr));
//    serv_addr.sin_family = AF_INET;
//    bcopy((char *)server->h_addr,
//    (char *)&serv_addr.sin_addr.s_addr,
//    server->h_length);
//    serv_addr.sin_port = htons(portno);
//    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
//        error("ERROR connecting");
//    printf("Please enter the message: ");
//    bzero(buffer,256);
//    fgets(buffer,255,stdin);
//    n = write(sockfd,buffer,strlen(buffer));
//    if (n < 0)
//        error("ERROR writing to socket");
//    bzero(buffer,256);
//    n = read(sockfd,buffer,255);
//    if (n < 0)
//    error("ERROR reading from socket");
//    printf("%s\n",buffer);
//    return 0;
//}

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <fcntl.h>

std::mutex m;                    //for exclusive access to memory
std::condition_variable cv;      //blocks the calling thread until notified to resume
int vari;

void reading(int sockfd, char recvBuff[], int s)
{
    std::unique_lock<std::mutex> lk(m);
    while(vari) cv.wait(lk);
    std::cout << "ready to read"<<std::endl;

    int n;
    while (1)
    {
        std::cout << "read while"<<std::endl;
        n=read(sockfd, recvBuff, s);
        if (n<0)
            std::cout << "cant read" << std::endl;
     }
}

int main(void)
{
    int sockfd = 0,n = 0;
    char recvBuff[1024];
    char buffer[256];
    struct sockaddr_in serv_addr;
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /* create transmitter thread : */
    std::thread threadtransmit;//creates thread threadtransmit
    int s = sizeof(recvBuff)-1;
    threadtransmit = std::thread(reading, sockfd, recvBuff, s); //function reading
    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    {
        std::unique_lock<std::mutex> lk(m);


        memset(recvBuff, '0' ,sizeof(recvBuff));
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
        {
            printf("\n Error : Could not create socket \n");
            return 1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(4000);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
        {
            printf("\n Error : Connect Failed \n");
            return 1;
        }

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /*make non_blocking socket: */
        int flags = fcntl(sockfd, F_GETFL);
        if(flags <0)
        {
            printf("could not get flags on TCP listening socket");
            return -1;
        }
        if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            printf("could not set TCP listening socket to be non-blocking");
            return -1;
        }
        /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

        vari=1;
    }
    cv.notify_one();
    std::cout << "notified"<<std::endl;


    //while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) /* waits to read from the socket*/
    while(1)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error");
        }
        //printf("\n");
        /*write:::*/
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);/* it reads from keyboard of the client */
        n = write(sockfd,buffer,strlen(buffer));/* writes the entered message on the socket */
        if (n < 0)
            printf("ERROR writing to socket");
    }

    /*if( n < 0)
    {
        printf("\n Read Error \n");
    }*/

    return 0;
}
