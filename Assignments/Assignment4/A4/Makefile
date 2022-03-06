OFLAGS = -O3 -lm -lpthread
galsim: quadtree.o main.o
	gcc -o galsim quadtree.o main.o $(OFLAGS)

main.o: main.c quadtree.o
	gcc -c main.c $(OFLAGS)

quadtree.o: quadtree.c
	gcc -c quadtree.c  $(OFLAGS)


clean:
	rm ./galsim ./quadtree.o ./main.o
