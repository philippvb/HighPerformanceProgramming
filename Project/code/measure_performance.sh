make

echo Starting performance measurement

I_list="10 20"
J_list="5 10"
for i in $I_list; do
    for j in $J_list; do
        echo i=$i j=$j
        ./qr $i $j
    done
done


# cleanup
make clean