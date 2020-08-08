#ifndef __C10K_MSG_PROC__
#define __C10K_MSG_PROC__
#include "c10k_common.h"

int send_message(int sock_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size);
int recv_message(int sock_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size);

#endif