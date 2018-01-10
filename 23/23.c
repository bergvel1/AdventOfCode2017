#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "mem.h"
#include "queue.h"

#define NUM_REGS 8 //26
int64_t * regs = NULL;
Memory * mem = NULL;
queue_t * q = NULL; // process 0 reads from this, process 1 writes to this (still needed?)

int mul_calls = 0;

void init(){
	assert(!regs);
	assert(!mem);
	assert(!q);

	regs = calloc(sizeof(int64_t),NUM_REGS);
	regs[0] = 1; // register 'a' starts with value 1
	mem = Memory_create();
	q = queue_init();
}

void destroy(){
	assert(regs);
	assert(mem);
	assert(q);

	free(regs);
	Memory_destroy(mem);
	queue_destroy(q);
}

void reg_dump(){
	assert(regs);

	for(int i = 0; i < NUM_REGS; i++){
		printf("%c: %lld\n",(char) i+97,regs[i]);
	}
}

void execute(){
	assert(regs);
	assert(mem);

	int size = Memory_size(mem);
	int pc = 0; // program counter

	while((pc < size) && (pc >= 0)){
		reg_dump();

		const cell * c = Memory_get(mem,pc);
		printf("\t%d: ",pc);

		/*
		//snd
		if(c->op == 0){
			printf("snd ");
			if(c->arg1_isreg){
				printf("[%lld]\n", c->arg1);
				last_sound = active ? regs1[c->arg1] : regs[c->arg1];
			}
			else{
				printf("%lld\n", c->arg1);
				last_sound = c->arg1;
			}
			//printf("\ttweeeet~ %lld\n",last_sound);
			if(active){
				enqueue(q,last_sound);
				pgm1_sends++;
				if(waiting0) waiting0 = 0;
			}
			else{
				enqueue(q1,last_sound);
				if(waiting1) waiting1 = 0;
			}
		}
		*/

		//set
		/*else*/ if(c->op == 1){
			int64_t reg_idx = c->arg1;
			int64_t val;

			printf("set ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = regs[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			regs[reg_idx] = val;
		} 

		/*
		//add
		else if(c->op == 2){
			int64_t reg_idx = c->arg1;
			int64_t val;

			printf("add ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = regs[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			active ? (regs1[reg_idx] += val) : (regs[reg_idx] += val);
		}
		*/

		//mul
		else if(c->op == 3){
			int64_t reg_idx = c->arg1;
			int64_t val;
			
			printf("mul ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = regs[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			regs[reg_idx] *= val;
			mul_calls++;
		}

		/*
		//mod
		else if(c->op == 4){
			int64_t reg_idx = c->arg1;
			int64_t val;
			
			printf("mod ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = regs[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			active ? (regs1[reg_idx] %= val) : (regs[reg_idx] %= val);
		}
		*/

		/*
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
				if(queue_size(q) > 0)
					regs[reg_idx] = dequeue(q);
				else{
					waiting0 = 1;
					pc--;
				}
			}
		}
		*/

		/*
		//jgz
		else if(c->op == 6){
			int64_t val1;
			int64_t val2;
			printf("jgz ");
			if(c->arg1_isreg){
				printf("[%lld] ", c->arg1);
				val1 = active ? regs1[c->arg1] : regs[c->arg1];
			}
			else{ 
				printf("%lld ", c->arg1);
				val1 = c->arg1;
			}
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val2 = regs[c->arg2];
			}
			else{ 
				printf("%lld\n", c->arg2);
				val2 = c->arg2;
			}

			if(val1 > 0){
				pc += val2 - 1; // minus 1 because we increment at the end of the loop
			}
		}
		*/

		//sub
		else if(c->op == 7){
			int64_t reg_idx = c->arg1;
			int64_t val;

			printf("sub ");
			if(c->arg1_isreg) printf("[%lld] ", c->arg1);
			else printf("%lld (error!) ", c->arg1);
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val = regs[c->arg2];
			}
			else{
				printf("%lld\n", c->arg2);
				val = c->arg2;
			}

			regs[reg_idx] -= val;
		}
		//jnz
		else if(c->op == 8){
			if((pc == 14) && (regs[5] != 0) && (regs[6] != 0)){
			//	regs[6] = 0;
			//	regs[4] = 53250;
			}
			if((pc == 19) && (regs[5] == 0) /*&& (regs[3] == 2)*/){
				// break free of first busy loop
				regs[6] = 0; // reg 'g' must == 0
				regs[4] = regs[1]; // 'e' == 'b'
				regs[5] = 0; // set 'f' to zero to trigger 'h' increment
			}
			if(pc == 23){
				// break free of second busy loop
				regs[6] = 0; // reg 'g' must again == 0
				regs[3] = regs[1]; // 'd' == 'b' == 106500
				//regs[5] = 0; // set 'f' to zero to trigger 'h' increment
			}


			int64_t val1;
			int64_t val2;
			printf("jnz ");
			if(c->arg1_isreg){
				printf("[%lld] ", c->arg1);
				val1 = regs[c->arg1];
			}
			else{ 
				printf("%lld ", c->arg1);
				val1 = c->arg1;
			}
			if(c->arg2_isreg){
				printf("[%lld]\n", c->arg2);
				val2 = regs[c->arg2];
			}
			else{ 
				printf("%lld\n", c->arg2);
				val2 = c->arg2;
			}

			if(val1 != 0){
				pc += val2 - 1; // minus 1 because we increment at the end of the loop
			}
		}
		else{
			fprintf(stderr,"\t Unknown instruction! (opcode %d)\n",c->op);
		}

		pc++;
	}
}

void simulate(){
	int64_t b = 106500;
	int64_t c = 123500;
	int64_t d = 2;
	int64_t e = 2;
	int64_t f = 1;
	int64_t h = 0;

	while(b <=  c){
		d = 2;
		f = 1;

		printf("b = %lld, h = %lld\n",b,h);

		if(((b % 2) == 0) || ((b % 3) == 0) || (((b % 5) == 0))) f = 0;

		else {
			while(d < ((b/2)+1)){
				e = 2;
				if(f == 0){
					d = b;
					e = b;
					break;
				}
				while(e < (sqrt(b)+1)){
					if(((d * e) == b)){
						f = 0;
						e = b;
						break;
					} 
					e++;
				}
				d++;
			}
		}
			
		if(f == 0) h++;
	
		b += 17;
	}

	printf("Result: h = %lld\n",h);
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
		else if(strcmp(inst,"sub") == 0){
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

			c->op = 7;

			//regs[reg_idx] += val;
		}
		else if(strcmp(inst,"jnz") == 0){
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

			c->op = 8;
		}

		
		Memory_insert(mem,curr_idx,c);
		if(line){
			free(line);
			line = NULL;
		}
		free(c);
		curr_idx++;
	}

	if(line) free(line);
	fclose(file);
	//mem_dump();
	//execute();
	//fprintf(stderr,"\t%d mul calls\n",mul_calls);
	simulate();

	destroy();
	return 0;
}