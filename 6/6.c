#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

char* serialize(int* nums, int count){
	char * ret = malloc(3*count); // remember to free this!
	int offset = 0; //number of bytes already written

	for(int i = 0; i < count; i++){
		offset += sprintf(ret+offset,"%d ",nums[i]);
	}

	return ret;
}

int find_max_idx(int* nums, int count){
	int max_idx = 0;

	for(int i = 0; i < count; i++){
		if(nums[i] > nums[max_idx])
			max_idx = i;
	}

	//printf("max at %d\n",max_idx);
	return max_idx;
}

void redistribute(int* nums, int count, int idx, int remaining){
	if (remaining == 0)
		return;

	nums[idx]++;
	int next_idx = (idx+1)%count;

	redistribute(nums,count,next_idx,remaining-1);
}

void print_nums(int* nums, int count){
	for(int i = 0; i < count; i++){
		printf("%d ",nums[i]);
	}
	printf("\n");
}

int one (int* nums, int count){
	map_t seen_configs = hashmap_new();
	int error;
	int ret = -1;

	for (int i = 0; i < 100000; i++){
		char * config_str = serialize(nums,count);
		int * last_seen = malloc(sizeof(int));
		*last_seen = i;
		//printf("bOOPS\n");
		error = hashmap_get(seen_configs,config_str,(void**)&last_seen);
		//printf("dOOPS\n");
		if(error == MAP_OK){
			ret = i-(*last_seen); // found duplicate prior configuration
			break;
		}
		else{
			free(last_seen);
			last_seen = malloc(sizeof(int));
			*last_seen = i;
			error = hashmap_put(seen_configs,config_str,(void*)last_seen);
			if(error != MAP_OK){
				ret = -1; // something went wrong!
				printf("OOPS\n");
				break;
			}
		}

		int max_idx = find_max_idx(nums,count);
		int rem = nums[max_idx];
		nums[max_idx] = 0;
		redistribute(nums,count,(max_idx+1)%count,rem);
		print_nums(nums,count);
	}

	// hashmap_remove??
	hashmap_free(seen_configs);
	return ret;
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
    int lines = 0;

    while(!feof(file)){
  		char ch = fgetc(file);
  		if(ch == '\n'){
			lines++;
		}
	}

	//printf("lines: %d\n",lines);
	int * numbers = malloc(lines*sizeof(int));
	int curr_idx = 0;
	fseek(file, 0, SEEK_SET);

    while ((read = getline(&line, &len, file)) != -1) {
    	numbers[curr_idx] = atoi(line);
    	//printf("%d\n", numbers[curr_idx]);
    	curr_idx++;
    }

    fclose(file);
	if (line)
        free(line);

    int ret = one(numbers,lines);
    printf("\n%d\n", ret);

  	free(numbers);
	return 0;
}