OFLAGS = -O3 -lm
main: quadtree.o main.o
	gcc -o galsim quadtree.o main.o -Wall $(OFLAGS)

main.o: main.c quadtree.o
	gcc -c main.c -Wall $(OFLAGS)

quadtree.o: quadtree.c
	gcc -c quadtree.c -Wall $(OFLAGS)


clean:
	rm ./galsim ./quadtree.o ./main.o
