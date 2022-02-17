make
echo Starting performance measurement


echo Starting variation of number of bodys, steps=100
echo N=10
./galsim 10 input_data/ellipse_N_00010.gal 100 0.00001 0
echo N=20
./galsim 20 input_data/ellipse_N_00020.gal 100 0.00001 0
echo N=50
./galsim 50 input_data/ellipse_N_00050.gal 100 0.00001 0
echo N=100
./galsim 100 input_data/ellipse_N_00100.gal 100 0.00001 0
echo N=200
./galsim 200 input_data/ellipse_N_00200.gal 100 0.00001 0
echo N=500
./galsim 500 input_data/ellipse_N_00500.gal 100 0.00001 0
echo N=1000
./galsim 1000 input_data/ellipse_N_01000.gal 100 0.00001 0
echo N=2000
./galsim 2000 input_data/ellipse_N_02000.gal 100 0.00001 0
echo N=3000
./galsim 3000 input_data/ellipse_N_03000.gal 100 0.00001 0
echo N=5000
./galsim 5000 input_data/ellipse_N_05000.gal 100 0.00001 0
echo N=7000
./galsim 7000 input_data/ellipse_N_07000.gal 100 0.00001 0
echo N=10000
./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 0

echo Using compare_gal_files program to check galsim.c
./galsim 100 input_data/ellipse_N_00100.gal 200 0.00001 0
./compare_gal_files/compare 100 result.gal ref_output_data/ellipse_N_00100_after200steps.gal > tmp.txt || exit 1
cat tmp.txt
grep pos_maxdiff tmp.txt | grep 00000000 || exit 1


# cleanup
rm tmp.txt
rm result.gal
make clean