//
//  main.cpp
//  TCP
//
//  Created by Jason Lo on 2016/5/16.
//  Copyright © 2016年 Jason Lo. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct hostent *gethostbyname(const char *name);
/*struct hostent {
    char *h_name;
    char **h_aliases;
    int h_addrtype;
    int h_length;
    char **h_addr_list;
};*/

int main( int argc, char *argv[] ) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 1234;
    
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    
    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */
    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(1){           //continuously listen to the client
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }
    
        /* If connection is established then start communicating */
        bzero(buffer,256);
        n = read( newsockfd,buffer,255 );
        
        /* Write a response to the client */
        //第一種功能
        if(*buffer=='1'){
            n=write(newsockfd,"Input URL Address ",18);

            bzero(buffer,256);
            n = read( newsockfd,buffer,255 );
            int i;
            struct hostent *he;
            struct in_addr **addr_list;
            struct in_addr addr;
            char blank[1000];
            int c = 0, d = 0;
            
            /* 取得命令后第一个参数，即要解析的域名或主机名 */
            /* 把buffer resize志剛剛好沒有空格的大小 */
            while (buffer[c] != '\n') {
                if (buffer[c] == ' ') {
                    int temp = c + 1;
                    if (buffer[temp] != '\0') {
                        while (buffer[temp] == ' ' && buffer[temp] != '\0') {
                            if (buffer[temp] == ' ') {
                                c++;
                            }
                            temp++;
                        }
                    }
                }
                blank[d] = buffer[c];
                c++;
                d++;
            }
            
            blank[d] = '\0';
            
            /* 调用gethostbyname()。调用结果都存在hptr中 */
            he = gethostbyname(blank);
            if (he == NULL) { // do some error checking
                herror("gethostbyname"); // herror(), NOT perror()
                exit(1);
            }
            
            // print information about this host:
            printf("Official name is: %s\n", he->h_name);
            printf("IP address: %s\n", inet_ntoa(*(struct in_addr*)he->h_addr));
            printf("All addresses: ");
            addr_list = (struct in_addr **)he->h_addr_list;
            for(i = 0; addr_list[i] != NULL; i++) {
                printf("%s ", inet_ntoa(*addr_list[i]));
            }
            printf("\n");
            // 把 IP address 在client端印出來
            n=write(newsockfd,inet_ntoa(*(struct in_addr*)he->h_addr),18);
        }
        //第二種功能
        else if(*buffer=='2'){
            n=write(newsockfd,"Input Student ID ",18);
            bzero(buffer,256);
            //matching the student ID and print it on client
            n = read(newsockfd, buffer, 100);
            
            Map(newsockfd, newsockfd);
            
            n = write(newsockfd, buffer, 100);
            n = read( newsockfd,buffer,255 );
        }
        //第三種功能：關閉
        else if(*buffer=='3'){
            close(newsockfd);
        }
        //提醒沒有這種指令
        else{
            bzero(buffer,256);
            n = write(newsockfd,"No such command \n",16);
        }
        
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
    
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
        
        close(newsockfd);
    }
    return 0;
}

void Map(char *ID, char *email)
{
    int IDpool[10], i=0, numData;
    char emailpool[10][100];
    FILE *tr;
    //// read file from query.txt to get email-ID info
    tr = fopen("query.txt","r");
    if( tr==NULL ){
        printf("fail to read query.txt.\n");
    }
    while( fscanf(tr, "%d", &IDpool[i])!=EOF ){
        fscanf(tr, "%s", emailpool[i]);
        i++;
        if( i>=10 ){
            printf("No enough memory to store email map from query.txt.\n");
            break;
        }
    }
    numData = i;
    fclose(tr);
    //// map ID to email
    for(i=0; i<numData; i++){
        if( atoi(ID)==IDpool[i] ){
            strncpy(email,emailpool[i],100);
            return;
        }
    }
    //// ID not found
    strncpy(email,"No such student ID",100);
}


