//Add some includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVsorter.h"

movieInfo** mergesort(movieInfo** arr, int size) {
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
	arr = merge(firstHalf, secondHalf, half, size-half);
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
       
	// this integer acts as a boolean
	// is 0 if the data is not an int, now is 1 if the data is an int
	int isInt = 1;
	
	//create temp pointer to iterate through all of array and check if each item is a number
	//because it is possible for a movie to be a number, e.g. '300', and that would be a problem
	//so we should check all of them to be sure
	movieInfo** tempPtrCheckInt = A;
	while(i < sizeA) {
		//iterate through each char, checking if it is int
		// NOTENOTENOTE: if you can find a better way, lmk cause this is very inefficient	
		char* currData = (**(tempPtrCheckInt)).toBeSorted;	
		
		while(currData != '\0') {
			// current char could be int or decimal point
			if(isdigit(currData) != 0 ||  *currData == '.' ) {
				isInt = 0;
				break;			
			}
		}
		
		//break if we found alphabetic char, no need to check anymore
		if(isInt == 0) {
			break;
		}

		tempPtrCheckInt++;
		i++;
	}

	i = 0;
	movieInfo** tempPtrCheckInt = B;

	if(isInt == 1) {
		while(i < sizeB) {
                	//iterate through each char, checking if it is int
                	// NOTENOTENOTE: if you can find a better way, lmk cause this is very inefficient       
                        char* currData = (**(tempPtrCheckInt)).toBeSorted;
                
                        while(currData != '\0') {
                
                        	if(isdigit(currData) != 0 ||  *currData == '.' ) {
                                	isInt = 0;
                                        break;
                                }
                        }
                                                                                                                                                                                                             //break if we found alphabetic char, no need to check anymore
                                                                                                                                                                                                             if(isInt == 0) {
                 		break;                                                                                                                                                                       }
                
                        tempPtrCheckInt++;
                        i++;
                                                                                                                                                                                     		     }
                      
	}
	
	i = 0;

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


