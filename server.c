#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <sys/select.h>
//소켓 프로그래밍에 사용될 헤더파일 선언
 
#define BUF_LEN 128
//메시지 송수신에 사용될 버퍼 크기를 선언
 
int main(int argc, char *argv[])
{
    char buffer[BUF_LEN];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    int server_fd, client_fd;
    int len, msg_size;
 
    if(argc != 2)
    {
        printf("usage : %s [port]\n", argv[0]);
        exit(0);
    }
 
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// 소켓 생성
        printf("Server : Can't open stream socket\n");
        exit(0);
    }
    memset(&server_addr, 0x00, sizeof(server_addr));
    //server_Addr 을 NULL로 초기화
 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    printf("[C10K][SERV] port =%s\n", argv[1]);
    //server_addr 셋팅
 
    printf("[C10K][SERV] trying bind!\n");
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
    {//bind() 호출
        printf("Server : Can't bind local address.\n");
        exit(0);
    }
    

	printf("[C10K][SERV] bind finished!\n");
    if(listen(server_fd, 5) < 0)
    {//소켓을 수동 대기모드로 설정
        printf("Server : Can't listening connect.\n");
        exit(0);
    }
	
	fd_set reads, cpy_reads;
	FD_ZERO(&reads);
	FD_SET(server_fd, &reads);
	int fd_max = server_fd; //for each connected clients, assign socket file descriptor
	int fd_num = 0;
    memset(buffer, 0x00, sizeof(buffer));
    printf("Server : wating connection request.\n");
    len = sizeof(client_addr);
	struct timeval timeout;
	unsigned int cnt = 0;
    while(1)
    {
		cnt++;
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 50000;
		if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
			printf("fd_num : %d\n", fd_num);
			perror("select() error");
			break;
		}
		if(fd_num == 0)
			continue;

		for(int i = 0; i<=fd_max; i++){
			if(FD_ISSET(i, &cpy_reads)){
				if(i == server_fd){
					client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
					FD_SET(client_fd, &reads);
					if(fd_max < client_fd){
						fd_max = client_fd;
					}
					printf("[C10K][SERV] accept!\n");
					if(client_fd < 0)
					{
						perror("Server: accept failed.\n");
						exit(0);
					}
					printf("connected socket %d\n", client_fd);
				}else{
					if(cnt % 1000 == 0)
						printf("processed socket %d\n", i);
					msg_size = read(i, buffer, 1024);
					if(msg_size == 0){
						FD_CLR(i, &reads);
				        close(i);
						printf("Server : %d client closed.\n", i);
					}
				}
			}
		}
    }
    close(server_fd);
    return 0;
}