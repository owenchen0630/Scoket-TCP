/*
 * Send.c
 *
 *  Created on: Apr 5, 2017
 *      Author: duser
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/wait.h>

#define MAXBUFSZ 512
#define PORT 8600

int sockfd_client;
struct sockaddr_in servaddr;
char buff[MAXBUFSZ] = "ABCDEFG";

void Send_init()
{
	char dest_ip[20] = "127.0.0.1";
	if((sockfd_client = socket(AF_INET , SOCK_STREAM , 0)) == -1){
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);

	if(inet_pton(AF_INET , dest_ip , &servaddr.sin_addr) < 0){
		printf("inet_pton error for %s\n",dest_ip);
		exit(1);
	}

	printf("---- Client initial successfully ----\n");
}

void Send_main()
{
	int send_len;

	if( connect(sockfd_client, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("connect error");
		exit(1);
	}


	send_len = send(sockfd_client, buff, MAXBUFSZ, 0);
	if( send_len > 0 ){
		printf("sever send %d bytes successfully\n", send_len);
	}
	else{
		printf("Send message error\n");
	}

	close(sockfd_client);
}


int  main()
{
	Send_init();
	Send_main();
	return 1;
}

