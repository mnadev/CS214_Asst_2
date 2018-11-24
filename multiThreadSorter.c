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
#include "multiThreadSorter.h"

void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename);
int isInt(movieInfo** dataRows, int sizeOfArray);
void parseCSV(char* filename, char* columnToSort, char* destDirectory);
int isValidCSV(char* filename, char* columnToSort);
void* dirSearch(void* args);
void* fileThread(void* args);

movieNode* head = NULL;

int isInt = 1;

void setData(movieInfo* A, void* data, char* column) {
	// TODO: VERIFY floats casted to floats, char* casted to char*
	if(strcmp(column, "color") == 0){
		A -> color = (char*) data;
	} else if(strcmp(column, "director_name") == 0){
		A -> director_name = (char*) data;
	} else if(strcmp(column, "num_critic_for_reviews") == 0){
		A -> num_critic_for_reviews = atof(data);
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
	char** columns = (char**) malloc(sizeof(char*)*1);
	
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = malloc(sizeof(char)*500);			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;		//For use in the below do-while loop
	
	int csv = open(filename,O_RDONLY);
	
	// create var to keep track of the number of commas
	int numCommasCurr = 0;
	char* columnCurr = malloc(sizeof(char)*500);
	int i = 0;
	int isInQuotes = 0;
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names	
	do{
		read(csv, &charIn, 1);
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
		if(charIn == '\"') {
			if(isInQuotes == 1) {
				isInQuotes = 0;
			} else {
				isInQuotes = 1;
			}
		} else if(charIn == ',' && isInQuotes == 0) {
			columnCurr[i] = '\0';
			numCommasCurr++;
			columnCurr = realloc(columnCurr, sizeof(char)*(i+1));
			i = 0;
			columns = realloc(columns, sizeof(char*)*(numCommasCurr+1));
			columnCurr = columns[numCommasCurr];
		} else {
			if(i > 500) {
				columnCurr = realloc(columnCurr, sizeof(char)*(1000));;
			}
			columnCurr[i] = charIn;
			i++;
		}
		//printf("%c", charIn);
	}while(charIn != '\n');
	
	columnNames[columnNamesIndex] = '\0';
	columnNames = realloc(columnNames, columnNamesIndex+1);
	
	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	
	char* locOfColumn = strstr(columnNames, columnToSort);

	
	if(locOfColumn == NULL ){
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

	if(hasHeaders(columnNames) == 0){
		write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);
		return;
	}
	
	//Reading through the rest of STDIN for data:
	//int sizeOfArray = 0;
	movieInfo** dataRows = NULL; //Array of pointers to each instance of movieInfo
	movieInfo* A = (movieInfo*)malloc(sizeof(movieInfo));
	
	isInQuotes = 0;
	int numCommas = 0;
	charIn = '\0';
	char* columnData = (char*) malloc(sizeof(char) * 500);
	int columnDataSize = 500;
	int columnDataInd = 0;
	
	int movieInd = 0;
	// count number of commas in current line and number of commas in 
	
	//checking previous char, will help us keep track of if the previous char is a new line if current is new line
	char previousChar = '\0';

	//double new lines will keep track of if two new lines occur in a row
	int doubleNewLines = 0;
	
	//waiter waits and sees for when we come across a malformed csv.
	//if there are two new lines and such then it will wait and see the next char before throwing error
	int waiter = 0;
	int eof = 1;
	while(eof > 0) {
		eof = read(csv, &charIn, 1);
		
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
			
			dataRows = realloc(dataRows, sizeof(movieInfo*)*(movieInd + 1));
			dataRows[movieInd] = A;
			movieInd++;
			numCommas = 0;
			A = (movieInfo*)malloc(sizeof(movieInfo));
		} else if(charIn == '\"') {
			if(isInQuotes == 1) {
				isInQuotes = 0;
			} else {
				isInQuotes = 1;
			}
		} else if(charIn == ',' && isInQuotes == 0) {
			setData(A, (char*) columnData, columns[numCommas]); 
			numCommasCurr++;
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
	}//setData
	
	if(movieInd == 0){
		write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
		return;
	}	
	
	//int isNumeric = isInt(dataRows, sizeOfArray);
	mergesort(dataRows, columnToSort ,0, movieInd - 1, isInt);
	close(csv);	
	
	
	addToFront(dataRows, movieInd);
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

// function to conver float to string
char* ftos(float number){
	if(number == 0) {
		return "0.0";
	}
	
	// find out how many digits in float value
	int lenInt = (int) (floor(log(number)) + 1);
	
	// extract decimal and integer values
	int integer = (int) number;
	
	float decimal = number - (float) integer;
	
	// integer to hold the value of the decimal so it's easier to print
	int decimalToInteger = 0;
	
	// convert the decimal to an int
	while(decimal >= 0) { 
		decimal = decimal * 10;
		
		int intPart = (int) decimal;
		decimal = decimal - (float) intPart;
		
		decimalToInteger = decimalToInteger * 10 +  intPart;
	}
	
	int lenDec = 1;
	// can't take log of 0 so making sure it's not 0;
	if(decimalToInteger > 0) { 
		lenDec = (int) (floor(log(number)) + 1);
	} 
	
	// create buffer and write to it.
	char* numBuffer = (char*) malloc(sizeof(char)*(lenInt + lenDec + 1));
	snprintf(numBuffer, lenInt + lenDec + 1, "%d", number);
	
	
	return numBuffer;
}

// will write output to csv file
void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename){
	//printf("writing to file %s \n",filename);
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
                fprintf(csvFile, A->color);
		fprintf(csvFile, ",");
		fprintf(csvFile, A->director_name);
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->num_critic_for_reviews));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->duration));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->director_facebook_likes));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->actor_3_facebook_likes));
		fprintf(csvFile, ",");
		fprintf(csvFile, A->actor_2_name);
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->actor_1_facebook_likes));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->gross));
		fprintf(csvFile, ",");
		fprintf(csvFile, A->genres);
		fprintf(csvFile, ",");
		fprintf(csvFile, A->actor_1_name);
		fprintf(csvFile, ",");
		fprintf(csvFile, A->movie_title);
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->num_voted_users));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->cast_total_facebook_likes));
		fprintf(csvFile, ",");
		fprintf(csvFile, A->actor_3_name);
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->facenumber_in_poster));
		fprintf(csvFile, ",");
		fprintf(csvFile, A->plot_keywords);
		fprintf(csvFile, ",");
		fprintf(csvFile, A->movie_imdb_link);
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->num_user_for_reviews));
		fprintf(csvFile, ",");
		fprintf(csvFile, A->language);
		fprintf(csvFile, ",");
		fprintf(csvFile, A->country);
		fprintf(csvFile, ",");
		fprintf(csvFile, A->content_rating);
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->budget));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->title_year));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->actor_2_facebook_likes));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->imdb_score));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->aspect_ratio));
		fprintf(csvFile, ",");
		fprintf(csvFile, ftos(A->movie_facebook_likes));
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
	threadRetvals* returnVals = (threadRetvals*)malloc(sizeof(threadRetvals*));
	returnVals->spawnedThreadList = NULL;
	returnVals->spawnedThreadNum = 0;
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
	char* dirToSearch;
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
			pthread_create(newFileThreadHandle, &threadAttrStruct, fileThread, (void*)args);

			threadIDList[threadCountID] = newThreadIDString;
			childrenThreadHandles[threadCountID] = newFileThreadHandle;
			threadCountID++;
	
			free(filepath);
			continue;

		} else if(fileMode == 4){
			dirToSearch = realloc(dirToSearch, sizeof(char)*(strlen(dirToSearch)+strlen(file)+2));
			strcat(dirToSearch, file);		//Appending new directory to current directory path;
			strcat(dirToSearch, "/");		//Forcing the current directory path to always end in / for reasons.

			char* newThreadIDString = (char*)malloc(sizeof(char) * (strlen(args->prevThreadID)+20));
			strcpy(newThreadIDString, args->prevThreadID);
			char* currentThreadCountID = (char*)malloc(sizeof(char)*20);
			snprintf(currentThreadCountID, 19, "%d", threadCountID);
			strcat(newThreadIDString, currentThreadCountID);
			
			threadArgs_DirFile* newArgs = (threadArgs_DirFile*)malloc(sizeof(threadArgs_DirFile));
			//*args = {filepath, args->columnToSort, args->dirDest, newThreadIDString};
			newArgs->pathName = dirToSearch;
			newArgs->columnToSort = args->columnToSort;
			newArgs->dirDest = args->dirDest;
			newArgs->prevThreadID = newThreadIDString;

			pthread_t* newDirThreadHandle = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_attr_t threadAttrStruct;
			pthread_attr_init(&threadAttrStruct);
			pthread_create(newDirThreadHandle, &threadAttrStruct, dirSearch, (void*)args);

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
		threadRetvals** retvals;
		pthread_join(*childrenThreadHandles[q], (void**)retvals);
		if(retvals[0]->spawnedThreadList == NULL){
			continue;
		} else{
			memcpy((threadIDList + totalSpawned), retvals[0]->spawnedThreadList, sizeof(char*)*(retvals[0]->spawnedThreadNum));
			totalSpawned = totalSpawned+(retvals[0]->spawnedThreadNum);
			free(*retvals); //idk why i'm bothering to free this. this program is memoryleakcity.		
		}
		
	}
	//Returning stuff
	threadRetvals* returnVals = (threadRetvals*)malloc(sizeof(threadRetvals*));
	//*returnVals = {childrenThreadHandles, threadCountID}
	returnVals->spawnedThreadList = threadIDList;
	returnVals->spawnedThreadNum = threadCountID;
	pthread_exit((void*)returnVals);
}

int main(int argc, char** argv){
	
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

	printf("\nInitial PID: ");
	int pid = getpid();
	printf("%d\n",pid);


	struct dirent* dirStruct;
	//For use in giving each thread an ID.
	int threadIDListing = 0;
	//List of threadHandles to use when joining later.
	pthread_t** childrenThreadHandles = (pthread_t**)malloc(sizeof(pthread_t*)*256);	//B/c of last assign, max num should only be 255.
	//Master list of threadIDs:
	char** threadIDList_all = (char**)malloc(sizeof(char*)*256);	
	memset(threadIDList_all, 0, 256*sizeof(char*))	;	//writing 0 bytes for easy iteration later. (Detect if == 0)

	printf("TIDS of all child processes: ");
	//threads share memspace so shouldn't need to flush?
	//fflush(stdout);

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
			dirToSearch = realloc(dirToSearch, sizeof(char)*(strlen(dirToSearch)+strlen(file)+2));
			strcat(dirToSearch, file);		//Appending new directory to current directory path;
			strcat(dirToSearch, "/");		//Forcing the current directory path to always end in / for reasons.

			char* spawnedThreadID = (char*)malloc(sizeof(char)*20);
			snprintf(spawnedThreadID,19, "%d", threadIDListing);
			
			threadArgs_DirFile* args = (threadArgs_DirFile*)malloc(sizeof(threadArgs_DirFile));				
			//*args = {filepath, columnToSort, dirDest, spawnedThreadID};
			args->pathName = dirToSearch;
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
	}

	//Joining children threads (only immediate children)
	int totalSpawned = threadIDListing;
	

	int q; //counter variable lol
	for(q = 0; q < threadIDListing; q++){
		threadRetvals** retvals;
		pthread_join(*childrenThreadHandles[q], (void**)retvals);
		if(retvals[0]->spawnedThreadList == NULL){
			continue;
		} else{
			memcpy((threadIDList_all + totalSpawned), retvals[0]->spawnedThreadList, sizeof(char*)*(retvals[0]->spawnedThreadNum));
			totalSpawned = totalSpawned+(retvals[0]->spawnedThreadNum);
			free(*retvals); //idk why i'm bothering to free this. this program is memoryleakcity.		
		}
		
	}
	
	printf("\n Total number of Threads: %d\n", totalSpawned);
	
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
	char * columnNames = "";
	csvwrite(head -> data, head -> arrLen, columnNames, fileToWrite);
	
	free(fileToWrite); 
	
	free(dirDest);
	free(columnToSort);
	free(dirToSearch);
	return 0;
}


