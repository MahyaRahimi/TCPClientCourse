#include "tcp.h"

int tcp::tcp_connection()
{


    memset(recvBuff, 0 ,sizeof(recvBuff));
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


    //debug
    //std::cout << "read in main"<<std::endl;

   /* int n=4;
    for(int i=0; i<n; i++)
    {
        memset(recvBuff, 0 ,sizeof(recvBuff));
        std::cout << "read while"<<std::endl;
        n=read(sockfd, recvBuff, s);
        if (n<0)
            std::cout << "cant read" << std::endl;
     }
    printf("Here is the received buffer: %s\n",recvBuff);*/


    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /* create transmitter and listening thread : */
    //s = sizeof(recvBuff)-1;
    listener= std::thread(&tcp::listening, this);//function listening
    transmitter = std::thread(&tcp::transmitting, this); //function transmitting
    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    listener.join();
    transmitter.join();
    return 0;


}
void tcp::listening()
{

    std::cout << "ready to read"<<std::endl;

    int n;
    while (1)
    {
        memset(recvBuff, 0 ,sizeof(recvBuff));
        //std::cout << "read while"<<std::endl;
        n=read(sockfd, recvBuff, sizeof(recvBuff)-1);
        if (n<0)
            std::cout << "cant read" << std::endl;
        printf("Here is the server message: %s\n",recvBuff);

     }
}
void tcp::transmitting()
{
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
}
