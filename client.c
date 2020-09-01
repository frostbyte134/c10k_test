 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "include/c10k_common.h"



int sock_desc[NUM_MAX_CLIENTS], cont = 1;

int num_clients = 0;
const int max_clients = 10000;

struct sockaddr_in server_addrs[NUM_MAX_CLIENTS]; //global var - assuming single server on sincle app
char cli_name[256];
const char* buffer = "abc\0";

void client_send_data(){
	const int buffer_size = strlen(buffer);
	uint32_t cnt = 0;
	while(cont){
		cnt++;
		if(!num_clients){
			sleep(0.5);
			continue;
		}
		int ind = rand() % num_clients;
		send(sock_desc[ind], buffer, buffer_size, 0);
		if (rand() % 100000 == 0)
			printf("cli %d : sending %s\n", atoi(cli_name) * max_clients + ind, buffer);		
	}
}

void shutdown_socket(){
	if(cont){
		cont = 0;
		for(int i = 0; i<max_clients; i++){
			close(sock_desc[i]);
		}
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

int main(int argc, char *argv[])
{
	
	if (setup_signals() != 0){
		fprintf(stdout, "cannot setup signal on client %s\n", argv[2]);
		return EXIT_FAILURE;
	}
	srand( time(NULL) );
	

	
	strcpy(cli_name, argv[2]);
	struct sockaddr_in server_addr;
	pthread_t thread;
	int32_t tid = pthread_create(&thread, NULL, client_send_data, NULL);
    
	// pthread_create() 으로 성공적으로 쓰레드가 생성되면 0 이 리턴됩니다
	if (tid < 0)
	{
		fprintf(stdout, "thread create error : %d\n", tid);
		exit(0);
	}
	for(; num_clients<max_clients; num_clients++){
		if((sock_desc[num_clients] = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		{//소켓 생성과 동시에 소켓 생성 유효검사
			printf("can't create socket\n");
			exit(0);
		}
		//서버의 소켓주소 구조체 server_addr을 NULL로 초기화
		server_addr.sin_family = AF_INET;
		//주소 체계를 AF_INET 로 선택
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		//32비트의 IP주소로 변환
		server_addr.sin_port = htons(SERV_PORT);
		if(connect(sock_desc[num_clients], (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{//서버로 연결요청
			fprintf(stdout, "connect() < 0. terminating client %s\n", argv[2]);
			exit(EXIT_SUCCESS);
		}
		fprintf(stdout, "[CLI] %d th cli connect finished!\n", atoi(argv[2]) * max_clients + num_clients);
	}
	int status = 0;
	pthread_join(thread, (void*)&status);
	if(cont)shutdown_socket();
}