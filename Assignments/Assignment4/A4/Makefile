main: quadtree.o main.o
	gcc -o main quadtree.o main.o

main.o: main.c quadtree.o
	gcc -c main.c

quadtree.o: quadtree.c
	gcc -c quadtree.c


clean:
	rm ./main ./quadtree.o ./main.o