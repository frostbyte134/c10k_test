
ps aux | grep nailbrainz_cli | awk '{print $2}' | xargs kill -9
ps aux | grep nailbrainz_serv | awk '{print $2}' | xargs kill -9


