//
//  client.cpp
//  TCP
//
//  Created by Jason Lo on 2016/6/19.
//  Copyright © 2016年 Jason Lo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

int main(void) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    int temp=0;

    /* Create a socket point */
    while(temp!=3){
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_family = PF_INET;
    portno = 1234;
    serv_addr.sin_port = htons(portno);
    
    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    connect (sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
    /* Now ask for a message from the user, this message
     * will be read by server
     */
    
        printf("What's your requirements? 1.DNS 2.QUERY 3.QUIT :");
    
        bzero(buffer,256);
        fgets(buffer,255,stdin);
    
        temp=0;
        temp= atoi(buffer);
    
        /* Send message to the server */
        n = write(sockfd, buffer, strlen(buffer));
    
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
  
    
        /* Now read server response */
        bzero(buffer,256);
        n = read(sockfd, buffer, 255);
    
            if (n < 0) {
                perror("ERROR reading from socket");
                exit(1);
            }
    
        printf("%s",buffer);            //顯示第一次server傳回來的值
    
            if(temp==1){
                
                bzero(buffer,256);
                fgets(buffer,255,stdin);
        
                n = write(sockfd, buffer, strlen(buffer));      //傳值到server
        
                bzero(buffer,256);
                n = read(sockfd, buffer, 255);  //回收回傳值
        
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                printf("address get from domain name : ");
                printf("%s\n\n",buffer);
                
            }
            if(temp==2){
                bzero(buffer,256);
                fgets(buffer,255,stdin);
                
                n = write(sockfd, buffer, strlen(buffer));    //傳值到server
                
                bzero(buffer,256);
                n = read(sockfd, buffer, 255);  //回收回傳值
                
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                printf("Email get from server : ");
                printf("%s\n\n",buffer);
            }
    }
    return 0;
}