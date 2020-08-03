 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h>
#define BUF_LEN 128
char buf[BUF_LEN+1];

void main(int argc, char *argv[])
{
	int s, n;
	struct sockaddr_in server_addr;
	
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{//소켓 생성과 동시에 소켓 생성 유효검사
		printf("can't create socket\n");
		exit(0);
	}

	//서버의 소켓주소 구조체 server_addr을 NULL로 초기화

	server_addr.sin_family = AF_INET;
	//주소 체계를 AF_INET 로 선택
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//32비트의 IP주소로 변환
	server_addr.sin_port = htons(10102);
	
	//daytime 서비스 포트 번호
	printf("[C10K][DWK] trying connect!\n");
	if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{//서버로 연결요청
		printf("can't connect.\n");
		exit(0);
	}
	printf("[C10K][DWK] trying connect!\n");
	const char* buffer = "abcdefghijklmnopqrstuv";
	unsigned int cnt = 0;
	while(1){
		if(cnt % 1000000 == 0)
				printf("sending %s\n", buffer);
		cnt++;
		send(s, buffer, strlen(buffer), 0);
	}
	send(s, buffer, 0, 0);
	close(s);
	printf("[C10K][SERV] closing client\n");
	//사용이 완료된 소켓을 닫기
}