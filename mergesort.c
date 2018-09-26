//Add some includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simpleCSVsorter.h"

movieInfo** mergesort(movieInfo** arr, int size) {
	//base case of merge sort
        if(size == 1) {
		return arr;
        }

  	//split array into two and mergesort
      	// i saved return value into array because we 
        // can then merge them easier
        movieInfo** firstHalf = mergesort(char** arr, size/2);
        movieInfo** secondHalf = mergesort(char** (arr + (size/2)*sizeof(movieInfo)),size/2);
	
	return merge(firstHalf, secondHalf, size/2, size/2);
}

movieInfo** merge(movieInfo** A, movieInfo** B, int sizeA, int sizeB){
        //index pointers
        int i = 0, j = 0;
       
        // allocate space for the return array
        movieInfo** retArr = malloc((sizeA + sizeB)*sizeof(movieInfo));
        
        int sizeStruct = sizeof(movieInfo);
        
        while(i < sizeA && j < sizeB) {
		int comparison = strncmp(*(A + i*sizeStruct).toBeSorted, *(B + j*sizeStruct).toBeSorted)
		
       		if(comparison > 0) {
        		*retArr = *(B + j*sizeStruct);
                        j++;
       			retArr++;
       		} else if(comparison < 0) {
       			*retArr = *(A + i*sizeStruct);
        		i++;
        		retArr++;
                } else {
       			*retArr = *(B + j*sizeStruct);
       			j++;
        		retArr++;
 			*retArr = *(A + i*sizeStruct);
			i++;
        		retArr++;
       	   	}           
	}

}                                                                                                                                                      

