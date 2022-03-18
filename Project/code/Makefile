flags = -lm -O3 -march=native -ffast-math

qr : qr_serial_opt.c
	gcc-11 -o qr qr_serial_opt.c $(flags)

qr_base : qr.c
	gcc-11 -o qr_base qr.c $(flags)

clean:
	rm qr