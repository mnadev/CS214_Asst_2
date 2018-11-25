#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
int main(void) {
	int i = 1;
	int k = 1;
	for(i = 1; i = i *2 ; i <= 256) {
		char * dir = malloc(sizeof(char) * 5000);
		snprintf(dir, 5,"dir%d\0", k);
		int j = 1;
		int l = 1;
		for( j = 1; j  = j * 2; j <= i) {
			printf("%s\n", dir);
			if(j != 1) {
				strcat(dir, "/dir");	
			}
			mkdir(dir, 0777);
			l++;
		}
		k++;
	}
	return 1;
}
