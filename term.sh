
ps aux | grep c10k_cli | awk '{print $2}' | xargs kill -9
ps aux | grep c10k_serv | awk '{print $2}' | xargs kill -9


