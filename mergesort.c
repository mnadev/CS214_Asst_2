#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "simpleCSVsorter.h"


void swap(movieInfo* A, movieInfo* B){
  movieInfo* temp = A;
  *A = *B;
  *B = *temp;
}

float intComparison(float intA, float intB) {
  return intA - intB;
}

void mergesort(movieInfo** arr, int left, int right, int isInt) {
  int i = 0, j = 0;
  int n = right - left;
  for(i = 0; i <= right; i++) {
    for(j = 0; j < right - i; j++) {
      movieInfo* A = arr[i];
      movieInfo* B = arr[i + 1];

      float comparison = 0;
      if(isInt == 0) {
        comparison = (float)strcmp((char*)A->toBeSorted,(char*)B->toBeSorted);
      } else {
        comparison = intComparison((float)atof(A->toBeSorted), (float)atof(B->toBeSorted));
      }

      if(comparison > 0) {
        swap(A, B);
      }

    }
  }
}

/*
void merge(movieInfo** arr, int left, int half, int right, int isInt);

void mergesort(movieInfo** arr, int left, int right, int isInt) {
  if(left >= right) {
    return;
  }


  //find mid point
  int half = (left + right)/2;

  //sort both halves
  mergesort(arr, left, half, isInt);
  mergesort(arr, half + 1, right, isInt);

  //
  merge(arr, left, half, right, isInt);
}
*/
/*
void merge(movieInfo** arr, int left, int half, int right, int isInt) {
    //if right and left point to same thing or if somehow right is to the left of left
    if(right - left < 1) {
      return;
    }

    // else check if right and left are right next to each other
    // and swap their values if they ar eout of order and then return;
    if((right - left) == 1) {
      movieInfo* A = arr[left];
      movieInfo* B = arr[right];

      float comparison = 0;
      if(isInt == 0) {
        comparison = (float)strcmp((char*)A->toBeSorted,(char*)B->toBeSorted);
      } else {
        comparison = intComparison((float)atof(A->toBeSorted), (float)atof(B->toBeSorted));
      }

      if(comparison > 0) {
        swap(A, B);
      }

      return;
    }

    //otherwise, get size of left half and right half and also store size of movieInfo
    int sizeA = half - left + 1;
    int sizeB = right - half;
    int sizeStruct = sizeof(movieInfo);

    // create two temp arrays
    // in tempA put left half of arrays
    // and in temp B put right half
    // and then we will use this as pointers
    movieInfo** tempA = (movieInfo**) malloc(sizeA*sizeStruct);
    movieInfo** tempB = (movieInfo**) malloc(sizeB*sizeStruct);

    // create counter variables and then iterate and copy
    // data from arr into temp arrays
    int i = 0, j = 0;

    while(i < sizeA) {
      tempA[i] = arr[left + i];

      i++;
    }

    while(j < sizeB){
      tempB[j] = arr[half + 1 + j];

      j++;
    }

    // now interate through temp arrays and merge
    // to merge we will have a pointer to left half and right half.
    // and a pointer to the OG array. then we will see which is less
    // the data in right pointer or left pointer, and then set the OG array
    // pointer to the lesser.
    i = 0, j = 0;

    int arrInd = left;
    while(i < sizeA && j < sizeB){
      movieInfo* A = tempA[i];
      movieInfo* B = tempB[j];

      float comparison = 0;
      if(isInt == 0) {
        comparison = (float)strcmp(A->toBeSorted, B->toBeSorted);
      } else {
        comparison = intComparison((float) atof(A->toBeSorted),(float) atof(B->toBeSorted));
      }

      if(comparison > 0) {
        arr[arrInd] = B;
        j++;
      } else {
        arr[arrInd] = A;
        i++;
      }

      arrInd++;
    }

    // check again to make sure we iterated through all of the temp arrays
    while(i < sizeA){
      movieInfo* A = tempA[i];
      arr[arrInd] = A;
      i++;
      arrInd++;
    }

    while(i < sizeA){
      movieInfo* B = tempB[j];
      arr[arrInd] = B;
      j++;
      arrInd++;
    }

    //free malloced arrays
    free(tempA);
    free(tempB);
}
*/
