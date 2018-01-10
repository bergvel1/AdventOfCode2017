#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int one (int* nums, int idx, int count, int acc){
	int old_num = nums[idx];
	int new_num = old_num+1;
	//printf("old_num: %d\n",old_num);
	//printf("new_num: %d\n",new_num);
	nums[idx] = new_num;

	int next_idx = idx + old_num;
	//printf("At %d going to %d on jump %d\n",idx,next_idx,acc);
	if((next_idx < 0) || (next_idx >= count))
		return acc;
	
	return one(nums,next_idx,count,acc+1);
}

int two (int* nums, int idx, int count, int acc){
	int old_num = nums[idx];
	int new_num = old_num;

	if (old_num >= 3)
		new_num--;
	else new_num++;
	//printf("old_num: %d\n",old_num);
	//printf("new_num: %d\n",new_num);
	nums[idx] = new_num;

	int next_idx = idx + old_num;
	//printf("At %d going to %d on jump %d\n",idx,next_idx,acc);
	if((next_idx < 0) || (next_idx >= count))
		return acc;
	
	return two(nums,next_idx,count,acc+1);
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

    int ret = two(numbers,0,lines,1);
    printf("\n%d\n", ret);

  	free(numbers);
	return 0;
}