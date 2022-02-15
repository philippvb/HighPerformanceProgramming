make
echo Starting performance measurement

# echo Stating variation of steps, N=100
# echo steps=10
# ./galsim 100 ellipse_N_00100.gal 10 0.00001 0
# echo steps=100
# ./galsim 100 ellipse_N_00100.gal 100 0.00001 0
# echo steps=1000
# ./galsim 100 ellipse_N_00100.gal 1000 0.00001 0

echo Starting variation of number of bodys, steps=100
echo N=10
./galsim 10 ellipse_N_00010.gal 100 0.00001 0
echo N=100
./galsim 100 ellipse_N_00100.gal 100 0.00001 0
echo N=500
./galsim 500 ellipse_N_00500.gal 100 0.00001 0
echo N=1000
./galsim 1000 ellipse_N_01000.gal 100 0.00001 0

echo Using compare_gal_files program to check galsim.c
./galsim 100 ellipse_N_00100.gal 200 0.00001 0
./compare_gal_files/compare 100 output_data/ellipse_N_00100.gal ref_output_data/ellipse_N_00100_after200steps.gal > tmp.txt || exit 1
cat tmp.txt
grep pos_maxdiff tmp.txt | grep 00000000 || exit 1


# cleanup
rm tmp.txt
make clean