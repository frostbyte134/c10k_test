
#include "c10k_select.h"
#include "c10k_msg_proc.h"
struct sockaddr_in client_addrs[NUM_MAX_CLIENTS]; //global var - assuming single server on sincle app

void select_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size){
    
    fd_set reads, cpy_reads;
    FD_ZERO(&reads);
    FD_SET(serv_fd, &reads);
    int fd_max = serv_fd; //for each connected clients, assign socket file descriptor
    int fd_num = 0;
    printf("[SERV][SELECT]: wating connection request.\n");
    struct timeval timeout;
    uint32_t cnt = 0, socket_cnt = 0;

    while(1)
    {
        cnt++;
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 50000;
        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
            fprintf(stderr, "[SERV][SELECT] fd_num : %d select() error", fd_num);
            break;
        }
        if(fd_num == 0)
            continue;

        for(int i = 0; i<=fd_max; i++){
            if(FD_ISSET(i, &cpy_reads)){
                if(i == serv_fd){
                    int sock_fd = 0, add_struct_size = 0;
                    sock_fd = accept(serv_fd, (struct sockaddr *)&client_addrs[socket_cnt++], &add_struct_size);
                    FD_SET(sock_fd, &reads);
                    if(fd_max < sock_fd){
                        fd_max = sock_fd;
                    }
                    printf("[SERV][SELECT] accept socket %d!\n", sock_fd);
                    if(sock_fd < 0)
                    {
                        fprintf(stderr, "[SERV][SELECT]: accept failed.\n");
                        exit(-1);
                    }
                }else{
                    //if(cnt % 1000 == 0)
                    //    printf("[SERV][SELECT] processed socket %d\n", i);
                    int msg_size = recv_message(i, buffer, BUF_SIZE);
                    if(msg_size == 0){
                        FD_CLR(i, &reads);
                        close(i);
                        printf("[SERV][SELECT] Server : %d client closed.\n", i);
                    }
                }
            }
        }
    }
    close(serv_fd);
    printf("[SERV][SELECT], closed socket successfully (unreachable, perahps)\n");
}