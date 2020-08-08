#ifndef __C10K_PTHREAD_SERV__
#define __C10K_PTHREAD_SERV__
#include "c10k_common.h"
#include <pthread.h>

void pthread_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size);

#endif