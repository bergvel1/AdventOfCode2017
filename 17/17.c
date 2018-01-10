#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STEP_COUNT 324
uint64_t NUM_INSERTIONS = 50000000; //2017

typedef struct node_s{
	uint64_t val;
	struct node_s * next;
} node_t;

typedef struct list_s{
	uint64_t size;
	node_t * hd;
} list_t;

list_t * spinlock = NULL;
size_t curr_pos = 0;
uint64_t ret = 0; 

void init_spinlock(){
	assert(!spinlock);

	spinlock = malloc(sizeof(list_t));
	spinlock->size = 1;
	spinlock->hd = malloc(sizeof(node_t));

	spinlock->hd->val = 0;
	spinlock->hd->next = spinlock->hd;
}

void destroy_spinlock(){
	assert(spinlock);

	node_t * itr1 = spinlock->hd;
	node_t * itr2 = spinlock->hd;
	for(int i = 0; i < spinlock->size; i++){
		itr2 = itr1->next;
		free(itr1);
		itr1 = itr2;
	}
	free(spinlock);
}

void print_spinlock(){
	assert(spinlock);

	node_t * itr = spinlock->hd;
	for(int i = 0; i < spinlock->size; i++){
		if(i == curr_pos) printf("(");
		printf("%llu",itr->val);
		if(i == curr_pos) printf(")");
		printf(" ");
		itr = itr->next;
	}
	printf("\n");
}

void step_and_insert(uint64_t insert_val){
	uint64_t new_pos = (curr_pos + STEP_COUNT) % spinlock->size;

	if(new_pos == 0){
		ret = insert_val; 
	}

	curr_pos = new_pos+1;
	spinlock->size++;

	/*
	node_t * insert_after = spinlock->hd;
	for(int i = 0; i < new_pos; i++){
		insert_after = insert_after->next;
	}
	node_t * insert_before = insert_after->next;

	node_t * insert = malloc(sizeof(node_t));
	insert->val = insert_val;
	insert_after->next = insert;
	insert->next = insert_before;

	curr_pos = new_pos+1;
	spinlock->size++;

	if(insert_val == NUM_INSERTIONS){
		printf("\tRESULT: %d\n",spinlock->hd->next->val);
	}
	*/
}

int main(){
	init_spinlock();

	for(uint64_t i = 1; i <= NUM_INSERTIONS; i++){
		step_and_insert(i);
		if((i % 5000) == 0) printf("-%llu- %llu\n",i,ret);
		//print_spinlock();
	}

	destroy_spinlock();
}