#ifndef __C10K_SELECT_SERV__
#define __C10K_SELECT_SERV__
#include "c10k_common.h"

void select_work_forever(struct sockaddr_in* p_serv_addr, uint8_t buffer[BUF_SIZE]);
#endif