#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "mem.h"

#define STEPS 2000

Memory * particles = NULL;

void init(){
	assert(!particles);
	particles = Memory_create();
}

void destroy(){
	assert(particles);

	Memory_destroy(particles);
}

void print_particles(){
	assert(particles);

	for(int i = 0; i < Memory_size(particles); i++){
		cell * p = Memory_get(particles,i);
		printf("p=<%lld,%lld,%lld> v=<%lld,%lld,%lld> a=<%lld,%lld,%lld>\n",p->pX,p->pY,p->pZ,p->vX,p->vY,p->vZ,p->aX,p->aY,p->aZ);
	}
}

void step(){
	assert(particles);
	// remove collisions
	for(int i = 0; i < Memory_size(particles); i++){
		cell * i_curr = Memory_get(particles,i);
		int remove_flag = 0;
		for(int j = i+1; j < Memory_size(particles); j++){
			cell * j_curr = Memory_get(particles,j);

			if((i_curr->pX == j_curr->pX) && (i_curr->pY == j_curr->pY) && (i_curr->pZ == j_curr->pZ)){
				remove_flag = 1;
				Memory_delete(particles,j);
				j--;
			}
		}
		if(remove_flag){
			Memory_delete(particles,i);
			i--;
		}
	}

	// update position and velocity
	for(int i = 0; i < Memory_size(particles); i++){
		cell * c = Memory_get(particles,i);

		c->vX += c->aX;
		c->vY += c->aY;
		c->vZ += c->aZ;
		c->pX += c->vX;
		c->pY += c->vY;
		c->pZ += c->vZ;
	}

}

void simulate(){
	assert(particles);

	for(int i = 0; i < STEPS; i++){
		step();
	}
	print_particles();
	printf("Size: %zu\n",Memory_size(particles));
}

// rank first by lowest acceleration, then lowest velocity, then lowest distance from center
void find_closest(){
	assert(particles);
	cell * best = Memory_get(particles,0);
	int64_t a_best = llabs(best->aX) + llabs(best->aY) + llabs(best->aZ);
	int64_t v_best = llabs(best->vX) + llabs(best->vY) + llabs(best->vZ);
	int64_t p_best = llabs(best->pX) + llabs(best->pY) + llabs(best->pZ);

	for(int i = 1; i < Memory_size(particles); i++){
		cell * curr = Memory_get(particles,i);
		int64_t a_curr = llabs(curr->aX) + llabs(curr->aY) + llabs(curr->aZ);
		int64_t v_curr = llabs(curr->vX) + llabs(curr->vY) + llabs(curr->vZ);
		int64_t p_curr = llabs(curr->pX) + llabs(curr->pY) + llabs(curr->pZ);

		if((a_curr < a_best) || ((a_curr == a_best) && (v_curr < v_best)) || (((a_curr == a_best) && (v_curr == v_best)) && (p_curr < p_best))){
			best = curr;
			a_best = a_curr;
			v_best = v_curr;
			p_best = p_curr;
		}
	}
	printf("\tResult: Particle %d p=<%lld,%lld,%lld> v=<%lld,%lld,%lld> a=<%lld,%lld,%lld>\n",best->addr,best->pX,best->pY,best->pZ,best->vX,best->vY,best->vZ,best->aX,best->aY,best->aZ);
}

int main(int argc, char * argv[]){
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

	while((read = getline(&line, &len, file)) != -1){
		cell * p = malloc(sizeof(cell));
		p->addr = curr_idx;

		char * tokstr = strtok(line,"<");
		p->pX = atoi(strtok(NULL,","));
		p->pY = atoi(strtok(NULL,","));
		p->pZ = atoi(strtok(NULL,">"));
		tokstr = strtok(NULL,"<");
		p->vX = atoi(strtok(NULL,","));
		p->vY = atoi(strtok(NULL,","));
		p->vZ = atoi(strtok(NULL,">"));
		tokstr = strtok(NULL,"<");
		p->aX = atoi(strtok(NULL,","));
		p->aY = atoi(strtok(NULL,","));
		p->aZ = atoi(strtok(NULL,">"));

		Memory_insert(particles,curr_idx,p);
		free(p); // Memory_insert copies p

		if(line){
			free(line);
			line = NULL;
		}
		curr_idx++;
	}

	simulate();

	if(line){
			free(line);
			line = NULL;
	}
	fclose(file);

	destroy();

	return 0;
}