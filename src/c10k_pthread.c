#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include "c10k_pthread.h"
#include "c10k_msg_proc.h"

struct thread_args{
    int32_t sock_fd;
    uint32_t thread_count;
};

struct sockaddr_in client_addrs[NUM_MAX_CLIENTS]; //global var - assuming single server on sincle app
pthread_t threads[NUM_MAX_CLIENTS];
struct thread_args targs[NUM_MAX_CLIENTS];


void socket_process(void* arg_p){
    printf("starting %u th thread!\n", ((struct thread_args*)arg_p)->thread_count);
    int32_t sock_fd = ((struct thread_args*)arg_p)->sock_fd;
    uint32_t cnt = 0;
    uint8_t local_buffer[1024];
    while(1){
        int msg_size = recv_message(sock_fd, local_buffer, BUF_SIZE);
        if(msg_size <= 0){
            printf("msg_size <= 0, closing socket\n");
            close(sock_fd);
            break;
        }else{
            if(cnt % 1000000 == 0)
                printf("rcvd %s from thread %u\n", local_buffer, ((struct thread_args*)arg_p)->thread_count);
        }
        cnt++;
    }
    printf("terminating %u th thread!\n", ((struct thread_args*)arg_p)->thread_count);
}

void pthread_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size){
    
    uint32_t socket_cnt = 0;
    
    while(1)
    {
        int sock_fd = -1, add_struct_size = 0;
        sock_fd = accept(serv_fd, (struct sockaddr *)&client_addrs[socket_cnt], &add_struct_size);
        if(sock_fd < 0){
            fprintf(stdout, "[SERV][PTHREAD] accept failed with sock_fd = %d, cnt %u\n", sock_fd, socket_cnt);
        }else{
            targs[socket_cnt].sock_fd = sock_fd;
            targs[socket_cnt].thread_count = socket_cnt;
            int32_t tid = pthread_create(&threads[socket_cnt], NULL, socket_process, (void *)&targs[socket_cnt]);
        
            // pthread_create() 으로 성공적으로 쓰레드가 생성되면 0 이 리턴됩니다
            if (tid < 0)
            {
                fprintf(stdout, "thread create error : %d\n", tid);
                exit(0);
            }
            fprintf(stdout, "[SERV][PTHREAD] accepted new socket %u\n", socket_cnt);
            socket_cnt++;
        }
    }
    close(serv_fd);
    printf("[SERV][PTHREAD], closed socket successfully (unreachable, perahps)\n");
}