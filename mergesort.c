//Add some includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVsorter.h"

movieInfo** mergesort(movieInfo** arr, int size, int isInt) {
	//base case of merge sort
        if(size == 1 || size == 0) {
		return arr;
        }

  	//split array into two and mergesort
      	// copy items into both temp arrays
        movieInfo** firstHalf = (movieInfo**) malloc((size/2)*sizeof(movieInfo));
	movieInfo** secondHalf = (movieInfo**) malloc((size - size/2)*sizeof(movieInfo));
	
	int half = size/2;
	
	//copy items into second half
	for(int i = 0; i < half; i++) {
		**(firstHalf) = (arr + i*sizeof(movieInfo));
		firstHalf++;
	}
	
	for(int i = half; i < size) {
		**(secondHalf) = (arr + i*sizeof(movieInfo));
		secondHalf++;
	} 
	
	//sort both arrays
	mergesort(movieInfo** firstHalf, half);
        mergesort(movieInfo** secondHalf,size-half);
	
	
	
	// merge the sorted arrays
	arr = merge(firstHalf, secondHalf, half, size-half, isInt);
	free(firstHalf);
	free(secondHalf);
	return arr;
}

//int comparator, will return negative value if 
//intA < intB. will return positive if
//intB < intA, zero if equal
int intComparator(float* intA, float* intB) {
	return *intA - *intB;
}

movieInfo** merge(movieInfo** A, movieInfo** B, int sizeA, int sizeB, int isInt){
        //index pointers
        int i = 0, j = 0;

        // allocate space for the return array
        movieInfo** retArr = movieInfo**[sizeA + sizeB];
        
	// int to store size of struct
        int sizeStruct = sizeof(movieInfo);
        
	// iterate through array
        while(i < sizeA && j < sizeB) {
		//int to hold result of comparison
		int comparison = 0;
		
		//if data is not an int, we will use string compare
		if(isInt == 0) {
		
			comparison = strcmp((**(A + i*sizeStruct)).toBeSorted, (**(B + j*sizeStruct)).toBeSorted)
		} else {
			//otherwise use int comparator functino that was created.
			comparison = intComparator(atoi((**(A + i*sizeStruct)).toBeSorted), atoi((**(B + j*sizeStruct)).toBeSorted));
		}

       		if(comparison < 0) {
        		// if comparison is less than 0, then B is less than A, and should be added first       
			**retArr = **(B + j*sizeStruct);
                        j++;
       			retArr++;
       		} else if(comparison > 0) {
			//if comparison is greater than 0, then B is more than A, and A should be added first
       			**retArr = **(A + i*sizeStruct);
        		i++;
        		retArr++;
                } else {
			//otherwise add both and iterate first.
       			**retArr = **(A + i*sizeStruct);
       			i++;
        		retArr++;
 			**retArr = **(B + j*sizeStruct);
			j++;
        		retArr++;
       	   	}           
	}
	return retArr;
}                                                                                                                                                      


