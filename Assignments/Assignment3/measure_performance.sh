make
echo Starting performance measurement
echo N=100, steps=200
time ./galsim 100 ellipse_N_00100.gal 200 0.00001 0
echo N=100, steps=1000
time ./galsim 100 ellipse_N_00100.gal 1000 0.00001 0
echo N=100, steps=5000
time ./galsim 100 ellipse_N_00100.gal 5000 0.00001 0
make clean