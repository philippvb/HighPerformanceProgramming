gcc-11 -o qr_serial_cache qr_serial_opt_cache_opt.c -lm -O3 -march=native -ffast-math -fopenmp
gcc-11 -o qr_serial qr_serial_opt.c -lm -O3 -march=native -ffast-math -fopenmp

echo Starting performance measurement


# J_list="10 100 500 1000"
# I_list="1000"
J_list="10"
I_list="10"
for i in $I_list; do
    for j in $J_list; do
            echo i=$i j=$j
            echo normal
            ./qr_serial $i $j
            echo cache
            ./qr_serial_cache $i $j            
    done
done


# cleanup
make clean