#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <arpa/inet.h>

#define PORT "9034"
#define BUFSIZE 1024

void *get_in_addr(struct sockaddr *sa);
void sendAndRecv(int iii, int sockfd);


int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int sockfd;
	

	
	int rv;

    	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
    	}	

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	// hints.ai_flags = AI_PASSIVE;
	
	// chuyen doi thong tin ve host server sang dang huu ich
	if((rv = getaddrinfo(argv[1], PORT, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// lap ket qua va ket noi den cai dau tien co the
	for(p = res; p != NULL; p = p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("loi server: socket");
			continue;
		}
		
		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("loi connect");
			continue;
		}
		
		break;
	}

	// printf("Ket noi thanh cong voi server\n");
	
	// neu khong tim ra cai nao phu hop
	if(p == NULL)
	{
		fprintf(stderr, "loi select server: that bai ket noi\n");
		return 2;
	}
	
	
	char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof s);
	printf("Client: dang ket noi voi %s\n", s);
	freeaddrinfo(res);
	
	// tao fd_set master va du bi
	fd_set master;
	fd_set readfds;
	FD_ZERO(&master);
	FD_ZERO(&readfds);
	
	FD_SET(0, &master);
	FD_SET(sockfd, &master);

	int fdmax = sockfd;

	
	while(1)
	{
		readfds = master;
		if(select(fdmax+1, &readfds, NULL, NULL, NULL) == -1)
		{
			perror("loi select");
			exit(3);
		}
		int iii;

		// printf("Phat hien tin hieu\n");
		for(iii = 0; iii <= fdmax; iii++)
		{
			if(FD_ISSET(iii, &readfds)) // iii la noi phat tin hieu
			{
				// printf("Tin hieu tu %d\n", iii);
				sendAndRecv(iii, sockfd);
			}
		} // END for
	} // END while
	
	close(sockfd);
	
		
		
	
	
	return 0;
}
	
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
	
}

void sendAndRecv(int iii, int sockfd)
{
	char buf[BUFSIZE];
	if(iii == 0) // tin hieu tu ban phim
	{
		fgets(buf, BUFSIZE, stdin);
		buf[strcspn(buf, "\n")] = '\0';
		
		if(strcmp(buf, "quit") == 0) // neu quit thi dong chat
		{
			close(sockfd);
			exit(0);
		}
		else // nguoc lai thi gui cho server
		{
			
			if(send(sockfd, buf, BUFSIZE, 0) == -1)
			{
				perror("loi send");
				exit(1);
			}
		}
		memset(buf, 0, sizeof buf); 
	} // END tin hieu tu ban phim
	else // tin hieu tu server
	{
		// printf("Tin nhan tu server\n");
		int nbytes;
		if((nbytes = recv(sockfd, buf, sizeof buf, 0)) == -1)
		{
			perror("loi recv");
			exit(2);
		}
		buf[nbytes] = '\0';
		printf("%s\n", buf);
		memset(buf, 0, sizeof buf); 
	}
}


		
	
			

	
	
	
	