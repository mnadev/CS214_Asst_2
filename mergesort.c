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
	printf("%s\n",arr[0]->toBeSorted);
        printf("%s\n",arr[1]->toBeSorted);
	printf("%s\n",arr[2]->toBeSorted);
        printf("%s\n",arr[3]->toBeSorted);
}

//int comparator, will return negative value if 
//intA < intB. will return positive if
//intB < intA, zero if equal
float intComparator(float intA, float intB) {
	return intA - intB;
}

void merge(movieInfo** arr, int left, int half, int right, int isInt){

	 printf("%d\n",12312983791827);
        //index pointers
        int i = left, j = half + 1;
	if(right - left <= 1) {
		
		return;
	}
        

	movieInfo** tempArrA = malloc((half - left + 1) * sizeof(movieInfo));
	movieInfo** tempArrB = malloc((right - half) * sizeof(movieInfo));	

	//copy data into new temp array
	// the temlp array will just allow us to not use in place
	// otherwise we'd have be in trouble
	for(i = left; i <= half; i++) {
		tempArrA[i - left] = arr[i];
	}
	
	for(i = half + 1; i <= right; i++) {
		tempArrB[i - half - 1] = arr[i];
	}

	i = 0;
	j = 0;
	// int to store size of struct
        int sizeStruct = sizeof(movieInfo);
        int arrInd = left;
	
	// iterate through array
        while(i < (half - left + 1)  && j < (right - half)) {
		//int to hold result of comparison
		int comparison = 0;
		
		movieInfo* A = tempArrA[i];
		movieInfo* B = tempArrB[j];
		printf("isInt: %d\n", isInt);	
		//if data is not an int, we will use string compare
		if(isInt == 0) {
			comparison = strcmp(A->toBeSorted, B->toBeSorted);
		} else {
			//otherwise use int comparator functino that was created.
			comparison = intComparator(atof(A->toBeSorted), atof(B->toBeSorted));
		}
		printf("%d\n",atof(A->toBeSorted));
 		printf("%d\n",atof(A->toBeSorted));
		//i++;
		//j++;
		printf("%d\n",comparison);	
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

       	   	}           
	}
	
	while(i <  (half - left + 1)) {
              movieInfo* A = tempArrA[i];
	     arr[arrInd] = A;
             i++;
             arrInd++;
        }

	while(j <= (right - half)) {
		 movieInfo* B = tempArrB[j];
             arr[arrInd] = B;
             j++;
             arrInd++;
        }
	
	free(tempArrA);
	free(tempArrB);
}                                                                                                                                                      


