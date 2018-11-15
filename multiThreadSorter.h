/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

//Suggestion: define a struct that mirrors a record (row) of the data set

//Defining standard file descriptors for code readability
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef
struct _movieInfo{
		char* color;
		char* director_name;
		float num_critic_for_reviews;
		float duration;
		float director_facebook_likes;
		float actor_3_facebook_likes;
		char* actor_2_name;
		float actor_1_facebook_likes;
		float gross;
		char* genres;
		char* actor_1_name;
		char* movie_title;
		float num_voted_users;
		float cast_total_facebook_likes;
		char* actor_3_name;
		float facenumber_in_poster;
		char* plot_keywords;
		char* movie_imdb_link;
		float num_user_for_reviews;
		char* language;
		char* country;
		char* content_rating;
		float budget;
		float title_year;
		float actor_2_facebook_likes;
		float imdb_score;
		float aspect_ratio;
		float movie_facebook_likes;
} movieInfo;

//Struct to be used to pass arguments into the thread functions.
typedef
struct _threadArgs{
	char* pathName;
	char* columnToSort;
	char* dirDest;
} threadArgs;

typedef struct _movieNode {
	movieInfo** data;
	int arrLen;
	movieNode* next;
} movieNode;


//Suggestion: prototype a mergesort function
void mergesort(movieInfo** arr, char * columnToBeSorted, int left, int right);                                                                                    
movieInfo** mergeNodeData(movieInfo** arrA, movieInfo** arrB, int arrLenA, int arrLenB, char * columnToBeSorted)
