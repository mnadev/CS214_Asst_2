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
		char* num_critic_for_reviews_Orig;
		
		float duration;
		char* duration_Orig;
		
		float director_facebook_likes;
		char* director_facebook_likes_Orig;
		
		float actor_3_facebook_likes;
		char* actor_3_facebook_likes_Orig;
		
		char* actor_2_name;
		
		float actor_1_facebook_likes;
		char* actor_1_facebook_likes_Orig;

		float gross;
		char* gross_Orig;
	
		char* genres;

		char* actor_1_name;

		char* movie_title;

		float num_voted_users;
		char* num_voted_users_Orig;

		float cast_total_facebook_likes;
		char* cast_total_facebook_likes_Orig;

		char* actor_3_name;

		float facenumber_in_poster;
		char* facenumber_in_poster_Orig;

		char* plot_keywords;

		char* movie_imdb_link;

		float num_user_for_reviews;
		char* num_user_for_reviews_Orig;

		char* language;

		char* country;

		char* content_rating;

		float budget;
		char* budget_Orig;

		float title_year;
		char* title_year_Orig;

		float actor_2_facebook_likes;
		char* actor_2_facebook_likes_Orig;

		float imdb_score;
		char* imdb_score_Orig;

		float aspect_ratio;
		char* aspect_ratio_Orig;

		float movie_facebook_likes;
		char* movie_facebook_likes_Orig;

} movieInfo;

//Struct to be used to pass arguments into the thread functions.
typedef
struct _threadArgs_DirFile{
	char* pathName;
	char* columnToSort;
	char* dirDest;
	char* prevThreadID;
} threadArgs_DirFile;

//Struct to be used to pass information back to calling thread (#threads spawned+threadID lists)
typedef
struct _threadRetvals{
	char** spawnedThreadList;
	int spawnedThreadNum;
} threadRetvals;

typedef struct _movieNode {
	movieInfo** data;
	int arrLen;
	struct _movieNode* next;
} movieNode;


//Suggestion: prototype a mergesort function
void mergesort(movieInfo** arr, char * columnToBeSorted, int left, int right, int isInt);                                                                             
movieInfo** mergeNodeData(movieInfo** arrA, movieInfo** arrB, int arrLenA, int arrLenB, char * columnToBeSorted,int isInt);
