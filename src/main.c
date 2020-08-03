#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <c10k_select.h>

#define SERV_PORT (24424u)
struct AppObj{
	void (*server_engine)(struct sockaddr_in* p_serv_addr, uint8_t buffer[BUF_SIZE]);
};

uint8_t buffer[BUF_SIZE];

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    int serv_fd;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);
    printf("[C10K][SELECT][SERV] port = %u\n", SERV_PORT);

	struct AppObj appObj = {
		.server_engine = select_work_forever
	};
	
	appObj.server_engine(&server_addr, buffer);

    return 0;
}