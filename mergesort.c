#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scannerCSVsorter.h"


void swap(movieInfo* A, movieInfo* B){
    movieInfo temp = *A;
    *A = *B;
    *B = temp;
}

float intComparison(float intA, float intB) {
    return intA - intB;
}
void merge(movieInfo** arr, char * columnToBeSorted, int left, int half, int right, int isInt);

void mergesort(movieInfo** arr, char * columnToBeSorted, int left, int right) {
	if(left >= right) {
	    return;
	}
    
    	int isInt = 1;
	if(strcmp(columnToBeSorted, "color") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "director_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
	} else if(strcmp(columnToBeSorted, "duration") == 0){
	} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){
	} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){
	} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
	} else if(strcmp(columnToBeSorted, "gross") == 0){
	} else if(strcmp(columnToBeSorted, "genres") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
	isInt = 0;
	} else if(strcmp(columnToBeSorted, "movie_title") == 0){
	isInt = 0;
	} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
	} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){
	} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){
	} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){
	} else if(strcmp(columnToBeSorted, "language") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "country") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "content_rating") == 0){
		isInt = 0;
	} else if(strcmp(columnToBeSorted, "budget") == 0){
	} else if(strcmp(columnToBeSorted, "title_year") == 0){
	} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){
	} else if(strcmp(columnToBeSorted, "imdb_score") == 0){
	} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){
	} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){
	} else{
		if(columnToBeSorted == '\0'){
		    return -1;
		}
		return -1;
	}
    
	//find mid point
	int half = (left + right)/2;
    
	//sort both halves
	mergesort(arr, left, half, isInt);
	mergesort(arr, half + 1, right, isInt);

	//
	merge(arr, columnToBeSorted, left, half, right, isInt);
}

void merge(movieInfo** arr, char* columnToBeSorted,int left, int half, int right, int isInt) {
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
           		 char* trimmedA;    //trimmed whitespace version of A
           		 char* trimmedB;    //trimmed whitespace version of B
                	if(strcmp(columnToBeSorted, "color") == 0){
                    		trimmedA = A->color;
                    		trimmedB = B->color;
                   	} else if(strcmp(columnToBeSorted, "director_name") == 0){
                   		trimmedA = A->director_name;
                    		trimmedB = B->director_name;
                    	} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
                    	} else if(strcmp(columnToBeSorted, "duration") == 0){
                    	} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){                    
                    	} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){                    
                    	} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
			    trimmedA = A->actor_2_name;
			    trimmedB = B->actor_2_name;
                    	} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
                    
                    	} else if(strcmp(columnToBeSorted, "gross") == 0){
                    
                    	} else if(strcmp(columnToBeSorted, "genres") == 0){
                   		trimmedA = A->genres;
                    		trimmedB = B->genres;
                    	} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
                    		trimmedA = A->actor_1_name;
                    		trimmedB = B->actor_1_name;
                    	} else if(strcmp(columnToBeSorted, "movie_title") == 0){
                    		trimmedA = A->movie_title;
                    		trimmedB = B->movie_title;
                    	} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
                    	} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){                   
                  	} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
                    		trimmedA = A->actor_3_name;
                    		trimmedB = B->actor_3_name;
                    	} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){                  
                    	} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
                    		trimmedA = A->plot_keywords;
                    		trimmedB = B->plot_keywords;
                    	} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
                    		trimmedA = A->movie_imdb_link;
                    		trimmedB = B->movie_imdb_link;
                    	} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){                  
                   	} else if(strcmp(columnToBeSorted, "language") == 0){
                    		trimmedA = A->language;
                    		trimmedB = B->language;
                    	} else if(strcmp(columnToBeSorted, "country") == 0){
                    		trimmedA = A->country;
                    		trimmedB = B->country;
                    	} else if(strcmp(columnToBeSorted, "content_rating") == 0){
                    		trimmedA = A->content_rating;
                    		trimmedB = B->content_rating;
                    	} else if(strcmp(columnToBeSorted, "budget") == 0){                   
                    	} else if(strcmp(columnToBeSorted, "title_year") == 0){                   
                    	} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){                   
                    	} else if(strcmp(columnToBeSorted, "imdb_score") == 0){                    
                    	} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){                    
                    	} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){                    
                    	} else{
                    		if(columnToBeSorted == '\0'){
                       			return -1;
                    		}
                    		return -1;
		    	}
			
  			int trimALen = strlen(trimmedA);
                	int trimBLen =strlen(trimmedB);
			
			/*while(trimmedA[0] == ' '){
			    trimmedA++;
			    trimALen--;
			}
			while(trimmedB[0] == ' '){
			    trimmedB++;
			    trimBLen--;
			}*/
			
			char* postTrimA = trimmedA;     //In case trimmedA has whitespace at end
			int needToFreeA = 0;            //If we do malloc, we need to free. This is to indicate if we need to or not.
			
			if(trimmedA[trimALen-1] == ' '){
			    
				postTrimA = malloc(trimALen);
			    	needToFreeA = 1;
			    	memcpy(postTrimA, trimmedA, trimALen);
			   	while(postTrimA[trimALen-1] == ' '){
					postTrimA[trimALen-1] = '\0';
					trimALen--;
			    	}
			}
			
			char* postTrimB = trimmedB;    //in case trimmedB has whitespace at the end
			int needToFreeB = 0;
			if(trimmedB[trimBLen-1] == ' '){
				postTrimB = malloc(trimBLen);
				needToFreeB = 1;
				memcpy(postTrimB, trimmedB, trimBLen);
				while(postTrimB[trimBLen-1] == ' '){
					postTrimB[trimBLen-1] = '\0';
					trimBLen--;
				}
			}
			comparison = (float)strcmp(postTrimA, postTrimB);
			if(needToFreeA == 1){    free(postTrimA); }
			if(needToFreeB == 1){    free(postTrimB); }
		} else {
			float intA;
			float intB;
			if(strcmp(columnToBeSorted, "color") == 0){
                   	} else if(strcmp(columnToBeSorted, "director_name") == 0){
                    	} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
                    		intA = A->num_critic_for_reviews;
                    		intB = B->num_critic_for_reviews;
                    	} else if(strcmp(columnToBeSorted, "duration") == 0){
                    		intA = A->duration;
                    		intB = B->duration;
                    	} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){
			    intA = A->director_facebook_likes;
			    intB = B->director_facebook_likes;
                    	} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){
			    intA = A->actor_3_facebook_likes;
			    intB = B->actor_3_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
			} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
				intA = A->actor_1_facebook_likes;
				intB = B->actor_1_facebook_likes;
			} else if(strcmp(columnToBeSorted, "gross") == 0){
				intA = A->gross;
				intB = B->gross;
			} else if(strcmp(columnToBeSorted, "genres") == 0){
			} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
			} else if(strcmp(columnToBeSorted, "movie_title") == 0){
			} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
				intA = A->num_voted_users;
				intB = B->num_voted_users;
			} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){
				intA = A->cast_total_facebook_likes;
				intB = B->cast_total_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
			} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){
				intA = A->facenumber_in_poster;
				intB = B->facenumber_in_poster;
			} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
			} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
			} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){
				intA = A->num_user_for_reviews;
				intB = B->num_user_for_reviews;
			} else if(strcmp(columnToBeSorted, "language") == 0){
			} else if(strcmp(columnToBeSorted, "country") == 0){
			} else if(strcmp(columnToBeSorted, "content_rating") == 0){
			} else if(strcmp(columnToBeSorted, "budget") == 0){
				intA = A->budget;
				intB = B->budget;
			} else if(strcmp(columnToBeSorted, "title_year") == 0){
				intA = A->title_year;
				intB = B->title_year;
			} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){
				intA = A->actor_2_facebook_likes;
				intB = B->actor_2_facebook_likes;
			} else if(strcmp(columnToBeSorted, "imdb_score") == 0){
				intA = A->imdb_score;
				intB = B->imdb_score;
			} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){
				intA = A->aspect_ratio;
				intB = B->aspect_ratio;
			} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){
				intA = A->movie_facebook_likes;
				intB = B->movie_facebook_likes;
			} else{
				if(columnToBeSorted == '\0'){
					return -1;
				}
				return -1;
			}
			//printf("%f\n", intA);
			//printf("%f\n", intB);
			//printf("%f\n", intComparison((float)atof(A->toBeSorted), (float)atof(B->toBeSorted)));
			comparison = intComparison(intA,intB);
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
			char* trimmedA;    //trimmed whitespace version of A
			char* trimmedB;    //trimmed whitespace version of B

			if(strcmp(columnToBeSorted, "color") == 0){
				trimmedA = A->color;
				trimmedB = B->color;
			} else if(strcmp(columnToBeSorted, "director_name") == 0){
				trimmedA = A->director_name;
				trimmedB = B->director_name;
			} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
			} else if(strcmp(columnToBeSorted, "duration") == 0){
			} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
				trimmedA = A->actor_2_name;
				trimmedB = B->actor_2_name;
			} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "gross") == 0){
			} else if(strcmp(columnToBeSorted, "genres") == 0){
				trimmedA = A->genres;
				trimmedB = B->genres;
			} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
				trimmedA = A->actor_1_name;
				trimmedB = B->actor_1_name;
			} else if(strcmp(columnToBeSorted, "movie_title") == 0){
				trimmedA = A->movie_title;
				trimmedB = B->movie_title;
			} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
			} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
				trimmedA = A->actor_3_name;
				trimmedB = B->actor_3_name;
			} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){
			} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
				trimmedA = A->plot_keywords;
				trimmedB = B->plot_keywords;
			} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
				trimmedA = A->movie_imdb_link;
				trimmedB = B->movie_imdb_link;
			} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){
			} else if(strcmp(columnToBeSorted, "language") == 0){
				trimmedA = A->language;
				trimmedB = B->language;
			} else if(strcmp(columnToBeSorted, "country") == 0){
				trimmedA = A->country;
				trimmedB = B->country;
			} else if(strcmp(columnToBeSorted, "content_rating") == 0){
				trimmedA = A->content_rating;
				trimmedB = B->content_rating;
			} else if(strcmp(columnToBeSorted, "budget") == 0){
			} else if(strcmp(columnToBeSorted, "title_year") == 0){
			} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "imdb_score") == 0){
			} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){
			} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){
			} else{
				if(columnToBeSorted == '\0'){
					return -1;
				}
				return -1;
			}
                
			int trimALen = strlen(trimmedA);
			int trimBLen = strlen(trimmedA);

			/*while(trimmedA[0] == ' '){
			    trimmedA++;
			}
			while(trimmedB[0] == ' '){
			    trimmedB++;
			}*/

			char* postTrimA = trimmedA;     //In case trimmedA has whitespace at end
			int needToFreeA = 0;            //If we do malloc, we need to free. This is to indicate if we need to or not.
			if(trimmedA[trimALen-1] == ' '){
			    postTrimA = malloc(trimALen);
			    needToFreeA = 1;
			    memcpy(postTrimA, trimmedA, trimALen);
			    while(postTrimA[trimALen-1] == ' '){
				postTrimA[trimALen-1] = '\0';
				trimALen--;
			    }
			}

			char* postTrimB = trimmedB;    //in case trimmedB has whitespace at the end
			int needToFreeB = 0;
			if(trimmedB[trimBLen-1] == ' '){
			    postTrimB = malloc(trimBLen);
			    needToFreeB = 1;
			    memcpy(postTrimB, trimmedB, trimBLen);
			    while(postTrimB[trimBLen-1] == ' '){
				postTrimB[trimBLen-1] = '\0';
				trimBLen--;
			    }
			}

			comparison = (float)strcmp(postTrimA, postTrimB);
			if(needToFreeA == 1){    free(postTrimA); }
			if(needToFreeB == 1){    free(postTrimB); }
		} else {
			float intA;
			float intB;
			
                	if(strcmp(columnToBeSorted, "color") == 0){
			} else if(strcmp(columnToBeSorted, "director_name") == 0){
			} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
				intA = A->num_critic_for_reviews;
				intB = B->num_critic_for_reviews;
			} else if(strcmp(columnToBeSorted, "duration") == 0){
				intA = A->duration;
				intB = B->duration;
			} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){
				intA = A->director_facebook_likes;
				intB = B->director_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){
				intA = A->actor_3_facebook_likes;
				intB = B->actor_3_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
			} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
				intA = A->actor_1_facebook_likes;
				intB = B->actor_1_facebook_likes;
			} else if(strcmp(columnToBeSorted, "gross") == 0){
				intA = A->gross;
				intB = B->gross;
			} else if(strcmp(columnToBeSorted, "genres") == 0){
			} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
			} else if(strcmp(columnToBeSorted, "movie_title") == 0){
			} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
				intA = A->num_voted_users;
				intB = B->num_voted_users;
			} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){
				intA = A->cast_total_facebook_likes;
				intB = B->cast_total_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
			} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){
				intA = A->facenumber_in_poster;
				intB = B->facenumber_in_poster;
			} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
			} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
			} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){
				intA = A->num_user_for_reviews;
				intB = B->num_user_for_reviews;
			} else if(strcmp(columnToBeSorted, "language") == 0){
			} else if(strcmp(columnToBeSorted, "country") == 0){
			} else if(strcmp(columnToBeSorted, "content_rating") == 0){
			} else if(strcmp(columnToBeSorted, "budget") == 0){
				intA = A->budget;
				intB = B->budget;
			} else if(strcmp(columnToBeSorted, "title_year") == 0){
				intA = A->title_year;
				intB = B->title_year;
			} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){
				intA = A->actor_2_facebook_likes;
				intB = B->actor_2_facebook_likes;
			} else if(strcmp(columnToBeSorted, "imdb_score") == 0){
				intA = A->imdb_score;
				intB = B->imdb_score;
			} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){
				intA = A->aspect_ratio;
				intB = B->aspect_ratio;
			} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){
				intA = A->movie_facebook_likes;
				intB = B->movie_facebook_likes;
			} else{
				if(columnToBeSorted == '\0'){
				r	eturn -1;
				}
				return -1;
			}
                	comparison = intComparison(intA,intB);
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



movieInfo** mergeNodeData(movieInfo** arrA, movieInfo** arrB, int arrLenA, int arrLenB, char * columnToBeSorted) {
	movieInfo** mergedArr = malloc(sizeof(movieInfo) * (arrLenA + arrLenB));
	
 	int ptrA = 0, ptrB = 0, ptrMerged = 0;	
	
	while(ptrA < (arrLenA - 1) && ptrB < (arrLenB - 1)){
		movieInfo* A = arrA[ptrA];
		movieInfo* B = arrB[ptrB];
            
		float comparison = 0;
		 
		if(isInt == 0) {
		    
			char* trimmedA;    //trimmed whitespace version of A
			char* trimmedB;    //trimmed whitespace version of B

                    	if(strcmp(columnToBeSorted, "color") == 0){
				trimmedA = A->color;
				trimmedB = B->color;
			} else if(strcmp(columnToBeSorted, "director_name") == 0){
				trimmedA = A->director_name;
				trimmedB = B->director_name;
			} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
			} else if(strcmp(columnToBeSorted, "duration") == 0){
			} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
				trimmedA = A->actor_2_name;
				trimmedB = B->actor_2_name;
			} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "gross") == 0){
			} else if(strcmp(columnToBeSorted, "genres") == 0){
				trimmedA = A->genres;
				trimmedB = B->genres;
			} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
				trimmedA = A->actor_1_name;
				trimmedB = B->actor_1_name;
			} else if(strcmp(columnToBeSorted, "movie_title") == 0){
				trimmedA = A->movie_title;
				trimmedB = B->movie_title;
			} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
			} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
				trimmedA = A->actor_3_name;
				trimmedB = B->actor_3_name;
			} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){
			} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
				trimmedA = A->plot_keywords;
				trimmedB = B->plot_keywords;
			} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
				trimmedA = A->movie_imdb_link;
				trimmedB = B->movie_imdb_link;
			} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){
			} else if(strcmp(columnToBeSorted, "language") == 0){
				trimmedA = A->language;
				trimmedB = B->language;
			} else if(strcmp(columnToBeSorted, "country") == 0){
				trimmedA = A->country;
				trimmedB = B->country;
			} else if(strcmp(columnToBeSorted, "content_rating") == 0){
				trimmedA = A->content_rating;
				trimmedB = B->content_rating;
			} else if(strcmp(columnToBeSorted, "budget") == 0){
			} else if(strcmp(columnToBeSorted, "title_year") == 0){
			} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){
			} else if(strcmp(columnToBeSorted, "imdb_score") == 0){
			} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){
			} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){
			} else{
				if(columnToBeSorted == '\0'){
					//return -1;
				}
				//return -1;
			}
                
			int trimALen = strlen(trimmedA);
			int trimBLen = strlen(trimmedA);
			
			/*while(trimmedA[0] == ' '){
			    trimmedA++;
			}
			while(trimmedB[0] == ' '){
			    trimmedB++;
			}*/
			
			char* postTrimA = trimmedA;     //In case trimmedA has whitespace at end
			int needToFreeA = 0;            //If we do malloc, we need to free. This is to indicate if we need to or not.

			if(trimmedA[trimALen-1] == ' '){
			    postTrimA = malloc(trimALen);
			    needToFreeA = 1;
			    memcpy(postTrimA, trimmedA, trimALen);
			    while(postTrimA[trimALen-1] == ' '){
				postTrimA[trimALen-1] = '\0';
				trimALen--;
			    }
			}
		    
			char* postTrimB = trimmedB;    //in case trimmedB has whitespace at the end
			int needToFreeB = 0;

			if(trimmedB[trimBLen-1] == ' '){
			    postTrimB = malloc(trimBLen);
			    needToFreeB = 1;
			    memcpy(postTrimB, trimmedB, trimBLen);
			    while(postTrimB[trimBLen-1] == ' '){
				postTrimB[trimBLen-1] = '\0';
				trimBLen--;
			    }
			}

			comparison = (float)strcmp(postTrimA, postTrimB);
			if(needToFreeA == 1){    free(postTrimA); }
			if(needToFreeB == 1){    free(postTrimB); }
                } else {
			float intA;
			float intB;
                	if(strcmp(columnToBeSorted, "color") == 0){
			} else if(strcmp(columnToBeSorted, "director_name") == 0){
			} else if(strcmp(columnToBeSorted, "num_critic_for_reviews") == 0){
				intA = A->num_critic_for_reviews;
				intB = B->num_critic_for_reviews;
			} else if(strcmp(columnToBeSorted, "duration") == 0){
				intA = A->duration;
				intB = B->duration;
			} else if(strcmp(columnToBeSorted, "director_facebook_likes") == 0){
				intA = A->director_facebook_likes;
				intB = B->director_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_3_facebook_likes") == 0){
				intA = A->actor_3_facebook_likes;
				intB = B->actor_3_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_2_name") == 0){
			} else if(strcmp(columnToBeSorted, "actor_1_facebook_likes") == 0){
				intA = A->actor_1_facebook_likes;
				intB = B->actor_1_facebook_likes;
			} else if(strcmp(columnToBeSorted, "gross") == 0){
				intA = A->gross;
				intB = B->gross;
			} else if(strcmp(columnToBeSorted, "genres") == 0){
			} else if(strcmp(columnToBeSorted, "actor_1_name") == 0){
			} else if(strcmp(columnToBeSorted, "movie_title") == 0){
			} else if(strcmp(columnToBeSorted, "num_voted_users") == 0){
				intA = A->num_voted_users;
				intB = B->num_voted_users;
			} else if(strcmp(columnToBeSorted, "cast_total_facebook_likes") == 0){
				intA = A->cast_total_facebook_likes;
				intB = B->cast_total_facebook_likes;
			} else if(strcmp(columnToBeSorted, "actor_3_name") == 0){
			} else if(strcmp(columnToBeSorted, "facenumber_in_poster") == 0){
				intA = A->facenumber_in_poster;
				intB = B->facenumber_in_poster;
			} else if(strcmp(columnToBeSorted, "plot_keywords") == 0){
			} else if(strcmp(columnToBeSorted, "movie_imdb_link") == 0){
			} else if(strcmp(columnToBeSorted, "num_user_for_reviews") == 0){
				intA = A->num_user_for_reviews;
				intB = B->num_user_for_reviews;
			} else if(strcmp(columnToBeSorted, "language") == 0){
			} else if(strcmp(columnToBeSorted, "country") == 0){
			} else if(strcmp(columnToBeSorted, "content_rating") == 0){
			} else if(strcmp(columnToBeSorted, "budget") == 0){
				intA = A->budget;
				intB = B->budget;
			} else if(strcmp(columnToBeSorted, "title_year") == 0){
				intA = A->title_year;
				intB = B->title_year;
			} else if(strcmp(columnToBeSorted, "actor_2_facebook_likes") == 0){
				intA = A->actor_2_facebook_likes;
				intB = B->actor_2_facebook_likes;
			} else if(strcmp(columnToBeSorted, "imdb_score") == 0){
				intA = A->imdb_score;
				intB = B->imdb_score;
			} else if(strcmp(columnToBeSorted, "aspect_ratio") == 0){
				intA = A->aspect_ratio;
				intB = B->aspect_ratio;
			} else if(strcmp(columnToBeSorted, "movie_facebook_likes") == 0){
				intA = A->movie_facebook_likes;
				intB = B->movie_facebook_likes;
			} else{
				if(columnToBeSorted == '\0'){
					//return -1;
				}
				//return -1;
			}
                	comparison = intComparison(intA,intB);
            	}
            
		if(comparison > 0) {
			mergedArr[ptrMerged] = B;
			ptrB++;
		} else {
			mergedArr[ptrMerged] = A;
			ptrA++;
		}
            
            	ptrMerged++;
	}
        
        // check again to make sure we iterated through all of the temp arrays
        while(ptrA < (arrLenA - 1)){
            movieInfo* A = arrA[i];
            mergedArr[ptrMerged] = A;
            ptrA++;
            ptrMerged++;
        }
        
        while(ptrB < (arrLenB - 1)){
            movieInfo* B = tempB[j];
            mergedArr[ptrMerged] = B;
            ptrA++;
            ptrMerged++;
        }
	
	//free(arrA);
	//free(arrB);
	return mergedArr;
}
