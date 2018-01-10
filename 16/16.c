#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PGM_COUNT 16
#define CYCLE_SIZE 42

uint64_t total_dances = 1000000000;

typedef struct node_s{
	char name;
	struct node_s * next;
} node_t;

node_t * line = NULL;
size_t line_idx = 0;
node_t ** node_arr = NULL;
char ** seen_seqs = NULL;
size_t cycle_size = 42;

void init_line(){
	assert(!line);
	assert(!node_arr);

	seen_seqs = calloc(CYCLE_SIZE,sizeof(char*));
	for(int i = 0; i < CYCLE_SIZE; i++){
		seen_seqs[i] = calloc(PGM_COUNT+1,1);
	}

	char curr = 'a';
	line = malloc(sizeof(node_t));
	node_arr = malloc(sizeof(node_t*) * PGM_COUNT);
	line->name = curr;
	line->next = NULL;
	node_arr[0] = line;

	node_t * curr_node = line;

	for(int i = 1; i < PGM_COUNT; i++){
		curr_node->next = malloc(sizeof(node_t));
		curr_node = curr_node->next;
		curr_node->name = (char) (++curr);
		curr_node->next = NULL;
		node_arr[i] = curr_node;
	}
	curr_node->next = line; // cycle to help with "spin" move
}

void destroy_line(){
	assert(line);
	assert(node_arr);
	assert(seen_seqs);

	node_t * itr1 = line->next;
	node_t * itr2 = itr1;
	line->next = NULL; // disconnect cycle

	while(itr2){
		itr2 = itr2->next;
		free(itr1);
		itr1 = itr2;
	}
	free(node_arr);

	for(int i = 0; i < CYCLE_SIZE; i++){
		free(seen_seqs[i]);
	}
	free(seen_seqs);
}

void print_line(int64_t seen_idx){
	assert(line);
	assert(node_arr);
	assert(seen_seqs);

	node_t * curr_node = line;

	for(int i = 0; i < PGM_COUNT; i++){
		if(seen_idx >= 0) (seen_seqs[seen_idx])[i] = curr_node->name;
		printf("%c",curr_node->name);
		curr_node = curr_node->next;
	}
	printf("\n");
}

void spin(int count){
	assert(line);
	assert(node_arr);

	line_idx = (line_idx + (PGM_COUNT - count)) % PGM_COUNT;
	line = node_arr[line_idx];
}

void exchange(int idx_a,int idx_b){
	assert(line);
	assert(node_arr);
	if(idx_a == idx_b) return;

	//node_t * itr = line;
	node_t * a_loc = node_arr[(line_idx+idx_a) % PGM_COUNT];
	node_t * b_loc = node_arr[(line_idx+idx_b) % PGM_COUNT];
	/*
	for(int i = 0; i < PGM_COUNT; i++){
		if(a_loc && b_loc) break;

		if(i == idx_a) a_loc = itr;
		if(i == idx_b) b_loc = itr;

		itr = itr->next;
	}*/

	char tmp = a_loc->name;
	a_loc->name = b_loc->name;
	b_loc->name = tmp;
}

void partner(char name_a, char name_b){
	assert(line);
	if(name_a == name_b) return;

	node_t * itr = line;
	for(int i = 0; i < PGM_COUNT; i++){
		if(itr->name == name_a) itr->name = name_b;
		else if(itr->name == name_b) itr->name = name_a;

		itr = itr->next;
	}
}

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given
	
	FILE *file;
	file = fopen(argv[1], "r");
	char * input = NULL;
	size_t len = 0;
	ssize_t read;
	if (file == NULL)
		return -1; // couldn't open file

	read = getline(&input, &len, file);
	fclose(file);
	char * input_cpy = malloc(strlen(input)+1);
	strcpy(input_cpy,input);

	init_line();
	print_line(-1);

	for(uint64_t i = 0; i < CYCLE_SIZE /*total_dances*/; i++){
		char * move = strtok(input,",");
		while(move){
			if(move[0] == 's'){
				int spin_count = atoi(move+1);
				//printf("spin %d\n",spin_count);
				spin(spin_count);
			}
			else if(move[0] == 'x'){
				char * sep_loc = strchr(move,'/');
				*sep_loc = '\0';
				int ex_1 = atoi(move+1);
				int ex_2 = atoi(sep_loc+1);
				*sep_loc = '/';
				//printf("exchange %d %d\n",ex_1,ex_2);
				exchange(ex_1,ex_2);
			}
			else if(move[0] == 'p'){
				int p_1 = move[1];
				int p_2 = move[3];
				//printf("partner %c %c\n",p_1,p_2);
				partner(p_1,p_2);
			}
			//print_line();
			move = strtok(NULL,",");
		}
		if(1){
			printf("%llu: ",i);
			print_line(i);
		}
		strcpy(input,input_cpy);
	}
	
	if(input) free(input);
	if(input_cpy) free(input_cpy);
    
    printf("%llu: %s\n",total_dances,seen_seqs[(total_dances-1) % cycle_size]);
  
	destroy_line();

	return 0;
}