#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOOP_SIZE (256) // == 5 for testing

typedef struct node_s {
	int val;
	struct node_s * next;
	struct node_s * prev;
} node_t;

node_t * nodes = NULL;
//int curr_idx = 0;
int skip_size = 0;

void print_ring(){
	printf("%d ",nodes->val);

	node_t * itr = nodes->next;
	while(itr != nodes){
		printf("%d ",itr->val);
		itr = itr->next;
	}

	printf("--- ");

	itr = itr->prev;
	while(itr != nodes){
		printf("%d ",itr->val);
		itr = itr->prev;
	}

	printf("%d",itr->val);

	printf("\n");
}

void create_ring(node_t * prev, int curr_val){
	if(curr_val >= LOOP_SIZE){
		if(prev) prev->next = nodes;
		nodes->prev = prev;
		return;
	} 

	node_t * curr = malloc(sizeof(node_t));
	curr->val = curr_val;
	curr->next = NULL;

	if(prev){
		curr->prev = prev;
		prev->next = curr;
	}
	else{
		curr->prev = NULL;
		nodes = curr; // making the first node
	}

	create_ring(curr,curr_val+1);
}

void destroy_ring(){
	nodes->prev->next = NULL; // disconnect loop

	node_t * curr = nodes;

	while(curr){
		node_t * next = curr->next;
		free(curr);
		curr = next;
	}
}

void reset_nodes(int len, int dist_from_start){
	int halfway_dist = (len/2) - dist_from_start;

		if(len % 2 == 0){
			if(halfway_dist > 0){
				int move_back = halfway_dist*2-1;
				for(int i = 0; i < move_back; i++) nodes = nodes->prev;
			}
			else if (halfway_dist <= 0){
				int move_forward = (-2*halfway_dist)+1;
				for(int i = 0; i < move_forward; i++) nodes = nodes->next;
			}
			/*
			else if (halfway_dist == 0){
				nodes = nodes->next;
			}*/
		}
		else{
			if(halfway_dist > 0){
				int move_back = halfway_dist*2;
				for(int i = 0; i < move_back; i++) nodes = nodes->prev;
			}
			else{
				int move_forward = -2*halfway_dist;
				for(int i = 0; i < move_forward; i++) nodes = nodes->next;
			}
	}
}

void reverse(node_t * start, int len){

	int reset_nodes_flag = -1;
	node_t * end = start;
	for(int i = 0; i < len; i++){
		if(end == nodes) reset_nodes_flag = i;
		end = end->next;	
	} 

	printf("\tstart: %d end: %d nodes: %d\n",start->val,end->val,nodes->val);

	if(len < 2) return;

	/*if(reset_nodes_flag == 0){
		for(int i = 0; i < (len-1); i++) nodes = nodes->next;
	}*/

	if(len >= LOOP_SIZE){
		//printf("Here be bugs!\n");

		/*
		if(reset_nodes_flag >= 0){
  			//printf("RNF: %d\n",reset_nodes_flag);
			for(int i = 0; i < (len-reset_nodes_flag); i++) nodes = nodes->next;
		}
		*/

		start->prev->next = NULL; // disconnect loop (reconnect later)

		node_t * temp = NULL;    
    	node_t * curr = start;
 
 		while(curr){
 			temp = curr->prev;
			curr->prev = curr->next;
			curr->next = temp;
			curr = curr->prev;
 		}

 		
 		if(temp){
 			node_t * new_start = temp->prev;
 			new_start->prev = start;
 			start->next = new_start;
 		}

 		if(reset_nodes_flag >= 0)
 			reset_nodes(len,reset_nodes_flag);
    	return; //prev;
	} 

	node_t * pre_end = end->prev;
	node_t * pre_start = start->prev;

	node_t * temp = NULL;
	node_t * curr = start;

	while(curr != end){
		temp = curr->prev;
		curr->prev = curr->next;
		curr->next = temp;
		curr = curr->prev;
	}
	
	end->prev = start;
	start->next = end;

	pre_end->prev = pre_start;
	pre_start->next = pre_end;

	if(reset_nodes_flag >= 0){
		reset_nodes(len,reset_nodes_flag);
		/*
		int halfway_dist = (len/2) - reset_nodes_flag;

		if(len % 2 == 0){
			if(halfway_dist > 0){
				int move_back = halfway_dist*2;
				for(int i = 0; i < move_back; i++) nodes = nodes->prev;
			}
			else if (halfway_dist < 0){
				int move_forward = -2*halfway_dist;
				for(int i = 0; i < move_forward; i++) nodes = nodes->next;
			}
			else if (halfway_dist == 0){
				nodes = nodes->next;
			}
		}
		else{
			if(halfway_dist > 0){
				int move_back = halfway_dist*2;
				for(int i = 0; i < move_back; i++) nodes = nodes->prev;
			}
			else{
				int move_forward = -2*halfway_dist;
				for(int i = 0; i < move_forward; i++) nodes = nodes->next;
			}
		}
*/
		/*
		if(reset_nodes_flag <= (len/2)){
			//if(len % 2 == 0) reset_nodes_flag++;
			int move_forward = (len) - reset_nodes_flag;
			for(int i = 0; i < move_forward; i++) nodes = nodes->next;
		}
		else if(reset_nodes_flag > (len/2)){
			int move_back = reset_nodes_flag - (len);
			for(int i = 0; i < move_back; i++) nodes = nodes->prev;
		}*/
	}
	//printf("curr: %d\ntemp: %d\nstart: %d\nend: %d\npre_end: %d\npre_start: %d\n",curr->val,temp->val,start->val,end->val,pre_end->val,pre_start->val);
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

	int curr_idx = 0;
	fseek(file, 0, SEEK_SET);

	int * numbers = malloc(lines*sizeof(int));

	while ((read = getline(&line, &len, file)) != -1) {
		numbers[curr_idx] = atoi(line);
    	//printf("%d\n", numbers[curr_idx]);
    	curr_idx++;
    	if(line){
    		free(line);
    		line = NULL;
    	}
    }

    create_ring(NULL,0);
    print_ring();

    node_t * curr_pos = nodes;
    for(int i = 0; i < lines; i++){
    	int len = numbers[i];
    	printf("len: %d --> \n", len);

    	reverse(curr_pos,len);
    	curr_pos = curr_pos->next; // move out of reversed section
    	for(int j = 0; j < skip_size; j++) curr_pos = curr_pos->next;
    	skip_size++;
    	print_ring();
    }

    int res = nodes->val * nodes->next->val;
    printf("Result: %d\n",res);

    fclose(file);
	if (line)
        free(line);

    free(numbers);
    destroy_ring();

    return 0;
}