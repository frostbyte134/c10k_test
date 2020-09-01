 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "include/c10k_common.h"


int sock_desc, cont = 1;
char cli_name[256];

void shutdown_socket(){
	if(cont){
		cont = 0;
		close(sock_desc);
	}
}

void handle_signal_action(int sig_number)
{
  if (sig_number == SIGINT) {
    printf("SIGINT was catched on cli %s\n", cli_name);
    shutdown_socket();
  }
  else if (sig_number == SIGPIPE) {
    printf("SIGPIPE was catched on cli %s\n", cli_name);
    shutdown_socket();
  }
}


int setup_signals()
{
  struct sigaction sa;
  sa.sa_handler = handle_signal_action;
  if (sigaction(SIGINT, &sa, 0) != 0) {
    perror("sigaction()");
    return -1;
  }
  if (sigaction(SIGPIPE, &sa, 0) != 0) {
    perror("sigaction()");
    return -1;
  }
  
  return 0;
}

void main(int argc, char *argv[])
{
	
	if (setup_signals() != 0){
		fprintf(stdout, "cannot setup signal on client %s\n", argv[2]);
		return EXIT_FAILURE;
	}
	strcpy(cli_name, argv[2]);
	struct sockaddr_in server_addr;
	
	if((sock_desc = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{//소켓 생성과 동시에 소켓 생성 유효검사
		printf("can't create socket\n");
		exit(0);
	}

	//서버의 소켓주소 구조체 server_addr을 NULL로 초기화
	srand( time(NULL) );
	server_addr.sin_family = AF_INET;
	//주소 체계를 AF_INET 로 선택
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//32비트의 IP주소로 변환
	server_addr.sin_port = htons(SERV_PORT);
	
	//daytime 서비스 포트 번호
	//printf("[CLI] trying connect to port %u!\n", SERV_PORT);
	if(connect(sock_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{//서버로 연결요청
		fprintf(stdout, "connect() < 0. terminating client %s\n", argv[2]);
		exit(EXIT_SUCCESS);
	}
	fprintf(stdout, "[CLI] %s th cli connect finished!\n", argv[2]);
	const char* buffer = "abcdefghijklmnopqrstuv";
	unsigned int cnt = 0;
	while(cont){
		usleep(10000);
		//for(int i = 0; i<5; i++)
		//	printf("sending %s\n", buffer);	
		cnt++;
		if (rand() % 10000 == 0)
			printf("cli %s : sending %s\n", argv[2], buffer);		
		send(sock_desc, buffer, strlen(buffer), 0);
	}
	close(sock_desc);
	//printf("[CLI] closing client\n");
	//사용이 완료된 소켓을 닫기
}