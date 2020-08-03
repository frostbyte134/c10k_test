#ifndef __C10K_EPOLL_SERV__
#define __C10K_EPOLL_SERV__
#include "c10k_common.h"

void epoll_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE]);

#endif