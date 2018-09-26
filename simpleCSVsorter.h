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
	char* beforeSortedCol;			//All the information before the column that needs to be sorted (might be null if sorted column is first in the list of columns)
	char* toBeSorted;				//The string containing the information that we're sorting on
	char* afterSortedCol;			//The information after the column that needs to be sorted on (might be null if sorted column is last in the list of columns)
} movieInfo;

//Suggestion: prototype a mergesort function
movieInfo** mergesort(movieInfo** arr, int size);                                                                                    

