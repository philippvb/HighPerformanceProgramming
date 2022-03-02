make

echo Starting performance measurement

STEPS=100
N_BODIES="00010 00100 01000"
echo Starting variation of number of bodys, steps=$STEPS
for i in $N_BODIES; do
    echo N=$i
    ./galsim $i ../input_data/ellipse_N_$i.gal 200 0.00001 0
done


# ./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0
# ../compare_gal_files/compare 2000 result.gal ../ref_output_data/ellipse_N_02000_after200steps.gal

echo Using compare_gal_files program to check galsim.c
./galsim 500 ../input_data/ellipse_N_00500.gal 200 0.00001 0
../compare_gal_files/compare 500 result.gal ../ref_output_data/ellipse_N_00500_after200steps.gal > tmp.txt || exit 1
cat tmp.txt
grep pos_maxdiff tmp.txt | grep 00000000 || exit 1

# cleanup
rm tmp.txt
rm result.gal
make clean