#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<math.h>
#include<limits.h>
#include "multiThreadSorter.h"

void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename);
void parseCSV(char* filename, char* columnToSort, char* destDirectory);
void* dirSearch(void* args);
void* fileThread(void* args);

movieNode* head = NULL;
pthread_mutex_t* headMutex;

int isInt = 1;
int processPID;

void addToFront(movieInfo** data, int arrLen) {
	// create new head node and set data 
	movieNode* newHead =(movieNode*) malloc(sizeof(movieNode));
	newHead -> data = data;
	newHead -> arrLen = arrLen;
	//set next node to current head
	newHead -> next = head;

	//set head to new head
	head = newHead;
}
movieInfo* initMovieInfo() {
	movieInfo* A = (movieInfo*)malloc(sizeof(movieInfo));
	A->color = '\0';
	A->director_name = '\0';

	A->actor_2_name = '\0';

	A->genres = '\0';
	A->actor_1_name = '\0';
	A->movie_title = '\0';

	A->actor_3_name = '\0';

	A->plot_keywords = '\0';
	A->movie_imdb_link = '\0';

	A->language = '\0';
	A->country = '\0';
	A->content_rating = '\0';
	
	
	A -> num_critic_for_reviews = (float) INT_MIN;
	A -> duration = (float) INT_MIN;
	A -> director_facebook_likes = (float) INT_MIN;
	A -> actor_3_facebook_likes = (float) INT_MIN;
	A -> actor_1_facebook_likes = (float) INT_MIN;
	A -> gross = (float) INT_MIN;
	A -> num_voted_users = (float) INT_MIN;
	A -> cast_total_facebook_likes = (float) INT_MIN;
	A -> facenumber_in_poster = (float) INT_MIN;
	A -> num_user_for_reviews = (float) INT_MIN;
	A -> budget = (float) INT_MIN;
	A -> title_year = (float) INT_MIN;
	A -> actor_2_facebook_likes = (float) INT_MIN;
	A -> imdb_score = (float) INT_MIN;
	A -> aspect_ratio = (float) INT_MIN;
	A -> movie_facebook_likes = (float) INT_MIN;
	return A;
}
void mergeSortNodes(char* category){
	//return if head is null or there is only one node in list
	if(head == NULL) {
		return;
	}

	if(head -> next == NULL) {
		return;
	}
	// get third node in list, could be NULL, don't matter
	movieNode* next = head -> next -> next;
	
	movieInfo** headData = head -> data;
	movieInfo** nextData = head -> next -> data;
	// mergesort the data
	movieInfo** mergedData = mergeNodeData(headData, nextData, head -> arrLen, head -> next -> arrLen, category, isInt);

	// change head node and set data
	head -> data = mergedData;
	head -> arrLen = (head -> next -> arrLen) + (head -> arrLen);

	movieNode* nextToFree = head -> next;
	head -> next = NULL;
	// free second nodes, also free movieInfo Arrs
	free(headData);
	free(nextData);
	free(nextToFree);
	
	//set next node equal to third node, could be NULL
	head -> next = next;
}

void setData(movieInfo* A, void* data, char* column) {
	// TODO: VERIFY floats casted to floats, char* casted to char*
	if(strcmp(column, "color") == 0){
		A -> color = (char*) data;
	} else if(strcmp(column, "director_name") == 0){
		A -> director_name = (char*) data;
	} else if(strcmp(column, "num_critic_for_reviews") == 0){
		A -> num_critic_for_reviews = atof((char*)data);
	} else if(strcmp(column, "duration") == 0){
		A -> duration = atof((char*)data);
	} else if(strcmp(column, "director_facebook_likes") == 0){
		A -> director_facebook_likes = atof((char*)data);
	} else if(strcmp(column, "actor_3_facebook_likes") == 0){
		A -> actor_3_facebook_likes = atof((char*)data);
	} else if(strcmp(column, "actor_2_name") == 0){
		A -> actor_2_name = (char*) data;
	} else if(strcmp(column, "actor_1_facebook_likes") == 0){
		A -> actor_1_facebook_likes = atof((char*)data);
	} else if(strcmp(column, "gross") == 0){
		A -> gross = atof((char*)data);
	} else if(strcmp(column, "genres") == 0){
		A -> genres = (char*) data;
	} else if(strcmp(column, "actor_1_name") == 0){
		A -> actor_1_name = (char*) data;
	} else if(strcmp(column, "movie_title") == 0){
		A -> movie_title = (char*) data;
	} else if(strcmp(column, "num_voted_users") == 0){
		A -> num_voted_users = atof((char*)data);
	} else if(strcmp(column, "cast_total_facebook_likes") == 0){
		A -> cast_total_facebook_likes = atof((char*)data);
	} else if(strcmp(column, "actor_3_name") == 0){
		A -> actor_3_name = (char*) data;
	} else if(strcmp(column, "facenumber_in_poster") == 0){
		A -> facenumber_in_poster = atof((char*)data);
	} else if(strcmp(column, "plot_keywords") == 0){
		A -> plot_keywords = (char*) data;
	} else if(strcmp(column, "movie_imdb_link") == 0){
		A -> movie_imdb_link = (char*) data;
	} else if(strcmp(column, "num_user_for_reviews") == 0){
		A -> num_user_for_reviews = atof((char*)data);
	} else if(strcmp(column, "language") == 0){
		A -> language = (char*) data;
	} else if(strcmp(column, "country") == 0){
		A -> country = (char*) data;
	} else if(strcmp(column, "content_rating") == 0){
		A -> content_rating = (char*) data;
	} else if(strcmp(column, "budget") == 0){
		A -> budget = atof((char*)data);
	} else if(strcmp(column, "title_year") == 0){
		A -> title_year = atof((char*)data);
	} else if(strcmp(column, "actor_2_facebook_likes") == 0){
		A -> actor_2_facebook_likes = atof((char*)data);
	} else if(strcmp(column, "imdb_score") == 0){
		A -> imdb_score = atof((char*)data);
	} else if(strcmp(column, "aspect_ratio") == 0){
		A -> aspect_ratio = atof((char*)data);
	} else if(strcmp(column, "movie_facebook_likes") == 0){
		A -> movie_facebook_likes = atof((char*)data);
	} else{
		return;
	}
}

//function to parse through csv file
void parseCSV(char* filename, char* columnToSort, char* destDirectory) {
	// functionality for isInt built into merge sort, not necessary to implement
	int pathLen = strlen(filename);
	// return if no .csv file extension
	if(!(filename[pathLen-1] == 'v' && filename[pathLen-2] == 's' && filename[pathLen-3] == 'c' && filename[pathLen-4] == '.')){
		return;
	}

	int numCommasB4Sort = 0;		//The number of commas before the column to be sorted is reached.
	
	// create array of columns
	char** columns = (char**) malloc(sizeof(char*)*28);
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = malloc(sizeof(char)*500);			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;		//For use in the below do-while loop
	int csv = open(filename,O_RDONLY);
	char previousChar = ',';
	// create var to keep track of the number of commas
	int numCommasCurr = 0;
	int sizeColumnCurr = 500;
	int i = 0;
	int isInQuotes = 0;
	int index = 0;
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names	
	do{
		char* columnCurr = (char * )malloc(sizeof(char)*26);
		while(1){
			int eof = read(csv, &charIn, 1);
			if(eof == 0){
				write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);	
				return;	
			}
			columnNames[columnNamesIndex] = charIn;
			columnNamesIndex++;

			if(charIn == '\n'){
				columnCurr[i] = '\0';
				columns[index] = columnCurr;
				i++;
				break;
			} else if(charIn == ',') {
				if(previousChar == ',') {
					write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);	
					return;	
				}
				columnCurr[i] = '\0';
				numCommasCurr++;
				i++;
				
				char* reallocTest = realloc(columnCurr, sizeof(char)*(i));
				if(reallocTest == NULL){
					reallocTest = realloc(columnCurr, sizeof(char)*i);
				} else{
					columnCurr = reallocTest;
				}
				i = 0;
				// for 28 headers, we'd have 27 commas, anything above is bad.
				if(numCommasCurr >= 28){
					write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);	
					return;		
				}
				columns[index] = columnCurr;
				index++;
				previousChar = charIn;	
				break;			
			} else {
				// the max length of a valid header is 25.
				if(i >= 25) {
					write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);	
					return;			
				}
				columnCurr[i] = charIn;
				i++;
				previousChar = charIn;
			}
		}
		//printf("%c", charIn);
	}while(charIn != '\n');

	if(numCommasCurr > 0) {
		if(columns[numCommasCurr] == NULL) {
			//columns = realloc(columns, sizeof(char*)*(numCommasCurr));	
		}
	}
	columnNames[columnNamesIndex] = '\0';
	//columnNames = realloc(columnNames, columnNamesIndex+1);
	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	
	char* locOfColumn = strstr(columnNames, columnToSort);

	if(locOfColumn == NULL ){
		free(columns);
		free(columnNames);
		write(STDERR, "Error while checking validity: The column to be sorted that was input as the 2nd parameter is not contained within the CSV.\n", 124);
		return;
	}
	//Searching for number of commas before column to be sorted
	//(Assumes that column names don't have commas in them, which they shouldn't for this assignment.
	for(i = 0; i <= (locOfColumn - columnNames); i++){

		if(columnNames[i] == ','){
			numCommasB4Sort++;		
		}
	} 
	//printf("here\n");
	if(hasHeaders(columnNames) == 0){
		free(columns);
		free(columnNames);
		write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);
		return;
	} 
	//Reading through the rest of STDIN for data:
	//int sizeOfArray = 0;
	movieInfo** dataRows = malloc(sizeof(movieInfo*)*1); //Array of pointers to each instance of movieInfo
	movieInfo* A = initMovieInfo();
	
	isInQuotes = 0;
	int numCommas = 0;
	charIn = '\0';
	char* columnData = (char*) malloc(sizeof(char) * 500);
	int columnDataSize = 500;
	int columnDataInd = 0;
	int movieInd = 0;
	// count number of commas in current line and number of commas in 
	//checking previous char, will help us keep track of if the previous char is a new line if current is new line
	previousChar = ',';

	//double new lines will keep track of if two new lines occur in a row
	int doubleNewLines = 0;
	
	//waiter waits and sees for when we come across a malformed csv.
	//if there are two new lines and such then it will wait and see the next char before throwing error
	int waiter = 0;
	int eof = 1;
	while(eof > 0) {
		eof = read(csv, &charIn, 1);
		if(eof == 0 && movieInd == 0) {
			write(STDERR, "Error while checking validity, Malformed CSV\n", 45);
			return;
		}
		if(doubleNewLines > 0 && eof > 0) {
			write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
			return;
		}
		
		// if we have a disparate amount of commas in each line, we will check if we have
		// the weird two lines error by checking if it is the end of the file
		if(waiter == 1 && eof > 0) {
			write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
			return;
		}

		waiter = 0;
		doubleNewLines = 0;
		
		if(previousChar == '\n' && charIn == '\n') {
			doubleNewLines += 1;
		}
		
		if(charIn == '\n') {
			if(numCommasCurr != numCommas && doubleNewLines == 0){
					write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
					return;
			}
			
			if(numCommasCurr != numCommas && doubleNewLines != 0) {
				waiter = 1;
			}
			if(movieInd > 0) {
				dataRows = realloc(dataRows, sizeof(movieInfo*)*(movieInd + 1));
			}
			if(doubleNewLines <=0) {
				columnData[columnDataInd] = '\0';
				columnDataInd++;
				setData(A, (void*) columnData, columns[numCommas]); 
				numCommas++;
				columnData = (char*) malloc(sizeof(char) * 500);
				columnDataInd = 0;
			}
			dataRows[movieInd] = A;
			movieInd++;
			numCommas = 0;
			A = initMovieInfo();
		} else if(charIn == '\"') {
			if(isInQuotes == 1) {
				isInQuotes = 0;
			} else {
				isInQuotes = 1;
			}
		} else if(charIn == ',' && isInQuotes == 0) {
			if(numCommas >= numCommasCurr) {
				write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
				return;
			}
			columnData[columnDataInd] = '\0';
			columnDataInd++;
			setData(A, (void*) columnData, columns[numCommas]); 
			numCommas++;
			columnData = (char*) malloc(sizeof(char) * 500);
			columnDataInd = 0;
		} else if(charIn == '\0') {
			break;
		} else {
			if(columnDataInd >= columnDataSize){
				columnData = (char*)realloc(columnData, sizeof(char)*2*columnDataSize);
				columnDataSize = columnDataSize*2;
			}
			columnData[columnDataInd] = charIn;
			columnDataInd++;
		}
		previousChar = charIn;
	}
	if(movieInd == 0){
		write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
		return;
	}	
	mergesort(dataRows, columnToSort ,0, movieInd - 1, isInt);
	close(csv);	
	
	pthread_mutex_lock(headMutex);
	addToFront(dataRows, movieInd);
	pthread_mutex_unlock(headMutex);
}


//Function to check if all column headers belong to the set of movie_data headers (Assign1 Requirement)
//Returns 1 if true, 0 if false;
int hasHeaders(char* columnNames){
	char headerName[50];
	int headerIndex = 0;
	
	int hasTitle = 0;
	int endOfHeaders = 0;
	int i = 0;	
	while(columnNames[i] != '\0'){
		switch(columnNames[i]){
			case ',':
				CheckHeader: headerName[headerIndex] = '\0';
				if(strcmp(headerName, "color") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "director_name") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "num_critic_for_reviews") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "duration") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "director_facebook_likes") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "actor_3_facebook_likes") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "actor_2_name") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "actor_1_facebook_likes") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "gross") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "genres") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "actor_1_name") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "movie_title") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "num_voted_users") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "cast_total_facebook_likes") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "actor_3_name") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "facenumber_in_poster") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "plot_keywords") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "movie_imdb_link") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "num_user_for_reviews") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "language") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "country") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "content_rating") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "budget") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "title_year") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "actor_2_facebook_likes") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "imdb_score") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "aspect_ratio") == 0){
					headerIndex = 0;
					i++;
					break;
				} else if(strcmp(headerName, "movie_facebook_likes") == 0){
					headerIndex = 0;
					i++;
					break;
				} else{
					if(headerName[0] == '\0'){
						return 1;
					}
					return 0;
				}
				break;
			default:
				if(columnNames[i+1] == '\0'){
					goto CheckHeader;
				} else if(columnNames[i+1] == '\r'){
					headerName[headerIndex] = columnNames[i];
					headerIndex++;
					i++;
					endOfHeaders = 1;
					goto CheckHeader;
				}
				headerName[headerIndex] = columnNames[i];
				headerIndex++;
				i++;
				break;
		}
	}
	return 1;
}

// will write output to csv file
void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename){
	// though about using file descriptors but we'd have to keep track of size
	// although we could use strlen?
	//int p_csv = open(filename, O_WRONLY);
	FILE *csvFile; 
	csvFile = fopen(filename, "w+");
       	fprintf(csvFile, categories);
        int i = 0;
	
	// TODO: ACCOUNT FOR DECIMALS WHEN WRITING
        while(i < size){
                movieInfo* A = movieArr[i];
                fprintf(csvFile, "%s", A->color);
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->director_name);
		fprintf(csvFile, ",");
		if(A->num_critic_for_reviews == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->num_critic_for_reviews );
		}
		fprintf(csvFile, ",");
		if(A->duration == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", A->duration);
		}
		fprintf(csvFile, ",");
		if(A->director_facebook_likes == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->director_facebook_likes);
		}
		fprintf(csvFile, ",");
		if(A->actor_3_facebook_likes == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->actor_3_facebook_likes);
		}
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->actor_2_name);
		fprintf(csvFile, ",");
		if(A->actor_1_facebook_likes == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d",(int)A->actor_1_facebook_likes);
		}
		fprintf(csvFile, ",");
		if(A->gross == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile,  "%d", A->gross);
		}
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->genres);
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->actor_1_name);
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->movie_title);
		fprintf(csvFile, ",");
		if(A->num_voted_users == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->num_voted_users);
		}
		fprintf(csvFile, ",");
		if(A->cast_total_facebook_likes == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->cast_total_facebook_likes);
		}
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->actor_3_name);
		fprintf(csvFile, ",");
		if(A->facenumber_in_poster == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->facenumber_in_poster);
		}
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->plot_keywords);
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->movie_imdb_link);
		fprintf(csvFile, ",");
		if(A->num_user_for_reviews == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->num_user_for_reviews);
		}
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->language);
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->country);
		fprintf(csvFile, ",");
		fprintf(csvFile, "%s", A->content_rating);
		fprintf(csvFile, ",");
		if(A->budget == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", A->budget);
		}
		fprintf(csvFile, ",");
		if(A->title_year == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile,  "%d", (int)A->title_year);
		}
		fprintf(csvFile, ",");
		if(A->actor_2_facebook_likes == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->actor_2_facebook_likes);
		}
		fprintf(csvFile, ",");
		if(A->imdb_score == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", A->imdb_score);
		}
		fprintf(csvFile, ",");
		if(A->aspect_ratio == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", A->aspect_ratio);
		}
		fprintf(csvFile, ",");
		if(A->movie_facebook_likes == (float) INT_MIN) {
			fprintf(csvFile, "");
		} else {
			fprintf(csvFile, "%d", (int)A->movie_facebook_likes);
		}
		fprintf(csvFile, "\n");
		i++;
        }
	fclose(csvFile);
}

void* fileThread(void* arguments){
	
	threadArgs_DirFile* args = (threadArgs_DirFile*)arguments;
	//fileThread should never spawn another thread, so we should be fine without catting a threadID here.

	parseCSV(args->pathName, args->columnToSort, args->dirDest);

	//Returning stuff
	
	//TODO: SEG FAULT HERE??????
	threadRetvals* returnVals = (threadRetvals*)malloc(sizeof(threadRetvals));
	returnVals->spawnedThreadList = NULL;
	returnVals->spawnedThreadNum = 0;

	/*printf("\nInitial PID: %d\n", processPID);
	printf("TIDS of all child processes: ");
	printf("\n Total number of Threads: 0\n");*/

	pthread_exit((void*)returnVals);

}


void* dirSearch(void* arguments){
	threadArgs_DirFile* args = (threadArgs_DirFile*)arguments;
	//Quick note that args is going to be a struct containing pathname, columnToSort, and dirDest and mutex stuff;
	
	int threadCountID = 0;	//For any threads that this thread spawns.
	//List of threadHandles to use when joining later.
	pthread_t** childrenThreadHandles = (pthread_t**)malloc(sizeof(pthread_t*)*256);	//B/c of last assign, max num should only be 255.
	//Master list of threadIDs:
	char** threadIDList = (char**)malloc(sizeof(char*)*256);	
	memset(threadIDList, 0, 256*sizeof(char*))	;	//writing 0 bytes for easy iteration later. (Detect if == 0)

	//Initializing variables needed for the while loop that was pasta from before.
	char* pathName = args->pathName;
	struct dirent* dirStruct;
	char* dirToSearch = pathName;
	DIR* currDir = opendir(pathName);
	while(31337) {
		if((dirStruct = readdir(currDir)) == NULL) {
			break;
		}
		char* file = dirStruct -> d_name;
		int fileMode = (int)(dirStruct -> d_type);
		if(!strcmp(file, ".git") || strcmp(file, ".") == 0 || strcmp(file, "..") == 0) {
			continue;
		}
		int status = 0;
 		if(fileMode == 8){
			char* filepath = (char*)malloc(sizeof(char)*10000);
			strcpy(filepath, dirToSearch);		//Because i need to create a new string for full file path.
			strcat(filepath, file);	//Concatting filename to file path for full file path 

			char* newThreadIDString = (char*)malloc(sizeof(char) * (strlen(args->prevThreadID)+20));
			strcpy(newThreadIDString, args->prevThreadID);
			char* currentThreadCountID = (char*)malloc(sizeof(char)*20);
			snprintf(currentThreadCountID, 19, "%d", threadCountID);
			strcat(newThreadIDString, currentThreadCountID);

			threadArgs_DirFile* newArgs = (threadArgs_DirFile*)malloc(sizeof(threadArgs_DirFile));
			newArgs->pathName = filepath;
			newArgs->columnToSort = args->columnToSort;
			newArgs->dirDest = args->dirDest;
			newArgs->prevThreadID = newThreadIDString;
			
			pthread_t* newFileThreadHandle = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_attr_t threadAttrStruct;
			pthread_attr_init(&threadAttrStruct);
			pthread_create(newFileThreadHandle, &threadAttrStruct, fileThread, (void*)newArgs);

			threadIDList[threadCountID] = newThreadIDString;
			childrenThreadHandles[threadCountID] = newFileThreadHandle;
			threadCountID++;

			continue;

		} else if(fileMode == 4){
			char* newDirToSearch = malloc(sizeof(char)*strlen(dirToSearch));
			strcpy(newDirToSearch, dirToSearch);
			char* reallocTest = realloc(newDirToSearch, sizeof(char)*(strlen(newDirToSearch)+strlen(file)+2));
			while(reallocTest == NULL){
				reallocTest = realloc(newDirToSearch, sizeof(char)*(strlen(newDirToSearch)+strlen(file)+2)); 
			}
			newDirToSearch = reallocTest;
			//newDirToSearch = realloc(newDirToSearch, sizeof(char)*(strlen(dirToSearch)+strlen(file)+2));
			strcat(newDirToSearch, file);		//Appending new directory to current directory path;
			strcat(newDirToSearch, "/");		//Forcing the current directory path to always end in / for reasons.

			char* newThreadIDString = (char*)malloc(sizeof(char) * (strlen(args->prevThreadID)+20));
			strcpy(newThreadIDString, args->prevThreadID);
			char* currentThreadCountID = (char*)malloc(sizeof(char)*20);
			snprintf(currentThreadCountID, 19, "%d", threadCountID);
			strcat(newThreadIDString, currentThreadCountID);
			
			threadArgs_DirFile* newArgs = (threadArgs_DirFile*)malloc(sizeof(threadArgs_DirFile));
			//*args = {filepath, args->columnToSort, args->dirDest, newThreadIDString};
			newArgs->pathName = newDirToSearch;
			newArgs->columnToSort = args->columnToSort;
			newArgs->dirDest = args->dirDest;
			newArgs->prevThreadID = newThreadIDString;

			pthread_t* newDirThreadHandle = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_attr_t threadAttrStruct;
			pthread_attr_init(&threadAttrStruct);
			pthread_create(newDirThreadHandle, &threadAttrStruct, dirSearch, (void*)newArgs);

			threadIDList[threadCountID] = newThreadIDString;
			childrenThreadHandles[threadCountID] = newDirThreadHandle;
			threadCountID++;
			continue;
		} else{
			//If for some reason there's a thing that's not a file or directory. Gotta handle all the errors dawg.	
			continue;
		}
	}
	//Joining children threads (only immediate children)
	int totalSpawned = threadCountID;
	
	int q; //counter variable lol
	for(q = 0; q < threadCountID; q++){
		threadRetvals** retvals = (threadRetvals**)malloc(sizeof(threadRetvals*));
		pthread_join(*(childrenThreadHandles[q]), (void**)retvals);
		if(retvals[0]->spawnedThreadList == NULL){
			continue;
		} else{
			memcpy((threadIDList + totalSpawned), retvals[0]->spawnedThreadList, sizeof(char*)*(retvals[0]->spawnedThreadNum));
			totalSpawned = totalSpawned+(retvals[0]->spawnedThreadNum);
			//free(*retvals); //idk why i'm bothering to free this. this program is memoryleakcity.		
		}
		
	}
	
	//Printing metadata
	/*printf("\nInitial PID: %d\n", processPID);
	printf("TIDS of all child processes: ");
	for(q = 0; q < totalSpawned; q++){
		printf("%s, ", threadIDList[q]);	
	}
	printf("\n Total number of Threads: %d\n", totalSpawned);*/

	//Returning stuff
	threadRetvals* returnVals = (threadRetvals*)malloc(sizeof(threadRetvals*));
	//*returnVals = {childrenThreadHandles, threadCountID}
	returnVals->spawnedThreadList = threadIDList;
	returnVals->spawnedThreadNum = totalSpawned;
	pthread_exit((void*)returnVals);
}

void mergeThread(char* column) {	
	pthread_mutex_lock(headMutex);
	mergeSortNodes(column);
	pthread_mutex_unlock(headMutex);
}

int main(int argc, char** argv){
	
	processPID = getpid();	//Setting the global

	//Input flags of the program and whether they are present: Index 0 = -c, Index 1 = -d, Index 2 = -o
	int flagsPresent[] = {0,0,0};		
	char* columnToSort;
	char* dirToSearch = (char*)malloc(sizeof(char)*3);
		strcpy(dirToSearch, "./");	//dirToSearch defaults to current directory if not changed by flag later.
	char* dirDest = NULL;

	//Write to STDERR if there are fewer than the required number of args
	if(argc < 3){
		write(STDERR, "Fatal Error: Insufficient arguments.\n", 38);
		write(STDOUT, "Fatal Error: Insufficient arguments.\n", 38);
		return -1;
	}
	if(argc > 7){
		write(STDERR, "Fatal Error: Excess arguments.\n", 31);
		write(STDOUT, "Fatal Error: Excess arguments.\n", 31);
		return -1;
	}
	//Flag Handling:
	int flag;
	while((flag = getopt(argc, argv, "c:d:o:")) != -1){
		switch(flag){		
			case 'c':
				if(flagsPresent[0] == 1){
					write(STDERR, "Fatal Error: Repeated argument -c.\n", 36); 
					write(STDOUT, "Fatal Error: Repeated argument -c.\n", 36); 
					return -1;
				} else{
					flagsPresent[0] = 1;				//The -c parameter is present
					columnToSort = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
					strcpy(columnToSort, optarg);		//copying the column name from optarg to columnToSort
					break;
				}
			case 'd':
				if(flagsPresent[1] == 1){
					write(STDERR, "Fatal Error: Repeated argument -d.\n", 36); 
					write(STDOUT, "Fatal Error: Repeated argument -d.\n", 36); 
					return -1;
				} else{
					flagsPresent[1] = 1;
					dirToSearch = (char*)malloc(sizeof(char)*(strlen(optarg)+2));
					if(optarg[strlen(optarg)-1] == '/'){
						strcpy(dirToSearch, optarg);
					} else{
						strcpy(dirToSearch, optarg);
						strcat(dirToSearch, "/");
					}
					break;
				}
			case 'o':
				if(flagsPresent[2] == 1){
					write(STDERR, "Fatal Error: Repeated argument -o.\n", 36); 
					write(STDOUT, "Fatal Error: Repeated argument -o.\n", 36); 
					return -1;
				} else{
					flagsPresent[2] = 1;
					dirDest = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
					strcpy(dirDest, optarg);
					break;
				}				
			case '?':
				write(STDERR, "Fatal Error: Unknown arguments.\n", 33);
				write(STDOUT, "Fatal Error: Unknown arguments.\n", 33);
				return -1;
		}

	}
	//Handles excess arguments
	if(optind < argc){
		write(STDERR, "Fatal Error: Unknown arguments.\n", 33);
		write(STDOUT, "Fatal Error: Unknown arguments.\n", 33);
		return -1;
	}
	//Write to STDERR if -c flag is not present
	if(flagsPresent[0] != 1){
		write(STDERR, "Fatal Error: The first argument of the program must be '-c to sort by column.\n", 81);
		write(STDOUT, "Fatal Error: The first argument of the program must be '-c to sort by column.\n", 81);
		return -1;
	}
	if(flagsPresent[2] == 1){
		DIR* testOpen;
		testOpen = opendir(dirDest);
		if(errno == ENOENT){
			write(STDERR, "Fatal Error: Output directory not found.\n", 41);
			write(STDOUT, "Fatal Error: Output directory not found.\n", 41);
			return -1;
		}
	}
	
	if(strcmp(columnToSort, "color") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "director_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "num_critic_for_reviews") == 0){
	} else if(strcmp(columnToSort, "duration") == 0){
	} else if(strcmp(columnToSort, "director_facebook_likes") == 0){
	} else if(strcmp(columnToSort, "actor_3_facebook_likes") == 0){
	} else if(strcmp(columnToSort, "actor_2_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "actor_1_facebook_likes") == 0){
	} else if(strcmp(columnToSort, "gross") == 0){
	} else if(strcmp(columnToSort, "genres") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "actor_1_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "movie_title") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "num_voted_users") == 0){
	} else if(strcmp(columnToSort, "cast_total_facebook_likes") == 0){
	} else if(strcmp(columnToSort, "actor_3_name") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "facenumber_in_poster") == 0){
	} else if(strcmp(columnToSort, "plot_keywords") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "movie_imdb_link") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "num_user_for_reviews") == 0){
	} else if(strcmp(columnToSort, "language") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "country") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "content_rating") == 0){
		isInt = 0;
	} else if(strcmp(columnToSort, "budget") == 0){
	} else if(strcmp(columnToSort, "title_year") == 0){
	} else if(strcmp(columnToSort, "actor_2_facebook_likes") == 0){
	} else if(strcmp(columnToSort, "imdb_score") == 0){
	} else if(strcmp(columnToSort, "aspect_ratio") == 0){
	} else if(strcmp(columnToSort, "movie_facebook_likes") == 0){
	} else{
	}
	DIR *currDir;
	currDir = opendir(dirToSearch);
	if(errno == ENOENT){
		write(STDERR, "Fatal Error: Directory to search not found.\n", 44);
		write(STDOUT, "Fatal Error: Directory to search not found.\n", 44);
		return -1;
	}

	//Initializing mutex stuff:
	headMutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(headMutex, NULL);
	

	struct dirent* dirStruct;
	//For use in giving each thread an ID.
	int threadIDListing = 0;
	//List of threadHandles to use when joining later.
	pthread_t** childrenThreadHandles = (pthread_t**)malloc(sizeof(pthread_t*)*256);	//B/c of last assign, max num should only be 255.
	//Master list of threadIDs:
	char** threadIDList_all = (char**)malloc(sizeof(char*)*256);	
	memset(threadIDList_all, 0, 256*sizeof(char*))	;	//writing 0 bytes for easy iteration later. (Detect if == 0)
	int* numThreadSort =(int *) malloc(sizeof(int));
	*numThreadSort = 0;
	int* sortThreadIDs = (int *) malloc(sizeof(int)*256); 
	char * file;		//used to determine i-node type: directory, file, or some other thing
	while(31337) {
		if((dirStruct = readdir(currDir)) == NULL) {
			break;
		}
		
		file = dirStruct -> d_name;
		int fileMode = (int)(dirStruct -> d_type);
		if(!strcmp(file, ".git") || strcmp(file, ".") == 0 || strcmp(file, "..") == 0) {
			continue;
		}

 		if(fileMode == 8){
			char* filepath = (char*)malloc(sizeof(char)*10000);
			strcpy(filepath, dirToSearch);		//Because i need to create a new string for full file path.
			strcat(filepath, file);	//Concatting filename to file path for full file path 

			char* spawnedThreadID = (char*)malloc(sizeof(char)*20);
			snprintf(spawnedThreadID, 19, "%d", threadIDListing); 
			threadArgs_DirFile* args = (threadArgs_DirFile*)malloc(sizeof(threadArgs_DirFile));
			//*args = {filepath, columnToSort, dirDest, spawnedthreadID};
			args->pathName = filepath;
			args->columnToSort = columnToSort;
			args->dirDest = dirDest;
			args->prevThreadID = spawnedThreadID;
			
			pthread_t* newFileThreadHandle = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_attr_t threadAttrStruct;
			pthread_attr_init(&threadAttrStruct);
			pthread_create(newFileThreadHandle, &threadAttrStruct, fileThread, (void*)args);

			threadIDList_all[threadIDListing] = spawnedThreadID;
			childrenThreadHandles[threadIDListing] = newFileThreadHandle;	//Storing address of threadHandle for joining later
			threadIDListing++;	

			free(filepath);
			continue;

		} else if(fileMode == 4){
			char* newDirToSearch = malloc(sizeof(char)*strlen(dirToSearch));
			strcpy(newDirToSearch, dirToSearch);
			char* reallocTest = realloc(newDirToSearch, sizeof(char)*(strlen(newDirToSearch)+strlen(file)+2));
			while(reallocTest == NULL){
				reallocTest = realloc(newDirToSearch, sizeof(char)*(strlen(newDirToSearch)+strlen(file)+2)); 
			}
			newDirToSearch = reallocTest;
			//dirToSearch = realloc(dirToSearch, sizeof(char)*(strlen(dirToSearch)+strlen(file)+2));
			strcat(newDirToSearch, file);		//Appending new directory to current directory path;
			strcat(newDirToSearch, "/");		//Forcing the current directory path to always end in / for reasons.

			char* spawnedThreadID = (char*)malloc(sizeof(char)*20);
			snprintf(spawnedThreadID,19, "%d", threadIDListing);
			
			threadArgs_DirFile* args = (threadArgs_DirFile*)malloc(sizeof(threadArgs_DirFile));				
			//*args = {filepath, columnToSort, dirDest, spawnedThreadID};
			args->pathName = newDirToSearch;
			args->columnToSort = columnToSort;
			args->dirDest = dirDest;
			args->prevThreadID = spawnedThreadID;

			pthread_t* newDirThreadHandle = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_attr_t threadAttrStruct;
			pthread_attr_init(&threadAttrStruct);
			pthread_create(newDirThreadHandle, &threadAttrStruct, dirSearch, (void*)args);

			threadIDList_all[threadIDListing] = spawnedThreadID;
			childrenThreadHandles[threadIDListing] = newDirThreadHandle;	//Same as above
			threadIDListing++;

		} else{
			//If for some reason there's a thing that's not a file or directory. Gotta handle all the errors dawg.	
			continue;
		}
		
		if(head != NULL) {
			if(head -> next != NULL) {
				pthread_t* threadSort = (pthread_t*)malloc(sizeof(pthread_t));
				pthread_attr_t threadAttr;
				pthread_attr_init(&threadAttr);
				*numThreadSort++;
				sortThreadIDs[*numThreadSort - 1] = (int) processPID / *numThreadSort;
				pthread_create(threadSort, &threadAttr, mergeThread, columnToSort);

			}
		}

	
	}

	//Joining children threads (only immediate children)
	int totalSpawned = threadIDListing;

	int q; //counter variable lol
	for(q = 0; q < threadIDListing; q++){
		threadRetvals** retvals = (threadRetvals**)malloc(sizeof(threadRetvals*));
		pthread_join(*(childrenThreadHandles[q]), (void**)retvals);
		if(retvals[0]->spawnedThreadList == NULL){
			continue;
		} else{
			memcpy((threadIDList_all + totalSpawned), retvals[0]->spawnedThreadList, sizeof(char*)*(retvals[0]->spawnedThreadNum));
			totalSpawned = totalSpawned+(retvals[0]->spawnedThreadNum);
			//free(*retvals); //idk why i'm bothering to free this. this program is memoryleakcity.		
		}
		
	}
	printf("\nInitial PID: %d\n", processPID);
	printf("TIDS of all child processes: ");
	for(q = 0; q < totalSpawned; q++){
		printf("%s, ", threadIDList_all[q]);	
	}
	//int total = totalSpawned + *numThreadSort;
	/*while(*numThreadSort > 0) {
		*numThreadSort--;
		printf("%d, ", sortThreadIDs[*numThreadSort]);
	}*/
	printf("\n Total number of Threads: %d\n", totalSpawned);
	if(head != NULL) {	
		while(head -> next != NULL) {
			mergeSortNodes(columnToSort);
		}
	
		//TODO: There should probably be a call to csvwrite here once we have giant mega super linked list of movieInfo.
		int isAbsolutePath = 1;
		if(dirDest != NULL) {
			if(*(dirDest) == '/') {
				isAbsolutePath = 0;
			}
		}	
		char* fileToWrite = (char*) malloc(sizeof(char) * 256);
	
		if(dirDest != NULL) {
			if(isAbsolutePath == 1) {
				snprintf(fileToWrite, 256, "%s/AllFiles-sorted-%s.csv\0", dirDest,columnToSort);
			} else {
				snprintf(fileToWrite, 256, "./%s/AllFiles-sorted-%s.csv\0", dirDest, columnToSort);
			}
		} else {
			snprintf(fileToWrite, 256, "AllFiles-sorted-%s.csv\0",columnToSort);
		}
		//TODO: write column names
		char * columnNames = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n";
		csvwrite(head -> data, head -> arrLen, columnNames, fileToWrite);
		free(fileToWrite); 
	}
	free(dirDest);
	free(columnToSort);
	free(dirToSearch);
	pthread_mutex_destroy(headMutex);
	return 0;
}


