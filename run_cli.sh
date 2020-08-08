rm -f err_cli_log.ini

for ((i=0;i<1000;i++))
do
    echo "Running loop "$i
    ./c10k_cli.out 127.0.0.1 $i 2>>err_cli_log.ini & # >/dev/null
    sleep 0.001
done

