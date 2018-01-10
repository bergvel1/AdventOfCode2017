#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "mem.h"
#include "queue.h"

#define NUM_REGS 26
int64_t * regs0 = NULL;
int64_t * regs1 = NULL;
Memory * mem = NULL;
queue_t * q0 = NULL; // process 0 reads from this, process 1 writes to this
queue_t * q1 = NULL; // vice verse
int64_t pgm1_sends = 0;

int64_t last_sound = 0;

void init(){
	assert(!regs0);
	assert(!regs1);
	assert(!mem);
	assert(!q0);
	assert(!q1);

	regs0 = calloc(sizeof(int64_t),NUM_REGS);
	regs1 = calloc(sizeof(int64_t),NUM_REGS);
	regs1[15] = 1; // register 'p'
	mem = Memory_create();
	q0 = queue_init();
	q1 = queue_init();
}

void destroy(){
	assert(regs0);
	assert(regs1);
	assert(mem);
	assert(q0);
	assert(q1);

	free(regs0);
	free(regs1);
	Memory_destroy(mem);
	queue_destroy(q0);
	queue_destroy(q1);
}

void execute(){
	assert(regs0);
	assert(regs1);
	assert(mem);

	int pc0 = 0;
	int pc1 = 0;
	int waiting0 = 0; // if process 0 is waiting
	int waiting1 = 0; // if process 1 is waiting
	int size = Memory_size(mem);
	int active = 0; // which process is currently active
	int pc = active ? pc1 : pc0; // PC for active process

	while((pc < size) && (pc >= 0)){
		if(waiting0 && waiting1){
			printf("Deadlock!\n");
			return;
		}

		const cell * c = Memory_get(mem,pc);

		//snd
		if(c->op == 0){
			printf("snd ");
			if(c->arg1_isreg){
				printf("[%lld]\n", c->arg1);
				last_sound = active ? regs1[c->arg1] : regs0[c->arg1];
			}
			else{
				printf("%lld\n", c->arg1);
				last_sound = c->arg1;
			}
			//printf("\ttweeeet~ %lld\n",last_sound);
			if(active){
				enqueue(q0,last_sound);
				pgm1_sends++;
				if(waiting0) waiting0 = 0;
			}
			else{
				enqueue(q1,last_sound);
				if(waiting1) waiting1 = 0;
			}
		}
		//set
		else if(c->op == 1){
			int64_t reg_idx = c->arg1;
			int64_t val;

			printf("set ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = active ? regs1[c->arg2] : regs0[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			active ? (regs1[reg_idx] = val) : (regs0[reg_idx] = val);
		} 
		//add
		else if(c->op == 2){
			int64_t reg_idx = c->arg1;
			int64_t val;

			printf("add ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = active ? regs1[c->arg2] : regs0[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			active ? (regs1[reg_idx] += val) : (regs0[reg_idx] += val);
		}
		//mul
		else if(c->op == 3){
			int64_t reg_idx = c->arg1;
			int64_t val;
			
			printf("mul ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = active ? regs1[c->arg2] : regs0[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			active ? (regs1[reg_idx] *= val) : (regs0[reg_idx] *= val);
		}
		//mod
		else if(c->op == 4){
			int64_t reg_idx = c->arg1;
			int64_t val;
			
			printf("mod ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = active ? regs1[c->arg2] : regs0[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			active ? (regs1[reg_idx] %= val) : (regs0[reg_idx] %= val);
		}
		//rcv
		else if(c->op == 5){
			int64_t reg_idx = c->arg1;;

			printf("rcv ");
			if(c->arg1_isreg) printf("[%lld]\n", c->arg1);
			else printf("%lld (error!)\n", c->arg1);

			if(active){
				if(queue_size(q1) > 0)
					regs1[reg_idx] = dequeue(q1);
				else{
					waiting1 = 1;
					pc--;
				} 
			}
			else{
				if(queue_size(q0) > 0)
					regs0[reg_idx] = dequeue(q0);
				else{
					waiting0 = 1;
					pc--;
				}
			}

			/*PART ONE
			if(c->arg1_isreg){
				printf("[%lld]\n", c->arg1);
				val = active ? regs1[c->arg2] : regs0[c->arg2];
			}
			else{ 
				printf("%lld\n", c->arg1);
				val = c->arg1;
			}
			if(val){ printf("\t Recovered %lld\n",last_sound);
						destroy();
						exit(0);
					}*/
		}
		//jgz
		else if(c->op == 6){
			int64_t val1;
			int64_t val2;
			printf("jgz ");
			if(c->arg1_isreg){
				printf("[%lld] ", c->arg1);
				val1 = active ? regs1[c->arg1] : regs0[c->arg1];
			}
			else{ 
				printf("%lld ", c->arg1);
				val1 = c->arg1;
			}
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val2 = active ? regs1[c->arg2] : regs0[c->arg2];
			}
			else{ 
				printf("%lld\n", c->arg2);
				val2 = c->arg2;
			}

			if(val1 > 0){
				pc += val2 - 1; // minus 1 because we increment at the end of the loop
			}
		}

		pc++;

		if(active){
			pc1 = pc; // write updated PC out
			if(!waiting0) active = 0; // if process 0 not waiting, switch to it
		}
		else{
			pc0 = pc; // write updated PC out
			if(!waiting1) active = 1; // if process 1 not waiting, switch to it
		}
		pc = active ? pc1 : pc0; // get PC for next round
	}
}

void mem_dump(){
	assert(mem);

	size_t idx = 0;
	const cell * c = Memory_get(mem,idx);

	while(c){
		printf("%d: %d %lld %lld (%d %d)\n",c->addr,c->op,c->arg1,c->arg2,c->arg1_isreg,c->arg2_isreg);
		c = Memory_get(mem,++idx);
	}
}

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given
	
	FILE *file;
	file = fopen(argv[1], "r");
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	uint16_t curr_idx = 0;
	if (file == NULL)
		return -1; // couldn't open file

	init();

	while ((read = getline(&line, &len, file)) != -1) {
		char * inst = strtok(line," ");
		cell * c = calloc(sizeof(cell),1);
		c->addr = curr_idx;

		if(strcmp(inst,"snd") == 0){
			char * rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;
			}
			c->op = 0;
	
			//printf("Play: %d\n",freq);
			//last_sound = freq;
		}
		else if(strcmp(inst,"set") == 0){
			char * rest = strtok(NULL," ");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;

				exit(1); // invalid register
			}

			rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg2 = (*rest) - 97;
				c->arg2_isreg = 1;
			}
			else{
				c->arg2 = atoi(rest);
				c->arg2_isreg = 0;
			}

			c->op = 1;

			//regs[reg_idx] = val;
		}
		else if(strcmp(inst,"add") == 0){
			char * rest = strtok(NULL," ");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;

				exit(1); // invalid register
			}

			rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg2 = (*rest) - 97;
				c->arg2_isreg = 1;
			}
			else{
				c->arg2 = atoi(rest);
				c->arg2_isreg = 0;
			}

			c->op = 2;

			//regs[reg_idx] += val;
		}
		else if(strcmp(inst,"mul") == 0){
			char * rest = strtok(NULL," ");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;

				exit(1); // invalid register
			}

			rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg2 = (*rest) - 97;
				c->arg2_isreg = 1;
			}
			else{
				c->arg2 = atoi(rest);
				c->arg2_isreg = 0;
			}

			c->op = 3;

			//regs[reg_idx] *= val;
		}
		else if(strcmp(inst,"mod") == 0){
			char * rest = strtok(NULL," ");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;

				exit(1); // invalid register
			}

			rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg2 = (*rest) - 97;
				c->arg2_isreg = 1;
			}
			else{
				c->arg2 = atoi(rest);
				c->arg2_isreg = 0;
			}

			c->op = 4;

			//regs[reg_idx] %= val;
		}
		else if(strcmp(inst,"rcv") == 0){
			char * rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;
			}

			c->op = 5;

			//if(regs[reg_idx]){
			//	printf("Recovered frequency %d\n",last_sound);
			//}
		}
		else if(strcmp(inst,"jgz") == 0){
			char * rest = strtok(NULL," ");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg1 = (*rest) - 97;
				c->arg1_isreg = 1;
			}
			else{
				c->arg1 = atoi(rest);
				c->arg1_isreg = 0;
			}

			rest = strtok(NULL,"\n");
			while((*rest) == ' ') rest++;
			if(isalpha(*rest)){
				c->arg2 = (*rest) - 97;
				c->arg2_isreg = 1;
			}
			else{
				c->arg2 = atoi(rest);
				c->arg2_isreg = 0;
			}

			c->op = 6;
		}

		
		Memory_insert(mem,curr_idx,c);
		if(line){
			free(line);
			line = NULL;
		}
		//free(c);
		curr_idx++;
	}

	if(line) free(line);
	fclose(file);
	//mem_dump();
	execute();
	fprintf(stderr,"Pgm 1 sent %lld values\n",pgm1_sends);

	destroy();
	return 0;
}