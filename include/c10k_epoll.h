#ifndef __C10K_EPOLL_SERV__
#define __C10K_EPOLL_SERV__
//system includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

//local file includes
#include "c10k_common.h"

void epoll_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size);

#endif