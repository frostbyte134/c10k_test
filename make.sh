gcc -o nailbrainz_cli.out simple_client.c
gcc -o nailbrainz_serv.out simple_server.c

./nailbrainz_serv.out 10102 &
sleep 1
./nailbrainz_cli.out 127.0.0.1 & 


