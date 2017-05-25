/*
 * Recv.c
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
#define BACKLOG 5
#define AIR_RECV_SERV_PORT 9400

int sockfd_serv, sockfd_client;
socklen_t sin_size;

void Recv_init()
{

	struct sockaddr_in serv_addr;


	if(-1 == (sockfd_serv = socket(AF_INET,SOCK_STREAM,0))){
		perror("error in create socket\n");
		exit(0);
	}

	 //Enable address reuse
	 int reuse = 1;
	 if (setsockopt(sockfd_serv, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
		 perror("set sockopt error\n");
	 }
	 else{
		 printf("Set socket successfully! \n");
	 }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(AIR_RECV_SERV_PORT);//server listening port
	serv_addr.sin_addr.s_addr = INADDR_ANY;//here is the specia in listening tcp connect
	bzero(&serv_addr.sin_zero,8);

	//bind
	if(-1 == bind(sockfd_serv,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))){
		perror("bind error\n");
		exit(0);
	}

	//listen
	if(-1 == listen(sockfd_serv,BACKLOG)){
		perror("listening");
		exit(1);
	}
	printf("---The server is listening--- \n");
	printf("---The server has been initialed--- \n");


}
void Recv_main()
{

	struct sockaddr_in client_sockaddr;
	int recvbytes;
	char recv_buf[MAXBUFSZ];
	char *p = &recv_buf[0];

	while(-1 == (sockfd_client = accept(sockfd_serv,(struct sockaddr*)&client_sockaddr,&sin_size))){
		perror("accept");
		usleep(1e5);
	}

	while(1){
		memset(recv_buf,0x00,sizeof(recv_buf));
		recvbytes = recv(sockfd_client, recv_buf, MAXBUFSZ, 0);
		if(recvbytes > 0){
			printf(" server receive %d bytes data \n", recvbytes);
			getchar();
		}
		else if(recvbytes <= 0){
			perror("server receive:recvbytes <= 0");
			close(sockfd_client);
		}
	}
	close(sockfd_client);
	close(sockfd_serv);
}

int main()
{
	Recv_init();
	Recv_main();
}
