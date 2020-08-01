gcc -o nailbrainz_cli.out client.c
gcc -o nailbrainz_serv.out server.c

./nailbrainz_serv.out 10102 &
sleep 1


for ((i=0;i<=1000;i++))
do
    echo "Running loop "$i
    ./nailbrainz_cli.out 127.0.0.1 & 
done


