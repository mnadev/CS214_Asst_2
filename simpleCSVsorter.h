/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

//Suggestion: define a struct that mirrors a record (row) of the data set

typedef
struct _movieInfo{
	char* color;
	char* director_name;
	char* num_critic_for_reviews;
	int duration;
	char* director_facebook_likes;
	int actor_3_facebook_likes;
	char* actor_2_name;
	int actor_1_facebook_likes;
	double gross;
	char* genres;  				
	char* actor_1_name;
	char* movie_title;
	int num_voted_users;
	int cast_total_facebook_likes;
	char* actor_3_name;
	int facenumber_in_poster;
	char* plot_keywords;			
	char* movie_imdb_link;
	int num_user_for_reviews;
	char* language;
	char* country;
	char* content_rating;
	u_int budget;
	int title_year;
	int actor_2_facebook_likes;
	double imdb_score;
	double aspect_ratio;
	int movie_facebook_likes;

} movieInfo;

//Suggestion: prototype a mergesort function
char** mergesort(char** arr, int size) {
	//base case of merge sort
        if(size == 1) {
		return arr;
        }
  	//split array into two and mergesort
      	// i saved return value into array because we 
        // can then merge them easier
        char** firstHalf = mergesort(char** arr, size/2);
        char** secondHalf = mergesort(char** (arr + (size/2) **sizeof(char)),size/2);
	return merge(firstHalf, secondHalf, sizeA, sizeB);
}

char* merge(void* A, void* B, int sizeA, int sizeB){
        //index pointers
        int i = 0, j = 0;
       
        // allocate space for the return array
        movieInfo* retArr = malloc((sizeA + sizeB)*sizeof(movieInfo));
        
        int sizeStruct = sizeof(movieInfo);
        
        while(i < sizeA && j < sizeB) {
        	if(*(A + i*sizeStruct) > *(B + j*sizeStruct)) {
        		*retArr = *(B + j*sizeStruct);
                        j++;
       			retArr++;
       		} else if(*(A + i*sizeStruct) < *(B + j*sizeStruct)) {
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
