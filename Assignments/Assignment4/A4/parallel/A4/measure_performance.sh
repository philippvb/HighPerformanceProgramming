make

echo Starting performance measurement

STEPS=100
BODIES="01000"
N_THREADS="1 2 3 4 5 6 7 8 9 10 15 20"
SUBDIR=../../../
echo Starting variation of number of threads, steps=$STEPS, bodies=$BODIES
for i in $N_THREADS; do
    echo N=$i
    ./galsim $BODIES ${SUBDIR}input_data/ellipse_N_$BODIES.gal $STEPS 0.00001 0 $i
done


echo Using compare_gal_files program to check galsim.c
./galsim 2000 ${SUBDIR}input_data/ellipse_N_02000.gal 200 0.00001 0 7
${SUBDIR}compare_gal_files/compare 2000 result.gal ${SUBDIR}ref_output_data/ellipse_N_02000_after200steps.gal > tmp.txt || exit 1
cat tmp.txt

# cleanup
rm tmp.txt

rm result.gal

make clean