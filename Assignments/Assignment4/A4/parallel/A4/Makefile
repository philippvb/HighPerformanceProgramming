OFLAGS = 
Subdir = ../../
main: quadtree.o main.o
	gcc -o galsim quadtree.o main.o -Wall $(OFLAGS)

main.o: main.c quadtree.o
	gcc -c main.c -Wall $(OFLAGS)

quadtree.o: $(Subdir)quadtree.c
	gcc -c $(Subdir)quadtree.c -Wall $(OFLAGS)


clean:
	rm ./galsim ./quadtree.o ./main.o