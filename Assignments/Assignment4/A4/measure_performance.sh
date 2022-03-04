make

echo Starting performance measurement

STEPS=100
# N_BODIES="00010 00020 00050 00100 00200 00500 01000 02000 03000 04000 05000"
N_BODIES="01000"
echo Starting variation of number of bodys, steps=$STEPS
for i in $N_BODIES; do
    echo N=$i
    ./galsim $i ../input_data/ellipse_N_$i.gal $STEPS 0.00001 0
done

echo Using compare_gal_files program to check galsim.c
./galsim 2000 ../input_data/ellipse_N_02000.gal 200 0.00001 0
../compare_gal_files/compare 2000 result.gal ../ref_output_data/ellipse_N_02000_after200steps.gal > tmp.txt || exit 1
cat tmp.txt

# cleanup
rm tmp.txt
rm result.gal
make clean