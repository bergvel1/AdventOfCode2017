#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ringhash.h"

#define GRID_SIZE 128
#define HASH_LEN 16

int ** region_grid = NULL;
unsigned int ** hashes = NULL;
int largest_reg = 0;

const char * input = "jxqlasbh-";
//const char * input = "63,144,180,149,1,255,167,84,125,65,188,0,2,254,229,24";

void init(){
	assert(!region_grid);
	assert(!hashes);
	assert(!largest_reg);

	region_grid = calloc(GRID_SIZE,sizeof(int*));

	for(int i; i < GRID_SIZE; i++){
		region_grid[i] = calloc(GRID_SIZE,sizeof(int));
	}

	hashes = calloc(GRID_SIZE,sizeof(unsigned int *));
}

void destroy(){
	assert(region_grid);
	assert(hashes);

	for(int i; i < GRID_SIZE; i++){
		free(region_grid[i]);
	}
	for(int i; i < GRID_SIZE; i++){
		if(hashes[i]) free(hashes[i]);
	}

	free(region_grid);
	free(hashes);
}

int numberOfSetBits(uint32_t i)
{
     // Java: use >>> instead of >>
     // C or C++: use uint32_t
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void calculate_hashes(){
	assert(hashes);

	int input_len = strlen(input);

	for(int i = 0; i < GRID_SIZE; i++){
		char * curr_input = NULL;
		char * end = NULL;
		if(i < 10){
			curr_input = malloc(input_len+2); // 1 digit + terminator
			//end = strcpy(curr_input,input);
		}
		if(i >= 100){
			curr_input = malloc(input_len+4); // 3 digits + terminator
			//end = strcpy(curr_input,input);
		}
		else{
			curr_input = malloc(input_len+3); // 2 digits + terminator
			//end = strcpy(curr_input,input);
		}
		sprintf(curr_input,"%s%d",input,i);
		hashes[i] = ringhash(curr_input);

		
		printf("%s -> ", curr_input);
		for(int j = 0; j < HASH_LEN; j++) printf("%02x",(hashes[i])[j]);
		printf("\n");
		free(curr_input);

		// put -1 in region_grid for each active bit
		unsigned short * itr = (unsigned short*) hashes[i];
		for(int j = 0; j < HASH_LEN; j++){

			for(int b = 7; b >= 0; b--){
				int y = i;
				int x = (j*8)+7-b;
				//printf("(%d,%d) ",x,y);

				unsigned short testbit = (1 << b);
				if(((*itr) & testbit)){
					(region_grid[y])[x] = -1;
				}

			}
			itr += 2;
		}
	}


}

/*
int adjacent_region(int y, int x){
	assert(hashes);
	assert(region_grid);

	//int reg = 0;

	// check above
	if(y > 0){
		int * above = region_grid[y-1];
		if(above[x])  return above[x];
	}
	// check left
	if(x > 0){
		if((region_grid[y])[x-1]) return (region_grid[y])[x-1];
	}

	return 0; //shouldn't need to check below or left
}

void assign_regions_to_chunk(unsigned short chunk, int hash_idx, int chunk_idx){

	for(int i = 7; i >= 0; i--){
		int y = hash_idx;
		int x = (chunk_idx*8)+7-i;
		//printf("(%d,%d) ",x,y);

		unsigned short testbit = (1 << i);
		if(((chunk) & testbit)){
			int adj_r = adjacent_region(y,x);
			if(adj_r){
				(region_grid[y])[x] = adj_r;
			}
			else (region_grid[y])[x] = ++largest_reg;
		}
	}
}
*/

int total_squares_used(){
	assert(hashes);
	assert(region_grid);

	int acc = 0;

	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < HASH_LEN; j++){
			acc += numberOfSetBits(hashes[i][j]);
		}
	}

	return acc;
}

void print_reg_grid(){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			int curr = (region_grid[i])[j];
			if(curr > 9) printf("&");
			else if (curr) printf("%d",curr);
			else printf(".");
		}
		printf("\n");
	}
}

void explore(int y, int x){
	int curr_val = (region_grid[y])[x];

	if(curr_val == -1){
		(region_grid[y])[x] = largest_reg;

		// up
		if(y > 0){
			explore(y-1,x);
		}
		// down
		if(y < (GRID_SIZE-1)){
			explore(y+1,x);
		}
		// left
		if(x > 0){
			explore(y,x-1);
		}
		// right
		if(x < (GRID_SIZE-1)){
			explore(y,x+1);
		}
	}
}

int total_regions(){
	/*
	unsigned short * itr = NULL;

	for(int i = 0; i < GRID_SIZE; i++){
		itr = (unsigned short*) hashes[i];
		for(int j = 0; j < HASH_LEN; j++){
			//acc += numberOfSetBits(hashes[i][j]);
			char * tmp = malloc(3);
			sprintf(tmp,"%02x",*itr);
			//printf("%c%c",tmp[0],tmp[1]);

			printf("%02x",*itr);

			//assign_regions_to_chunk(*itr,i,j);

			free(tmp);
			itr += 2;
		}
		printf("\n");
	}
	*/
	largest_reg = 0;

	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if ((region_grid[i])[j] == -1){
				largest_reg++;
				explore(i,j);
			}
		}
	}


	print_reg_grid();
	return largest_reg;
}

int main(int argc, char* argv[]){
	init();

	calculate_hashes();

	printf("Result: %d\n",total_regions());
	// unsigned int * test = ringhash(input);
	// for(int i = 0; i < 16; i++) printf("%02x",test[i]);
	// printf("\n");
	// free(test);

	destroy();
	return 0;
}