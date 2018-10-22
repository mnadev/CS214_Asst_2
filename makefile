all: scannerCSVsorter.c mergesort.o
	gcc scannerCSVsorter.c mergesort.o -o scannerCSVSorter

mergesort.o: mergesort.o
	gcc -c mergesort.c

clean:
	rm mergesort.o
