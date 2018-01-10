#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct layer_s{
	int depth;
	int range;
	int trange;
	int dir; // 0 == down, otherwise == up
	int curr_pos; // 0 == top
	struct layer_s * next;
} layer_t;

typedef struct {
	layer_t * hd;
	layer_t * tl;
	int num_layers;
} firewall_t;

int cycle_size = 0; // how many steps until all sentries reach original state simultaneously

firewall_t * fw = NULL;
int time = 0;
int packet_pos = -1;
int times_caught = 1;
int delay = 0;

// http://rosettacode.org/wiki/Least_common_multiple#C
int gcd(int m, int n)
{
        int tmp;
        while(m) { tmp = m; m = n % m; n = tmp; }       
        return n;
}
 
int lcm(int m, int n)
{
        return m / gcd(m, n) * n;
}

layer_t * get_layer(int depth){
	assert(fw);
	assert(depth <= fw->num_layers);

	layer_t * it = fw->hd;
	while(it){
		if(it->depth == depth)
			break; // layer found 
		else if(it->depth > depth){
			it = NULL; // layer not found
			break;
		}

		it = it->next;
	}

	return it;
}

void cleanup(){
	layer_t* it1 = fw->hd;
    layer_t* it2 = fw->hd;

    while(it2){
		it2 = it1->next;

		free(it1);
		it1 = it2;
	}
	free(fw);
}

// result of delay
void scanners_set(int time){
	assert(fw);
	layer_t * it = fw->hd;

	while(it){
		int new_pos = (it->curr_pos+time) % it->trange;
		it->curr_pos = new_pos;

		it = it->next;
	}
}

void scanners_step(){
	assert(fw);
	layer_t * it = fw->hd;

	while(it){
		/*if(it->curr_pos == (it->range-1))
			it->dir = 1;
		else if(it->curr_pos == 0){
			it->dir = 0;
		}
		it->curr_pos = (it->dir) ? (it->curr_pos-1) : (it->curr_pos+1);*/
		it->curr_pos = (it->curr_pos + 1) % it->trange;

		it = it->next;
	}
}

void scanners_reset(){
	assert(fw);
	layer_t * it = fw->hd;

	while(it){
		it->curr_pos = 0;
		it->dir = 0;

		it = it->next;
	}
}

void get_cycle(){
	assert(fw);
	assert(!cycle_size);

	layer_t * it = fw->hd;
	cycle_size = it->range;

	while(it->next){
		cycle_size = lcm(cycle_size,it->next->range);
		//((cycle_size % it->range) == 0) ? cycle_size : cycle_size*it->range;

		it = it->next;
	}

	if(cycle_size == 1){
		cycle_size = 0;
	}
	else{
		printf("\tCycle! %d\n",cycle_size);
	}
}

void check_cycle(){
	assert(fw);
	assert(!cycle_size);

	layer_t * it = fw->hd;
	int cycle_flag = 0;

	while(it){
		cycle_flag |= it->curr_pos;

		it = it->next;
	}

	if(!cycle_flag){
		cycle_size = delay;
		printf("\tFound %d cycle!\n",cycle_size);
	}
}

// performs severity calculation
int execute(){
	assert(fw);

	packet_pos = -1;
	int severity = 0;
	times_caught = 0;

	while(packet_pos < fw->num_layers){
		packet_pos++;
		//printf("ppos: %d\n",packet_pos);
		layer_t * curr_l = get_layer(packet_pos);
		if(curr_l){
			if(curr_l->curr_pos == 0){
				severity += (curr_l->depth * curr_l->range);
				times_caught++;
				//printf("Caught at time %d! Sev %d\n",packet_pos,severity);
			}
		}

		scanners_step();
	}

	return severity;
}

int main(int argc, char * argv[]){
	if (argc < 2)
		return -1; // no input given

	FILE *file;
	file = fopen(argv[1], "r");
	if (file == NULL)
		return -1; // couldn't open file

	char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fw = calloc(1,sizeof(firewall_t));
    fw->num_layers = 0;

     while ((read = getline(&line, &len, file)) != -1) {
		//printf("%s\n",line);

		layer_t * lyr = calloc(1,sizeof(layer_t));

		char * tok = strtok(line, ":");
		lyr->depth = atoi(tok);
		tok = strtok(NULL, "\n");
		while (tok && *tok == '\040')
        		tok++; // skip space(s)

        lyr->range = atoi(tok); // should just be an integer until the \n
        lyr->trange = (((lyr->range)-2)*2)+2;

        if(fw->tl){
        	fw->tl->next = lyr;
        	fw->tl = lyr;
        }
        else{
        	fw->hd = lyr;
        	fw->tl = lyr;
        }
        fw->num_layers = lyr->depth; // update max layer

        //printf("Layer %d %d %d\n",lyr->depth,lyr->range,lyr->trange);

		if(line){
    		free(line);
    		line = NULL;
    	}
    }

    fclose(file);
	if (line)
        free(line);

    int best_sev = 1;
    //get_cycle();

    while(times_caught > 0){
    	/*int real_delay = (cycle_size) ? (delay % cycle_size) : delay;
    	for(int i = 0; i < real_delay; i++){
    		scanners_step();
    	}*/
    	//if(!cycle_size) check_cycle();
    	//else printf("\t!!!!!! %d\n",cycle_size);
    	scanners_set(delay);

    	best_sev = execute();
    	/*if (delay % 10000 == 0)*/ printf("Delay %d TC %d Sev %d\n",delay,times_caught,best_sev);

    	scanners_reset();
    	delay += 2; //(cycle_size) ? (delay+1)%cycle_size : delay+1;
    }

    //printf("Severity: %d\n",execute());
    printf("Delay: %d\n",delay-2);

    cleanup();

	return 0;
}