#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include "multiThreadSorter.h"

void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename);
int isInt(movieInfo** dataRows, int sizeOfArray);
void parseCSV(char* filename, char* columnToSort, char* destDirectory);
int isValidCSV(char* filename, char* columnToSort);

int isInt(movieInfo** dataRows, int sizeOfArray) {
	// this integer acts as a boolean
	// is 0 if the data is not an int, now is 1 if the data is an int
	int isInt = 1;
	
	//create temp pointer to iterate through all of array and check if each item is a number
	//because it is possible for a movie to be a number, e.g. '300', and that would be a problem
	//so we should check all of them to be sure
	movieInfo** tempPtrCheckInt = dataRows;
	int i = 0;
		
	while(i < sizeOfArray) {
		//iterate through each char, checking if it is int
		// NOTENOTENOTE: if you can find a better way, lmk cause this is very inefficient
		movieInfo* temp = tempPtrCheckInt[i];
		char* currData = temp->toBeSorted;	
		
		int j = 0;
		while(currData[j] != '\0') {
			char c = currData[j];
			// current char could be int or decimal point
			// if it is not a digit and also not a decimal
			// then we know its not a numeric category
			if(isdigit(c) == 0 &&  c != '.' && c != ' ') {
				isInt = 0;
				break;			
			}
			j++;
		}
		
		//break if we found alphabetic char, no need to check anymore
		if(isInt == 0) {
			break;
		}

		//tempPtrCheckInt++;
		i++;
	}
	
	
	return isInt;
}

//function to parse through csv file
void parseCSV(char* filename, char* columnToSort, char* destDirectory) {
	int pathLen = strlen(filename);
	
	// return 0 if no .csv file extension
	if(!(filename[pathLen-1] == 'v' && filename[pathLen-2] == 's' && filename[pathLen-3] == 'c' && filename[pathLen-4] == '.')){
		return;
	} 
	
	int numCommasB4Sort = 0;		//The number of commas before the column to be sorted is reached.
	
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = malloc(sizeof(char)*500);			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;		//For use in the below do-while loop
	
	int csv = open(filename,O_RDONLY);
	
	
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names
	
	do{
		read(csv, &charIn, 1);
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
		//printf("%c", charIn);
	}while(charIn != '\n');
	
	columnNames[columnNamesIndex] = '\0';
	columnNames = realloc(columnNames, columnNamesIndex+1);
	
	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	
	char* locOfColumn = strstr(columnNames, columnToSort);

	//Searching for number of commas before column to be sorted
	//(Assumes that column names don't have commas in them, which they shouldn't for this assignment.
	
	int i;
	for(i = 0; i <= (locOfColumn - columnNames); i++){

		if(columnNames[i] == ','){
			numCommasB4Sort++;		
		}
	}

	//Reading through the rest of STDIN for data:
	int sizeOfArray = 0;
	movieInfo** dataRows = NULL; //Array of pointers to each instance of movieInfo

	int quoteMode = 0; 	//Variable (bool) to detect whether a quotation is opened and to ignore contents.	

	int eofDetect = 1;	//Checking return value of read() for eof. At eof, read() returns 0.
	rowIterator: while(eofDetect > 0){
		
		movieInfo* rowPtr = malloc(sizeof(movieInfo));
		movieInfo row;
		int parsedCommas = 0;		
		
		//The following 3 variables will be used to determine size of the strings to be put into the movieInfo struct.
		int sizeOfPreSortColumn = 0;
		int sizeOfSortColumn = 0;
		int sizeOfPostSortColumn = 0;
		int sortHasQuotes = 0;
		
		
		//Because allocating a large chunk of memory then shrinking it is probably more efficient than
		//constantly realloc-ing the memory, I'm declaring an initial buffer size for the strings here
		char* preSortColumn = malloc(sizeof(char)*500);
		char* sortColumn = malloc(sizeof(char)*200);
		char* postSortColumn = malloc(sizeof(char)*500);
		
		//Variables indicating size of malloc'd space for reallocing when buffer is full
		int preArraySize = 500;
		int sortArraySize = 200;
		int postArraySize = 500;

		int firstCommaOfSort = 0; 		//Boolean to detect if the first char parsed into sortColumn is a comma.
		
		while(1){
			
			eofDetect = read(csv, &charIn, 1);
			
			if(eofDetect < 1){
				break;
			}
			
			switch(charIn){
				
				case '"':
					quoteMode = !quoteMode;	//Switches quoteMode on or off
					if(parsedCommas == numCommasB4Sort){
						sortHasQuotes = 1;
						break;
					}
					goto fromQuoteMode;		//Gonna do a bad thing here with goto for the sake of not breaking things.
				
				case '\n':
					//Shrinking char buffer sizes, making them strings, and assigning to movieInfo

					sortColumn = realloc(sortColumn, sizeOfSortColumn+1);
					sortColumn[sizeOfSortColumn] = '\0';
					row.toBeSorted = sortColumn;
					row.sizeOfSort = sizeOfSortColumn;
					

					preSortColumn = realloc(preSortColumn, sizeOfPreSortColumn+1);
					preSortColumn[sizeOfPreSortColumn] = '\0';
					row.beforeSortedCol = preSortColumn;
					row.sizeBefore = sizeOfPreSortColumn;
					


					postSortColumn = realloc(postSortColumn, sizeOfPostSortColumn+1);
					postSortColumn[sizeOfPostSortColumn] = '\0';
					row.afterSortedCol = postSortColumn;
					row.sizeAfter = sizeOfPostSortColumn;
						
					row.sortHasQuotes = sortHasQuotes;		//To be used for printing later.
					sizeOfArray++;
 					dataRows = realloc(dataRows, sizeof(movieInfo*)*sizeOfArray);
					memcpy(rowPtr, &row, sizeof(movieInfo));
					dataRows[sizeOfArray-1] = rowPtr;
					goto rowIterator;
				
				case ',':
					if(!quoteMode){			//If not in quote mode, increment parsed commas
						parsedCommas++;	
					}
					//No break on comma detection so it goes into pre/post col strings.
				
				default:
				fromQuoteMode:	if(parsedCommas < numCommasB4Sort){
						sizeOfPreSortColumn++;
						if(sizeOfPreSortColumn > preArraySize){
								preSortColumn = realloc(preSortColumn, sizeof(char)*preArraySize*2);
								preArraySize = preArraySize*2;					
						}
						preSortColumn[sizeOfPreSortColumn-1] = charIn;
					}
					else if(parsedCommas > numCommasB4Sort){
						sizeOfPostSortColumn++;
						if(sizeOfPostSortColumn > postArraySize){
								postSortColumn = realloc(postSortColumn, sizeof(char)*postArraySize*2);
								postArraySize = postArraySize*2;					
						}
						postSortColumn[sizeOfPostSortColumn-1] = charIn;
					} 
					else{	//Implies that parsedCommas == numCommasB4Sort
						if(charIn == ',' && firstCommaOfSort == 0){
							firstCommaOfSort = 1; // To prevent any inner commas like in movie titles from interfering
							sizeOfPreSortColumn++;
							if(sizeOfPostSortColumn > postArraySize){
								postSortColumn = realloc(postSortColumn, sizeof(char)*postArraySize*2);
								postArraySize = postArraySize*2;					
							}
							preSortColumn[sizeOfPreSortColumn -1] = charIn;
						}
						else{
							sizeOfSortColumn++;
							if(sizeOfSortColumn > sortArraySize){
								sortColumn = realloc(sortColumn, sizeof(char)*sortArraySize*2);
								sortArraySize = sortArraySize*2;
							}
							sortColumn[sizeOfSortColumn-1] = charIn;
						}
					}
			}	
		}
	} 
	
	int isNumeric = isInt(dataRows, sizeOfArray);
	mergesort(dataRows, 0, sizeOfArray - 1, isNumeric);
	close(csv);	
	
	//truncate file extension off filename
	if(filename[pathLen-1] == 'v' && filename[pathLen-2] == 's' && filename[pathLen-3] == 'c' && filename[pathLen-4] == '.'){
		filename[pathLen - 1] = '\0';
		filename[pathLen - 2] = '\0';
		filename[pathLen - 3] = '\0';
		filename[pathLen - 4] = '\0';
	}
	

	int isAbsolutePath = 0;
	if(destDirectory != NULL) {
		if(*(destDirectory) == '/') {
			isAbsolutePath = 0;
		}
	}	
	//filename has a ./ in front of it so we want to remove that

	if(*(filename) == '.') { 
		filename = filename + 1;
	}	
	if(*(filename) == '/') {
		filename = filename + 1;
	}
	char* realFileName = filename;
	while(strstr(realFileName, "/") != NULL){
		realFileName = strstr(realFileName, "/")+1;
	}
	char* fileToWrite = (char*) malloc(sizeof(char) * 256);
	
	if(destDirectory != NULL) {
		if(isAbsolutePath == 1) {
			 snprintf(fileToWrite, 256, "%s/%s-sorted-%s.csv\0",destDirectory,realFileName,columnToSort);
		} else {
			snprintf(fileToWrite, 256, "./%s/%s-sorted-%s.csv\0",destDirectory, realFileName, columnToSort);
		}
	} else {
		snprintf(fileToWrite, 256, "%s-sorted-%s.csv\0",filename,columnToSort);
	}
	
	csvwrite(dataRows,sizeOfArray, columnNames, fileToWrite);
	
	free(fileToWrite);
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

/** function to check if the csv is valid. will return 1 if so, 0 if not valid. 
** Checks the file extension, if the column to be sorted exists in file
** and also check number of commas in each line. 
*/
int isValidCSV(char* filename, char* columnToSort) {
	int pathLen = strlen(filename);
	
	// return 0 if no .csv file extension
	if(!(filename[pathLen-1] == 'v' && filename[pathLen-2] == 's' && filename[pathLen-3] == 'c' && filename[pathLen-4] == '.')){
		return 0;
	} 
	
	//Checking if the CSV file already has a sorted version.
	if(strstr(filename, "-sorted-") != NULL){
		return 0;
	}
	
 	int numCommas = 0;		//for later	

	int p_csv;		//file descriptor for file to be opened.
	p_csv = open(filename, O_RDONLY);
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = (char*)malloc(sizeof(char)*500);		//Buffer where we're going to put the first line containing all titles
	
	int columnNamesIndex = 0;		//For use in the below do-while loop
	
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names
	
	do{
		read(p_csv, &charIn, 1);
		if(charIn == ','){
			numCommas++;
		}
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
	}while(charIn != '\n');

	// realloc to save space
	columnNames[columnNamesIndex] = '\0';
	columnNames = realloc(columnNames, columnNamesIndex+1);
        	
	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	char* locOfColumn = strstr(columnNames, columnToSort);
	
	// printf("COLUMN CHECKING:\n COLUMN LINE: %s \nCOLUMN SORTING: %s\n RESULT:%s \n",columnNames, columnToSort, locOfColumn);
	// theres is an error here the reaso for which i am currently looking up but it doesn't sort because of this always returns 0
	// printf("\n EQUALITY CHECK:%s %c%c%c %d \n", filename, *locOfColumn, *(locOfColumn+1), *(locOfColumn + 2), locOfColumn == NULL);
	
	if(locOfColumn == NULL ){
		write(STDERR, "Error while checking validity: The column to be sorted that was input as the 2nd parameter is not contained within the CSV.\n", 124);
		return 0;
	}
	if(hasHeaders(columnNames) == 0){
		write(STDERR, "Error while checking validity: The CSV contained an unknown column header.\n", 75);
		return 0;
	}

	free(columnNames);	
	close(p_csv);
	
	
	int csv = open(filename, O_RDONLY);
	
	// count number of commas in current line and number of commas in 
	int noCommas = 0;
	int prevNoCommas = 0;
	
	char currentChar = '\0';//establishing a default value. If the first char is a null terminator, we have bigger problems
	
	//checking previous char, will help us keep track of if the previous char is a new line if current is new line
	char previousChar = '\0';
	int isInQuotes = 0;

	//double new lines will keep track of if two new lines occur in a row
	int doubleNewLines = 0;
	
	//Rewriting the checker for malformed CSVs because it's glitching and i can't follow it:
	int eofDetect = 1;
	int firstLineParsed = 0;
	
	//waiter waits and sees for when we come across a malformed csv.
	//if there are two new lines and such then it will wait and see the next char before throwing error
	int waiter = 0;
	while(eofDetect > 0){
		eofDetect = read(csv, &currentChar, 1);
		//if double new lines just occured and we are not at the end of the file.
		if(doubleNewLines > 0 && eofDetect > 0) {
			write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
			return 0;
		}
		
		// if we have a disparate amount of commas in each line, we will check if we have
		// the weird two lines error by checking if it is the end of the file
		if(waiter == 1 && eofDetect > 0) {
			write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
			return 0;
		}

		waiter = 0;
		doubleNewLines = 0;
		
		if(previousChar == '\n' && currentChar == '\n') {
			doubleNewLines += 1;
		}
		
		switch(currentChar){
			case '"':
				isInQuotes = !isInQuotes;
				break;
			case ',':
				if(isInQuotes == 1){
					break;
				} else{
					noCommas++;
					break;
				}
			case '\n':
				if(noCommas != numCommas && doubleNewLines == 0){
					write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
					return 0;
				}
				if(noCommas != numCommas && doubleNewLines != 0) {
					waiter = 1;
				}
				if(doubleNewLines == 0 ) {
					firstLineParsed++;
				}
				noCommas = 0;
				//firstLineParsed = 1;
				break;
			default:
				break;
		}
		
		previousChar = currentChar;
	}

	if(firstLineParsed == 0 || firstLineParsed == 1){
		write(STDERR, "Error while checking validity: Malformed CSV\n", 45);
		return 0;
	}	
	
	close(csv);
	return 1;
}

// will write output to csv file
void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename){
	//printf("writing to file %s \n",filename);
	FILE *csvFile; 
	csvFile = fopen(filename, "w+");
	
       	fprintf(csvFile, categories);
        int i = 0;
        while(i < size){
                movieInfo* A = movieArr[i];
                fprintf(csvFile, A->beforeSortedCol);
		if(A->sortHasQuotes == 1){
			fprintf(csvFile,"\"");
			fprintf(csvFile, A->toBeSorted);
			fprintf(csvFile, "\"");
		} else{
			fprintf(csvFile, A->toBeSorted);
		}
		fprintf(csvFile, A->afterSortedCol);	
		fprintf(csvFile, "\n");
		i++;
        }
	fclose(csvFile);
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
	
	if(currDir == NULL) {
		return -2;
	}
	//dirStruct = 
	//printf("\nPIDS of all child processes: ");
	int noProcesses = 1;
	int totalProcesses = 1;
	printf("PIDS of all child processes: ");
	fflush(stdout);

	char * file;		//used to determine i-node type: directory, file, or some other thing
	while(31337) {
		if((dirStruct = readdir(currDir)) == NULL) {
			if(getpid() == pid) {
				break;
			} else {
				int subdirProcs = 1;
				int subdirExitStatus = 0;
				printf(", %d ", getpid());
				while(wait(&subdirExitStatus) > 0){
					subdirProcs += WEXITSTATUS(subdirExitStatus);
				}
				exit(subdirProcs);
			}
		}
		
		//char sortedEnd[] = {'-','s','o','r','t','e','d','-','\0'};				<---- We still need to fork on every file.
		//strcat(sortedEnd, columnToSort);
		//isValidCSV(file, columnToSort);		//debug temp
		file = dirStruct -> d_name;
		int fileMode = (int)(dirStruct -> d_type);
		if(!strcmp(file, ".git") || strcmp(file, ".") == 0 || strcmp(file, "..") == 0) {
			continue;
		}

		int status = 0;
 		if(fileMode == 8){
			int fileFork = fork();
			if(fileFork == 0){
				printf(", %d ",getpid());
				
				char* filepath = (char*)malloc(sizeof(char)*10000);
				strcpy(filepath, dirToSearch);		//Because i need to create a new string for full file path.
				strcat(filepath, file);	//Concatting filename to file path for full file path 
				int validity = 1;
				validity = isValidCSV(filepath, columnToSort);

				//printf("\nisValidCSV: %s %d \n", filepath, validity);

				if(validity){
					parseCSV(filepath, columnToSort, dirDest);
				}
				
				free(filepath);
				exit(1);
			} else{
				continue;
			}	
		} else if(fileMode == 4){
			if(strcmp(file, ".git") == 0 || strcmp(file, ".") == 0 || strcmp(file, "..") == 0) {
				continue;
			}
			int dirFork = fork();
			if(dirFork == 0){
				dirToSearch = realloc(dirToSearch, sizeof(char)*(strlen(dirToSearch)+strlen(file)+2));
				strcat(dirToSearch, file);		//Appending new directory to current directory path;
				strcat(dirToSearch, "/");		//Forcing the current directory path to always end in / for reasons.
				currDir = opendir(dirToSearch);
				continue;
			}
			else{
				continue;
			}
		} else{
			//If for some reason there's a thing that's not a file or directory. Gotta handle all the errors dawg.	
			continue;
		}
	}
	int rootDirExitStatus;		//For the root directory; helps to sum all the process numbers 
	int totalProcs = 0;			//Francisco said don't count main process
	while(wait(&rootDirExitStatus) > 0){
		//Waiting for all the child processes to return.
		totalProcs += WEXITSTATUS(rootDirExitStatus);
	}
	printf("\n Total number of Processes: %d\n", totalProcs);
	
	free(dirDest);
	free(columnToSort);
	free(dirToSearch);
	return 0;
}

