make

echo Starting performance measurement

# I_list="10 100 1000 2000 4000"
# J_list="1000"
# T_list="4"
# for i in $I_list; do
#     for j in $J_list; do
#         for t in $T_list; do
#             echo i=$i j=$j, t=$t
#             ./qr $i $j $t
#         done
#     done
# done

# I_list="10 100 200 400 1000 2000 3000 4000"
J_list="10 100 200 400 3000"
I_list="1000"
for i in $I_list; do
    for j in $J_list; do
            echo i=$i j=$j
            ./qr $i $j 4
    done
done


# cleanup
make clean