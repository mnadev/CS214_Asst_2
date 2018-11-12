all: multiThreadSorter.c mergesort.o
	gcc multiThreadSorter.c mergesort.o -pthread -o multiThreadSorter

debug: multiThreadSorter.c mergesort.o
	gcc -g multiThreadSorter.c mergesort.o -o scannerDebug

mergesort.o: mergesort.o
	gcc -c mergesort.c

clean:
	rm mergesort.o
