all: scannerCSVsorter.c mergesort.o
	gcc scannerCSVsorter.c mergesort.o -o scannerCSVSorter

debug: scannerCSVsorter.c mergesort.o
	gcc -g scannerCSVsorter.c mergesort.o -o scannerDebug

mergesort.o: mergesort.o
	gcc -c mergesort.c

clean:
	rm mergesort.o
