
./c10k_serv.out &
sleep 1


for ((i=0;i<=1000;i++))
do
    echo "Running loop "$i
    ./c10k_cli.out 127.0.0.1 & 
done

