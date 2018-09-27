#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "simpleCSVsorter.h"

void csvwrite(movieInfo** movieArr,int size ,char* categories, int sizeOfCategories){
       	write(STDOUT, categories, sizeOfCategories);
        int i = 0;
        while(i < size){
                movieInfo* A = movieArr[i];
                write(STDOUT, A->beforeSortedCol, A->sizeBefore);
		write(STDOUT, A->toBeSorted, A->sizeOfSort);
		write(STDOUT, A->afterSortedCol, A->sizeAfter);	
		write(STDOUT, "\n", 1);
		i++;
        }
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

		//Because allocating a large chunk of memory then shrinking it is probably more efficient than
		//constantly realloc-ing the memory, I'm declaring an initial buffer size for the strings here
		char* preSortColumn = malloc(sizeof(char)*500);
		char* sortColumn = malloc(sizeof(char)*200);
		char* postSortColumn = malloc(sizeof(char)*500);

		int firstCommaOfSort = 0; 		//Boolean to detect if the first char parsed into sortColumn is a comma.
		while(1){
			eofDetect = read(STDIN, &charIn, 1);
			if(eofDetect < 1){
				break;
			}
			switch(charIn){
				case '"':
					quoteMode = !quoteMode;	//Switches quoteMode on or off
					break;
				case '\n':
					//Shrinking char buffer sizes, making them strings, and assigning to movieInfo

					sortColumn = realloc(sortColumn, sizeOfSortColumn+1);
					sortColumn[sizeOfSortColumn] = '\0';
					row.toBeSorted = sortColumn;
					row.sizeOfSort = sizeOfSortColumn+1;
					//printf("%s\n", row.toBeSorted); (Debug code)
					//printf("%d, %d, %d\n", sizeOfPreSortColumn, sizeOfSortColumn, sizeOfPostSortColumn);

					preSortColumn = realloc(preSortColumn, sizeOfPreSortColumn+1);
					preSortColumn[sizeOfPreSortColumn] = '\0';
					row.beforeSortedCol = preSortColumn;
					row.sizeBefore = sizeOfPreSortColumn+1;
					//printf("%s\n", row.beforeSortedCol); (Debug code)


					postSortColumn = realloc(postSortColumn, sizeOfPostSortColumn+1);
					postSortColumn[sizeOfPostSortColumn] = '\0';
					row.afterSortedCol = postSortColumn;
					row.sizeAfter = sizeOfPostSortColumn+1;
						
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
					if(parsedCommas < numCommasB4Sort){
						sizeOfPreSortColumn++;
						preSortColumn[sizeOfPreSortColumn-1] = charIn;
					}
					else if(parsedCommas > numCommasB4Sort){
						sizeOfPostSortColumn++;
						postSortColumn[sizeOfPostSortColumn-1] = charIn;
					} 
					else{	//Implies that parsedCommas == numCommasB4Sort
						if(charIn == ',' && firstCommaOfSort == 0){
							firstCommaOfSort = 1; // To prevent any inner commas like in movie titles from interfering
							sizeOfPreSortColumn++;
							preSortColumn[sizeOfPreSortColumn -1] = charIn;
						}
						else{
							sizeOfSortColumn++;
							sortColumn[sizeOfSortColumn-1] = charIn;
						}
					}
			}	
		}
	} 
	
	// this integer acts as a boolean
	// is 0 if the data is not an int, now is 1 if the data is an int
	int isInt = 1;
	
	//create temp pointer to iterate through all of array and check if each item is a number
	//because it is possible for a movie to be a number, e.g. '300', and that would be a problem
	//so we should check all of them to be sure
	movieInfo** tempPtrCheckInt = dataRows;
	i = 0;
	
	while(i < sizeOfArray) {
		//iterate through each char, checking if it is int
		// NOTENOTENOTE: if you can find a better way, lmk cause this is very inefficient	
		char* currData = (*(tempPtrCheckInt[i])).toBeSorted;	
		
		int j = 0;
		while(currData[j] != '\0') {
			// current char could be int or decimal point
			if(isdigit(currData[j]) != 0 &&  currData[j] == '.' ) {
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

	
	mergesort(dataRows, 0, sizeOfArray - 1, isInt);
	csvwrite(dataRows,sizeOfArray, columnNames, columnNamesIndex);
	return 0;
}

