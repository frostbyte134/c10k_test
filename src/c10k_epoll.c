
#include "c10k_epoll.h"
#include "c10k_msg_proc.h"

#define EPOLL_SIZE (20000u)
struct epoll_event events[EPOLL_SIZE];              // occured events
struct sockaddr_in client_addrs[NUM_MAX_CLIENTS];   // global var - assuming single server on sincle app

void epoll_work_forever(int serv_fd, uint8_t buffer[BUF_SIZE], uint32_t buffer_size){

    struct epoll_event ev;  // ev : for main epoll control?
    
    printf("[SERV][EPOLL]: wating connection request.\n");
    uint32_t cnt = 0;
    
    int32_t epollfd;
    if((epollfd = epoll_create(EPOLL_SIZE)) == -1){
        fprintf(stderr, "[SERV][EPOLL] cannot create epoll with size %u", EPOLL_SIZE);
        exit(-1);
    }
    printf("[SERV][EPOLL]: epoll_create success\n");

    ev.events = EPOLLIN;
    ev.data.fd = serv_fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_fd, &ev);
    printf("[SERV][EPOLL]: epoll_ctl success\n");
    uint32_t socket_cnt = 0;
    while(1)
    {
        cnt++;
        int event_size =  epoll_wait(epollfd, events, EPOLL_SIZE, -1); 
        if(cnt % 1000 == 0){
            //printf("event size = %d\n", event_size);
        }
        if(event_size == -1)
        {
            printf("[SERV][EPOLL]: event_size -1, terminating\n");
            exit(-1);
        }
        for(int i = 0; i<event_size; i++){
            if(events[i].data.fd == serv_fd){
                int sock_fd = 0, add_struct_size = 0;
                sock_fd = accept(serv_fd, (struct sockaddr *)&client_addrs[socket_cnt++], &add_struct_size);
                
                if(sock_fd < 0){
                    fprintf(stderr, "[SERV][EPOLL] accept failed with cnt %u\n", socket_cnt);
                }else{
                    ev.events = EPOLLIN;
                    ev.data.fd = sock_fd;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_fd, &ev);
                    fprintf(stderr, "[SERV][EPOLL] accepting new socket %u\n", socket_cnt);
                }

            }else{
                //if(cnt % 100000 == 0)
                //    printf("[SERV][EPOLL] processed socket %d\n", i);
                int msg_size = recv_message(events[i].data.fd, buffer, BUF_SIZE);
                if(msg_size == 0){
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, events);
                    close(events[i].data.fd);
                    //printf("[SERV][EPOLL] Server : %d client closed.\n", i);
                }
            }
        }
    }
    close(serv_fd);
    printf("[SERV][EPOLL], closed socket successfully (unreachable, perahps)\n");
}