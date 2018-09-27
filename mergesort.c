//Add some includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVsorter.h"

void merge(movieInfo** A, int left, int half, int right, int isInt);

void mergesort(movieInfo** arr, int leftInd, int rightInd, int isInt) {
	//base case of merge sort
        if(leftInd >= rightInd) {
		return;
        }

	//get mid
	int half = (leftInd+rightInd)/2;
	
	//sort both arrays
	mergesort(arr, leftInd, half, isInt);
    mergesort(arr,half + 1, rightInd, isInt);
	
	// merge the sorted arrays
	merge(arr, leftInd, half, rightInd, isInt);
}

//int comparator, will return negative value if 
//intA < intB. will return positive if
//intB < intA, zero if equal
int intComparator(float intA, float intB) {
	return intA - intB;
}

void merge(movieInfo** arr, int left, int half, int right, int isInt){
        //index pointers
        int i = left, j = half + 1;

        
	movieInfo** tempArr = malloc((right-left+1) * sizeof(movieInfo));
	
	//copy data into new temp array
	// the temp array will just allow us to not use in place
	// otherwise we'd have be in trouble
	for(i = left; i <= (right); i++) {
		tempArr[i - left] = arr[i];
	}
	
	i = left;
	j = half + 1;
	// int to store size of struct
        int sizeStruct = sizeof(movieInfo);
        int arrInd = left;
	
	// iterate through array
        while(i < half || j < right) {
		//int to hold result of comparison
		int comparison = 0;
		
		movieInfo* A = tempArr[i - left];
		movieInfo* B = tempArr[j - (half + 1)];
		
		printf("merge");
                                        printf("%s",A->beforeSortedCol);
                                        printf("%s",A->toBeSorted);
                                        printf("%s",A->afterSortedCol);
                                        printf("\n");	

		 printf("%s",B->beforeSortedCol);
                                        printf("%s",B->toBeSorted);
                                        printf("%s",B->afterSortedCol);
                                        printf("\n");	
		//if data is not an int, we will use string compare
		if(isInt == 0) {
			comparison = strcmp(A->toBeSorted, B->toBeSorted);
		} else {
			//otherwise use int comparator functino that was created.
			comparison = intComparator(atof(A->toBeSorted), atof(B->toBeSorted));
		}

       		if(comparison < 0) {
        		// if comparison is less than 0, then B is more than A, and A should be added first       
			arr[arrInd] = A;
                        i++;
			arrInd++;
       		} else if(comparison > 0) {
			//if comparison is greater than 0, then B is less than A, and B should be added first
       			arr[arrInd] = B;
        		j++;
			arrInd++;
			
                } else {
			//otherwise add both and iterate both.
			arr[arrInd] = A;
                        i++;
			arrInd++;
			arr[arrInd] = B;
        		j++;
			arrInd++;
       	   	}           
	}
	free(tempArr);
}                                                                                                                                                      


