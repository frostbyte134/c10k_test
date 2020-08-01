gcc -o nailbrainz_cli simple_client.c
gcc -o nailbrainz_serv simple_server.c

./nailbrainz_serv 10102 &
sleep 1
./nailbrainz_cli 127.0.0.1 & 


