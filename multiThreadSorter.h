/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

//Suggestion: define a struct that mirrors a record (row) of the data set
#include<limits.h>
//Defining standard file descriptors for code readability
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef
struct _movieInfo{
		char* color;
		char* director_name;
		float num_critic_for_reviews = (float) INT_MIN;
		float duration = (float) INT_MIN;
		float director_facebook_likes = (float) INT_MIN;
		float actor_3_facebook_likes = (float) INT_MIN;
		char* actor_2_name;
		float actor_1_facebook_likes = (float) INT_MIN;
		float gross = (float) INT_MIN;
		char* genres;
		char* actor_1_name;
		char* movie_title;
		float num_voted_users = (float) INT_MIN;
		float cast_total_facebook_likes = (float) INT_MIN;
		char* actor_3_name;
		float facenumber_in_poster = (float) INT_MIN;
		char* plot_keywords;
		char* movie_imdb_link;
		float num_user_for_reviews = (float) INT_MIN;
		char* language;
		char* country;
		char* content_rating;
		float budget = (float) INT_MIN;
		float title_year = (float) INT_MIN;
		float actor_2_facebook_likes = (float) INT_MIN;
		float imdb_score = (float) INT_MIN;
		float aspect_ratio = (float) INT_MIN;
		float movie_facebook_likes = (float) INT_MIN;
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
