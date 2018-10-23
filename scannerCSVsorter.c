#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<errno.h>
#include "scannerCSVsorter.h"

void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename);

// function that finds max length of lines in the file, useful when creating buffer. 
int maxLengthLine(char* filename) {
	FILE *csv;
	csv = fopen(filename, "r");
	
	int count = 0;
	int maxCount = 0;
	
	char currentChar;
	while(!feof(stdin)) {
		currentChar = (char)(fgetc(csv));
		if(currentChar == '\n') {
			if(count > maxCount) {
				maxCount = count;
			}
			count = 0;
		} else {
			count++;
		}
	}
	fclose(csv);
	return maxCount;
}

int isInt(movieInfo** dataRows, int sizeOfArray) {
	// this integer acts as a boolean
	// is 0 if the data is not an int, now is 1 if the data is an int
	int isInt = 1;
	
	//create temp pointer to iterate through all of array and check if each item is a number
	//because it is possible for a movie to be a number, e.g. '300', and that would be a problem
	//so we should check all of them to be sure
	movieInfo** tempPtrCheckInt = dataRows;
	int i = 0;



	// THIS IS WHERE THE PROBLEM LIES
	// LOOK INTO YOUR EYES
	// AND YOU WILL FIND
	// SOME PORK RIND
		
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
void parseCSV(char* filename, int maxLength, char* columnToSort, char* destDirectory) {
	int numCommasB4Sort = 0;		//The number of commas before the column to be sorted is reached.
	
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = malloc(sizeof(char)*500);			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;		//For use in the below do-while loop
	
	
	FILE *csv;
	csv = fopen(filename,"r");
	
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names
	do{
		fgets(&charIn, 1, csv);
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
	}while(charIn != '\n');
	columnNames[columnNamesIndex] = '\0';
	columnNames = realloc(columnNames, columnNamesIndex+1);

	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	char* locOfColumn = strstr(columnNames, columnToSort);
	if(locOfColumn == NULL){
		write(STDERR, "Error: The column to be sorted that was input as the 2nd parameter is not contained within the CSV.\n", 100);
		//return -1;
	}
	/**************************************************************************************
	Probably need to move ^^^^ to isValidCSV
	**************************************************************************************/	

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
			fgets(&charIn, 1, csv);
			if(feof(csv)){
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
					//printf("%s\n", row.toBeSorted); (Debug code)
					//printf("%d, %d, %d\n", sizeOfPreSortColumn, sizeOfSortColumn, sizeOfPostSortColumn);

					preSortColumn = realloc(preSortColumn, sizeOfPreSortColumn+1);
					preSortColumn[sizeOfPreSortColumn] = '\0';
					row.beforeSortedCol = preSortColumn;
					row.sizeBefore = sizeOfPreSortColumn;
					//printf("%s\n", row.beforeSortedCol); (Debug code)


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
	char fileToWrite[255];
	fclose(csv);	
	snprintf(fileToWrite, 255, "%s/%s-sorted-%s.csv",destDirectory,filename,columnToSort);
	csvwrite(dataRows,sizeOfArray, columnNames, fileToWrite);
	//^^^^^^^^^ Need to reassign later, just commented out for debugging purposes atm

}

// checks if the csv is valid. will return 1 if so, 0 if not valid. 
int isValidCSV(char* filename, char* columnToSort) {
	FILE *p_csv;
	p_csv = fopen(filename, "r");
	
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = malloc(sizeof(char)*500);			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;		//For use in the below do-while loop
	
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names
	do{
		fgets(&charIn, 1, p_csv);
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
	}while(charIn != '\n');
	columnNames[columnNamesIndex] = '\0';
	columnNames = realloc(columnNames, columnNamesIndex+1);

	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	char* locOfColumn = strstr(columnNames, columnToSort);
	if(locOfColumn == NULL){
		write(STDERR, "Error: The column to be sorted that was input as the 2nd parameter is not contained within the CSV.\n", 100);
		fclose(p_csv);
		return 0;
	}
	
	fclose(p_csv);
	
	FILE *csv;
	csv = fopen(filename, "r");
	
	// count number of commas in current line and number of commas in 
	int noCommas = 0;
	int prevNoCommas = 0;
	
	char currentChar = '\0';	//establishing a default value. If the first char is a null terminator, we have bigger problems
	
	int isInQuotes = 0;
	
	// get number of commas in first lne. this will be the base number of commas that should be in each 
	// line
	while(currentChar != '\n') {
		fgetc(csv);
		if(currentChar == ',' && !isInQuotes) {
			noCommas++;
		}
		
		if(currentChar = '\"') {
			isInQuotes = !isInQuotes;
		}
	}
	
	prevNoCommas = noCommas;
	noCommas = 0;
	
	while(!feof(stdin)) {
		fgetc(csv);
		if(currentChar == '\n') {
			if(noCommas != prevNoCommas) {
				return 0;
			} else {
				prevNoCommas = noCommas;
				prevNoCommas = 0;
			}
		} else {
			if(currentChar == ',' && !isInQuotes) {
				noCommas++;
			}
		
			if(currentChar = '\"') {
				isInQuotes = !isInQuotes;
			}
		}
	}
	fclose(csv);
		
	return 1;
}

// will write output to csv file
void csvwrite(movieInfo** movieArr, int size ,char* categories, char* filename){
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
	char* dirToSearch = ".";	//dirToSearch defaults to current directory if not changed by flag later.
	char* dirDest = NULL;

	//Write to STDERR if there are fewer than the required number of args
	if(argc < 3){
		write(STDERR, "Insufficient arguments.\n", 25);
		return -1;
	}
	//Flag Handling:
	int flag;
	while((flag = getopt(argc, argv, "c:d:o:")) != -1){
		switch(flag){		
			case 'c':
				if(flagsPresent[0] == 1){
					write(STDERR, "Error: Repeated argument -c.\n", 30); 
					return -1;
				} else{
					flagsPresent[0] = 1;				//The -c parameter is present
					//columnToSort = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
					//strcpy(columnToSort, optarg);		//copying the column name from optarg to columnToSort
					break;
				}
			case 'd':
				if(flagsPresent[1] == 1){
					write(STDERR, "Error: Repeated argument -d.\n", 30); 
					return -1;
				} else{
					flagsPresent[1] = 1;
					dirToSearch = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
					strcpy(dirToSearch, optarg);
					break;
				}
			case 'o':
				if(flagsPresent[2] == 1){
					write(STDERR, "Error: Repeated argument -o.\n", 30); 
					return -1;
				} else{
					flagsPresent[2] = 1;
					dirDest = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
					strcpy(dirDest, optarg);
					break;
				}				
			case '?':
				write(STDERR, "Error: Unknown arguments.\n", 27);
				return -1;
		}

	}
	//Write to STDERR if -c flag is not present
	if(flagsPresent[0] != 1){
		write(STDERR, "Error: The first argument of the program must be '-c to sort by column.\n", 75);
		return -1;
	}

	printf("Initial PID: ");
	int pid = getpid();
	printf("%d\n",pid);
	
	// check for directory to search
	// using command line argument if inputted
	if(argc > 3){
		if(strcmp("-d",argv[3]) != 0){
			//set it to what they give
			if(argc > 4) {
				dirToSearch = argv[4];
			}
		}
	}
	
	DIR *currDir;
	currDir = opendir(dirToSearch);
	if(errno == ENOENT){
		dirToSearch = ".";
		currDir = opendir(dirToSearch);	
	}
	struct dirent* dirStruct;
	
	if(currDir == NULL) {
		return -2;
	}
	//dirStruct = 
	printf("PIDS of all child processes: ");
	int noProcesses = 1;
	while(1337) {
		if((dirStruct = readdir(currDir)) == NULL){
			if(getpid() != pid){			
				exit(noProcesses+1);
			} else{
				break;
			}
		}
		// skipping first two file because its current and parent dirs
		int statusLoc = 0;
		char * currFile = dirStruct -> d_name;
		if(strcmp(currFile,".") == 0 || strcmp(currFile, "..") == 0) {
			continue;	
		}
		int cpid = fork();
		if(cpid == 0) {
			int fileType = dirStruct -> d_type;		//fileType = 4 for directory, 8 for file
		
			if(fileType == 8) {
				// to do create data rows array from csv file
				char* sortedFileEnding = strcat("-sorted-", argv[2]);
				// cheack to make sure we are sorting a csv file and we are not
				//sorting an already sorted file.
				if(strstr(currFile,".csv") != NULL && strstr(currFile, sortedFileEnding) == NULL){
					if(isValidCSV(currFile, argv[2])) {
						parseCSV(currFile, maxLengthLine(currFile), argv[2], dirDest);
					}
				}
				exit(1);
			} else {	//Implies that next "file" is actually a directory, so we fork() to process directory.
				int anotherPID = fork();	//variable name pending?
				if(anotherPID == 0) {
					//More directory stuff	(placeholder comment while i think some things through)
					currDir = opendir(currFile);
					continue;		
				} else{
					// add noProcesses to wait to count noProcesses
					// int waitRet = wait();
					// noProcesses = noProcesses + waitRet;
					wait(&statusLoc);		//placeholder wait
					noProcesses += statusLoc;	//Adds by the number of processes spawned from child
					continue;
				}
			 	//Might need to add more things here later to account for writing all the child PIDs to STDOUT.
			}
		} else{
			wait(&statusLoc); //placeholder wait
			noProcesses = noProcesses + statusLoc;
			fflush(stdout);
			if(noProcesses <= 2){
				printf(",%d", pid);
			} else {
				printf("%d", pid);
			}
		}
	}
	
	printf("\nTotal Number of Processes: %d\n", noProcesses);
	
	//closedir(dir);
	
	
	return 0;
}


