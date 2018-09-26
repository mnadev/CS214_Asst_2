all: simpleCSVsorter.c mergesort.o
	gcc simpleCSVsorter.c mergesort.o

mergesort.o: mergesort.o
	gcc -c mergesort.c

clean:
	rm mergesort.o
