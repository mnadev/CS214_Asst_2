#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include "scannerCSVsorter.h"


// function that finds max length of lines in the file, useful when creating buffer. 
int maxLengthLine(char* filename) {
	FILE *csv;
	csv = fopen(filename);
	
	int count = 0;
	int maxCount = 0;
	
	char * currentChar[1];
	while(!feof(stdin)) {
		fgets(currentChar, 1, csv);
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

int isInt(movieInfo** dataRows) {
	// this integer acts as a boolean
	// is 0 if the data is not an int, now is 1 if the data is an int
	int isInt = 1;
	
	//create temp pointer to iterate through all of array and check if each item is a number
	//because it is possible for a movie to be a number, e.g. '300', and that would be a problem
	//so we should check all of them to be sure
	movieInfo** tempPtrCheckInt = dataRows;
	i = 0;



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
movieInfo** parseCSV(char* filename, int maxLength) {
	
}

// checks if the csv is valid. will return 1 if so, 0 if not valid. 
int isValidCSV(char* filename) {
	FILE *csv;
	csv = fopen(filename);
	
	// count number of commas in current line and number of commas in 
	int noCommas = 0;
	int prevNoCommas = 0;
	
	char * currentChar[1];
	
	int isInQuotes = 0;
	
	// get number of commas in first lne. this will be the base number of commas that should be in each 
	// line
	while(currentChar != '\n') {
		fgets(currentChar, 1, csv);
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
		fgets(currentChar, 1, csv);
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
void csvwrite(movieInfo** movieArr, int size ,char* categories, int sizeOfCategories, char* filename){
	FILE *csvFile; 
	csvFile = fopen(filename, "w+");
	
       	fprintf(csvFile, categories);
        int i = 0;
        while(i < size){
                movieInfo* A = movieArr[i];
                write(STDOUT, A->beforeSortedCol, A->sizeBefore);
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
	//Write to STDERR if there are fewer than the required number of args
	if(argc < 3){
		write(STDERR, "Insufficient arguments.\n", 25);
		return -1;
	}
	//Write to STDERR if the 1st program argument is not -c
	else if(strcmp("-c", argv[1]) != 0){
		write(STDERR, "Error: The first argument of the program must be '-c' to sort by column.\n", 75);
		return -1;
	}
	
	
	// check for directory to search
	// using command line argument if inputted
	char* dirToSearch;
	if(strcmp("-d",argv[3]) != 0){
		//set it to what they give
		*dirToSearch = argv[4];
	} else {
		// set dir to curr directory
		
		dirToSearch = ".";
	}
	
	DIR *currDir;
	currDir = opendir(dirToSearch);
	struct dirent* dirStruct;
	
	if(currDir == NULL) {
		return -2;
	}
	//dirStruct = 
	while((currDir = readdir(currDir)) != NULL) {
		
		// skipping first two file because its current and parent dirs
		
		char * currFile = currDir -> d_name;
		if(strcmp(currFile,".") || strcmp(currFile, "..")) {
			continue;	
		}
		int pid = fork()
		if(pid == 0) {
			openDir(currFile);
		
			if(errno == ENOTDIR) {
				int pid = fork();
				if(pid == 0) {
					// to do create data rows array from csv file
				if(strstr(currFile,".csv"))
					movieInfo** dataRows = parseCSV(currFile);
					mergesort(dataRows, 0, sizeOfArray - 1, isInt(dataRows));
					csvwrite(dataRows,sizeOfArray, columnNames, columnNamesIndex, dirDest);
				}
			} else {
				// recursively run program on this directory.
			 	exec(./scannerCSVsorter, "-o", argv[2],argv[3],argv[4],argv[5],argv[6]);
			}
		}
	} 
	
	closedir(dir);
	// check for directory to write to
	// using command line argument if inputted
	char* dirDest;
	if(strcmp("-o",argv[5]) != 0){
		//set it to what they give
		*dirDest = argv[6];
	} else {
		// set dir to curr directory
		dirDest = getcwd();
	}
	
	int numCommasB4Sort = 0;		//The number of commas before the column to be sorted is reached.
	
	char charIn = '\0';				//Buffer to put each char that's being read in from STDIN
	char* columnNames = malloc(sizeof(char)*500);			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;		//For use in the below do-while loop
	
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names
	do{
		read(STDIN, &charIn, 1);
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
	}while(charIn != '\n');
	columnNames[columnNamesIndex] = '\0';
	columnNames = realloc(columnNames, columnNamesIndex+1);

	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	char* locOfColumn = strstr(columnNames, argv[2]);
	if(locOfColumn == NULL){
		write(STDERR, "Error: The column to be sorted that was input as the 2nd parameter is not contained within the CSV.\n", 100);
		return -1;
	}
	
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
			eofDetect = read(STDIN, &charIn, 1);
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
	
	int isNumeric = isInt(dataRows);
	
	mergesort(dataRows, 0, sizeOfArray - 1, isInt(dataRows));
	csvwrite(dataRows,sizeOfArray, columnNames, columnNamesIndex, dirDest);
	return 0;
}


