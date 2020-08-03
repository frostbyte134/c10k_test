
./c10k_serv.out &
sleep 1


for ((i=0;i<850;i++))
do
    #echo "Running loop "$i
    ./c10k_cli.out 127.0.0.1 >/dev/null &
    #sleep 0.01
done

