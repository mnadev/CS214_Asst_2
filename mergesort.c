//Add some includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVsorter.h"

void merge(movieInfo* A, int left, int half, int right, int isInt);

void mergesort(movieInfo* arr, int leftInd, int rightInd, int isInt) {
	//base case of merge sort
        if(leftInd > rightInd) {
		return;
        }

	//get mid
	int half = (leftInd+rightInd)/2;
	
	//sort both arrays
	mergesort(movieInfo* arr, left, half, isInt);
        mergesort(movieInfo* arr,half + 1, right, isInt);
	
	// merge the sorted arrays
	merge(arr, left, half, right, isInt);
}

void swap(movieInfo* A, movieInfo* B) {
	movieInfo* temp = *A;
	*A = *B;
	*B = *A;
}

//int comparator, will return negative value if 
//intA < intB. will return positive if
//intB < intA, zero if equal
int intComparator(float intA, float intB) {
	return intA - intB;
}

void merge(movieInfo* arr, int left, int half, int right, int isInt){
        //index pointers
        int i = left, j = half + 1;

        // allocate space for the return array
        //movieInfo** retArr[sizeA + sizeB];
        
	movieInfo* A = arr[i];
	movieInfo* B = arr[j];
	
	// int to store size of struct
        int sizeStruct = sizeof(movieInfo);
        
	// iterate through array
        while(i < right && j < right) {
		//int to hold result of comparison
		int comparison = 0;
		
		//if data is not an int, we will use string compare
		if(isInt == 0) {
			comparison = strcmp(A->toBeSorted, B->toBeSorted);
		} else {
			//otherwise use int comparator functino that was created.
			comparison = intComparator(atof(A->toBeSorted), atof(B->toBeSorted));
		}

       		if(comparison < 0 && i > j) {
        		// if comparison is less than 0, then B is more than A, and A should be added first       
			//TODO: switch places of items
			swap(*A, *B);
                        j++;
			B = arr[j];
			A = arr[i];
       		} else if(comparison > 0 && i < j) {
			//if comparison is greater than 0, then B is less than A, and B should be added first
       			swap(*A, *B);
        		i++;
			B = arr[j];
			A = arr[i];
                } else {
			//otherwise add both and iterate first.
       	   	}           
	}
	
	return retArr;
}                                                                                                                                                      


