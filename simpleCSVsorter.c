#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "simpleCSVsorter.h"

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
	
	char charIn = '\0';
	char columnNames[500];			//Buffer where we're going to put the first line containing all titles
	int columnNamesIndex = 0;	
	
	//Reading in from STDIN char by char until a '\n' is reached to get a string containing all column names
	do{
		read(STDIN, &charIn, 1);
		columnNames[columnNamesIndex] = charIn;
		columnNamesIndex++;
	}while(charIn != '\n');
	columnNames[columnNamesIndex] = '\0';

	//Determining if the column to be sorted parameter is in the list of columns using strstr()
	char* locOfColumn = strstr(columnNames, argv[2]);
	if(locOfColumn == NULL){
		write(STDERR, "Error: The column to be sorted that was input as the 2nd parameter is not contained within the CSV.\n", 100);
		return -1;
	}
	

	return 0;
}
