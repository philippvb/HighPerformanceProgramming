compiler = gcc-11
flags = -lm -O3 -march=native -ffast-math -fopenmp

# the final version, serial and paralle optimized
qr : qr.c
	$(compiler) -o qr qr.c $(flags)

# serial optimized version
qr_serial : qr_serial.c
	$(compiler) -o qr_serial qr_serial.c $(flags)

# the base version
qr_base : qr_base.c
	$(compiler) -o qr_base qr_base.c $(flags)

clean:
	rm qr
	rm qr_serial
	rm qr_base