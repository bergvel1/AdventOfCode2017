#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int one (char * filename){
	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL)
		return -1; // couldn't open file

	char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int acc = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);

    	char * curr_entry_str = strtok(line," \t");
    	int currLargest = atoi(curr_entry_str);
    	int currSmallest = atoi(curr_entry_str);

    	while (curr_entry_str != NULL){
		    //printf ("%s\n",curr_entry_str);
    		int currEntry = atoi(curr_entry_str);

    		if (currEntry > currLargest)
    			currLargest = currEntry;

    		if (currEntry < currSmallest)
    			currSmallest = currEntry;

		    curr_entry_str = strtok (NULL, " \t");
		}
		acc += currLargest - currSmallest;
    }

	fclose(file);
	if (line)
        free(line);

    printf("%d\n", acc);

	return 0;
}

int two (char * filename){
	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL)
		return -1; // couldn't open file

	char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int acc = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);

    	char * curr_entry_str = strtok(line," \t");
    	int * numbers = malloc(20*sizeof(int));
    	int curr_idx = -1;

    	// construct array of ints
    	while (curr_entry_str != NULL){
    		curr_idx++;
    		int currEntry = atoi(curr_entry_str);
    		numbers[curr_idx] = currEntry;
    		//printf ("%d\n",currEntry);

		    curr_entry_str = strtok (NULL, " \t");
		}

		// search for divisible pairs
		for(int i = 0; i <= curr_idx; i++){
			for(int j = 0; j <= curr_idx; j++){
				if ((i != j) && ((numbers[i] % numbers[j]) == 0)){
					acc += numbers[i] / numbers[j];
					//printf ("%d\n",acc);
				}
			}
		}

		free(numbers);
    }

	fclose(file);
	if (line)
        free(line);

    printf("%d\n", acc);

	return 0;
}

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given

	char* filename = argv[1];
	
	return two(filename);
}