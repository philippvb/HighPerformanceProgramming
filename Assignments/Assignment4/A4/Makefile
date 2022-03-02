main: quadtree.o main.o
	gcc -o galsim quadtree.o main.o -Wall

main.o: main.c quadtree.o
	gcc -c main.c -Wall

quadtree.o: quadtree.c
	gcc -c quadtree.c -Wall


clean:
	rm ./galsim ./quadtree.o ./main.o