#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "8112"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main()
{
	int listener, new_fd;
	struct addrinfo hints, *ai, *p;
	
	// khai bao host muon convert
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_prototype = AI_PASSIVE;
	
	
	
	int rv;
	if((rv=getaddrinfo(NULL, PORT, &hint, &ai)) != 0)
	{
		fprintf(stderr, "%s\n", gai_strerror(rv));
		exit(1);
	}
	
	// duyet de tim ra thang co the bind
	
	
	for(p = ai; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_prototype);
		if(listener < 0)
			continue;
		
		{
			int yes = 1;
			setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		}
		
		if(bind(listener, p->ai_addr, p->ai_addrlen) < 0)
			continue;
	}
	
	
	// neu khong co cai nao gan duoc
	if(p == NULL)
	{
		fprintf(stderr, "chon server: that bai de bind\n");
		exit(2);
	}
	
	
	// nghe tin tu socket nay
	if(listen(listener, 10) < 0)
	{
		perror("loi listen\n");
		exit(3);
	}
	
	// them listener vao master set
	fd_set master;
	FD_ZERO(&master);
	FD_SET(listener, &master);
	
	
	int fdmax;
	fd_set read_fds;
	FD_ZERO(&read_fds)
	// max luc nay la listener
	fdmax = listener;
	
	// main loop
	while(1)
	{
		read_fds = master;
		if(select(fdmax + 1, read_fds, NULL, NULL, NULL) == -1)
		{
			perror("loi select\n");
			exit(4);
		}
		
		// tim trong cac ket noi hien tai
		int iii;
		for(iii = 0; iii < = fdmax; iii++)
		{
			if(FD_ISSET(iii, read_fds)) // chinh la nay
			{
				if(iii == listener) // co ket noi moi
				{
					int newfd;
					struct sockaddr_storage remoteaddr;
					socklen_t addrlen;
					char remoteIP[INET6_ADDRSTRLEN];
					addrlen = sizeof remoteaddr;
					newfd = accept(listener, (struct sockaddr*) &remoteaddr, addrlen);
					if(newfd == -1)
					{
						perror("loi accept\n");
						exit(5);
					}
					else
					{
						FD_SET(newfd, &master); // them vao master set
						if(newfd > fdmax)
							fdmax = newfd;
						// thong bao tren console
						printf("Ket noi moi tu %s tren socket %d", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr_storage*) &remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd));
					}
				}
				else // nhan du lieu tu client
				{
					
					int nbytes;
					char buf[256];
					if((nbytes = recv(iii, buf, sizeof buf, 0) <= 0) // ngat hoac bi loi
					{
						if(nbytes == 0)
						{
							// ket noi bi ngat
							printf("socket %d ket thuc\n", iii);
						}
						else
						{
							// loi
							perror("loi recv");
						}
						close(iii);
						FD_ZERO(iii, &master);
					}
					else
					{
						// nhan duoc du lieu
						// gui toi cho moi nguoi
						int jjj;
						for(jjj = 0; jjj <= fdmax; jjj++)
						{
							if(jjj != listener && jjj != iii)
							{
								// gui 
								if(send(jjj, buf, nbytes, 0) == -1)
									perror("loi send");
							}
						}
					}  // END ngat hoac bi loi
				} // END nhan du lieu tu client
								
						
						
				
			} // END chinh la nay
		} // END tim trong ket noi hien tai
		
	} // END main loop
	
	
	
	
	
	
	
	return 0;
}