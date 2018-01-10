#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanup(char ** words, int num_words){
	for(int i = 0; i < num_words; i++){
		free(words[i]);
	}
	free(words);
}


int is_valid_one(char * phrase){
	char * curr_entry_str = strtok(phrase," \t\n");
	int num_words = 15;
	int max_word_size = 15;
	int curr_idx = 0;

	// allocate memory for words
	char ** words = malloc(num_words*sizeof(char*));
	for(int i = 0; i < num_words; i++){
		words[i] = calloc(max_word_size,1);
	}

	while (curr_entry_str != NULL){
		strcpy(words[curr_idx],curr_entry_str);


		//printf("%s\n",words[curr_idx]);
		curr_entry_str = strtok (NULL, " \t\n");
		curr_idx++;
	}

	// search for duplicate words
	for(int i = 0; i < curr_idx; i++){
		for(int j = i+1; j < curr_idx; j++){
			//printf("strcmp %s %s.\n",words[i],words[j]);
			if (strcmp(words[i],words[j]) == 0){
				// duplicate found

				cleanup(words,num_words);
				return 0;
			}
		}
	}

	cleanup(words,num_words);
	return 1; // valid phrase
}

int compare(const void * a, const void * b){
	return *(char*)b - *(char*)a;
}

int is_valid_two(char * phrase){
	char * curr_entry_str = strtok(phrase," \t\n");
	int num_words = 15;
	int max_word_size = 15;
	int curr_idx = 0;

	// allocate memory for words
	char ** words = malloc(num_words*sizeof(char*));
	for(int i = 0; i < num_words; i++){
		words[i] = calloc(max_word_size,1);
	}

	while (curr_entry_str != NULL){
		strcpy(words[curr_idx],curr_entry_str);

		// sort into alphabetical order
		qsort(words[curr_idx],14,1,compare);

		//printf("%s ",words[curr_idx]);
		curr_entry_str = strtok (NULL, " \t\n");
		curr_idx++;
	}

	// search for duplicate words
	for(int i = 0; i < curr_idx; i++){
		for(int j = i+1; j < curr_idx; j++){
			//printf("strcmp %s %s.\n",words[i],words[j]);
			if (strcmp(words[i],words[j]) == 0){
				// duplicate found

				cleanup(words,num_words);
				return 0;
			}
		}
	}

	cleanup(words,num_words);
	return 1; // valid phrase
}

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given
	
	FILE *file;
	file = fopen(argv[1], "r");
	if (file == NULL)
		return -1; // couldn't open file

	char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int acc = 0;

    while ((read = getline(&line, &len, file)) != -1) {
    	//printf("%s", line);
    	acc += is_valid_two(line);
    }

    fclose(file);
	if (line)
        free(line);

    printf("%d\n", acc);

	return 0;
}