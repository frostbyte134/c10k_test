rm -f err_serv_log.ini
strace -s 65536 -t -f -o serv.strace ./c10k_serv.out & #>>err_serv_log.ini 2>&1 &
