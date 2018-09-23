//Add some includes

char** mergesort(char** arr, int size) {
	//base case of merge sort
        if(size == 1) {
		return arr;
        }
  	//split array into two and mergesort
      	// i saved return value into array because we 
        // can then merge them easier
        char** firstHalf = mergesort(char** arr, size/2);
        char** secondHalf = mergesort(char** (arr + (size/2) **sizeof(char)),size/2);
	return merge(firstHalf, secondHalf, sizeA, sizeB);
}

char* merge(void* A, void* B, int sizeA, int sizeB){
        //index pointers
        int i = 0, j = 0;
       
        // allocate space for the return array
        movieInfo* retArr = malloc((sizeA + sizeB)*sizeof(movieInfo));
        
        int sizeStruct = sizeof(movieInfo);
        
        while(i < sizeA && j < sizeB) {
        	if(*(A + i*sizeStruct) > *(B + j*sizeStruct)) {
        		*retArr = *(B + j*sizeStruct);
                        j++;
       			retArr++;
       		} else if(*(A + i*sizeStruct) < *(B + j*sizeStruct)) {
       			*retArr = *(A + i*sizeStruct);
        		i++;
        		retArr++;
                } else {
       			*retArr = *(B + j*sizeStruct);
       			j++;
        		retArr++;
 			*retArr = *(A + i*sizeStruct);
			i++;
        		retArr++;
       	   	}           
	}

}                                                                                                                                                      

