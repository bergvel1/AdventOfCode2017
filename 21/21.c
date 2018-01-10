#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct rule_s{
	int size;
	char** pre;
	char** post;
} rule_t;

rule_t * rules = NULL;
int num_rules = 0;

char ** art = NULL;
int curr_size = 0;

void init(){
	assert(!rules);
	assert(!art);

	rules = calloc(sizeof(rule_t),num_rules+1);

	curr_size = 3;
	art = malloc(sizeof(char*)*(curr_size));
	for(int i = 0; i < curr_size; i++){
		art[i] = malloc(curr_size+1);
	}
	strcpy(art[0],".#.");
	strcpy(art[1],"..#");
	strcpy(art[2],"###");
}

void destroy(){
	assert(rules);
	assert(art);

	for(int i = 0; i < num_rules; i++){
		rule_t * r = &(rules[i]);
		for(int j = 0; j < r->size; j++){
			free(r->pre[j]);
			free(r->post[j]);
		}
		free(r->post[r->size]);

		free(r->pre);
		free(r->post);
	}
	free(rules);

	for(int i = 0; i < curr_size; i++){
		free(art[i]);
	}
	free(art);
}

void add_rule(char * rule,int idx){
	assert(rules);

	rule_t * r = &(rules[idx]);
	r->size = 0;
	char * itr = rule;
	while((*itr != '\0') && (*itr != '/')){
		itr++;
		r->size++;
	}
	itr = rule;

	r->pre = malloc(sizeof(char*)*(r->size));
	for(int i = 0; i < r->size; i++){
		(r->pre)[i] = malloc(1+r->size); // allocate space
		for(int j = 0; j < r->size; j++){
			((r->pre)[i])[j] = *itr; // copy characters
			itr++;
		}
		((r->pre)[i])[r->size] = '\0';
		itr++; // skip "\" character
	}

	itr += 3; // skip past "=> "

	r->post = malloc(sizeof(char*)*((r->size) + 1));
	for(int i = 0; i < (r->size+1); i++){
		(r->post)[i] = malloc(2+r->size); // allocate space
		for(int j = 0; j < (r->size+1); j++){
			((r->post)[i])[j] = *itr; // copy characters
			itr++;
		}
		((r->post)[i])[(r->size+1)] = '\0';
		itr++; // skip "\" character
	}
}

void print_rule(int idx){
	assert(rules);

	rule_t * r = &(rules[idx]);
	printf("Size: %d\n\t",r->size);

	for(int i = 0; i < r->size; i++){
		printf("%s\\",((r->pre)[i]));
	}
	printf(" => ");
	for(int i = 0; i < (r->size+1); i++){
		printf("%s\\",((r->post)[i]));
	}
	printf("\n");
}

void print_art(){
	assert(art);

	for(int i = 0; i < curr_size; i++){
		for(int j = 0; j < curr_size; j++){
			printf("%c",((art)[i])[j]);
		}
		printf("\n");
	}
}

// transforms copies result of rules[rule_idx] to the chunk in new_art starting at (new_i,new_j)
void update_chunk(int new_i, int new_j, int rule_idx,char ** new_art){
	rule_t * r = &(rules[rule_idx]);
	char ** post = r->post;

	for(int i = 0; i < (r->size+1); i++){
		for(int j = 0; j < (r->size+1); j++){
			((new_art)[new_i+i])[new_j+j] = ((post)[i])[j];
		}
	}
}

// checks square of art array from start_i to (start_i + multiplicity) and from start_j to (start_j + multiplicity) to find a corresponding enhancement rule
void transform(int multiplicity,int start_i, int start_j,char ** new_art){
	assert(rules);
	assert(art);
	assert((multiplicity == 2) || (multiplicity == 3));

	rule_t * r = rules;
	for(int r_idx = 0; r_idx < num_rules; r_idx++){
		r = &(rules[r_idx]);
		if(r->size == multiplicity){ // rule must be of correct size for current chunk
			//printf("checking rule %d...\n",r_idx);
			// check if rule applies

			int rule_i = 0;
			int rule_j = 0;
			int new_i = (start_i/multiplicity) * (multiplicity+1); // starting coords of chunk in new_art
			int new_j = (start_j/multiplicity) * (multiplicity+1);
			char * rule_itr;
			char * art_itr;
			int no_match_flag = 0; // marks discrepancy between art and rule to break out of loops

			// left-right, top-bottom (regular)
			for(int i = start_i; i < (start_i+multiplicity); i++){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int j = start_j; j < (start_j+multiplicity); j++){
					char rule_char = ((r->pre)[rule_i])[rule_j];
					char art_char = ((art)[i])[j];

					if((rule_char) != (art_char)){
						//printf("\tbonk... \"%c\" vs \"%c\" (%d,%d)\n%s\n%s\n",rule_char,art_char,i,j,((r->pre)[rule_i]),(art)[i]);
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){ /*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// top-bottom, right-left (rotate 90deg clockwise)
			for(int j = (start_j+multiplicity-1); j >= start_j; j--){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int i = start_i; i < (start_i+multiplicity); i++){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// right-left, bottom-top (rotate 180 degrees)
			for(int i = (start_i+multiplicity-1); i >= start_i; i--){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int j = (start_j+multiplicity-1); j >= start_j; j--){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// bottom-top, left-right (rotate 90 degrees counter-clockwise)
			for(int j = start_j; j < (start_j+multiplicity); j++){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int i = (start_i+multiplicity-1); i >= start_i; i--){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// now, check mirrored views
			// NOTE: only need to check vertical mirroring, because horizontal mirroring is equivalent to 180deg rotation plus vertical mmirroring
			// right-left, top-bottom (regular, mirrored over vertical axis)
			for(int i = start_i; i < (start_i+multiplicity); i++){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int j = (start_j+multiplicity-1); j >= start_j; j--){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// bottom-top, right-left (90deg clockwise rotation, mirrored over vertical axis)
			for(int j = (start_j+multiplicity-1); j >= start_j; j--){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int i = (start_i+multiplicity-1); i >= start_i; i--){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// left-right, bottom-top (180deg rotation, mirrored over vertical axis (i.e. regular mirrored over horizontal axis))
			for(int i = (start_i+multiplicity-1); i >= start_i; i--){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int j = start_j; j < (start_j+multiplicity); j++){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
			no_match_flag = rule_i = rule_j = 0; // otherwise, reset and try a different orientation

			// top-bottom, left-right (90deg counter-clockwise rotation, mirrored over vertical axis)
			for(int j = start_j; j < (start_j+multiplicity); j++){
				if(no_match_flag){
					break;
				}
				rule_j = 0;
				for(int i = start_i; i < (start_i+multiplicity); i++){
					rule_itr = &((r->pre)[rule_i])[rule_j];
					art_itr = &((art)[i])[j];

					if((*rule_itr) != (*art_itr)){
						no_match_flag = 1;
						break;
					}
					rule_j++;
				}
				rule_i++;
			}
			if(no_match_flag == 0){/*printf("\tding!\n");*/ return update_chunk(new_i,new_j,r_idx,new_art);} // found valid rule!
		}
	}

	// should not be reached
	fprintf(stderr,"No matching rule found for chunk at (%d,%d)! Exiting...\n",start_i,start_j);
	
	// free new_art
	for(int i = 0; i < (curr_size+(curr_size/multiplicity));i++){
		free(new_art[i]);
	}
	free(new_art);

	destroy();
	exit(0);
}

// execute itr iterations and returns number of "#" pixels in art
int execute(int itr){
assert(rules);
assert(art);

	// base case: return number of lit pixels
	if(itr == 0){
		int ret = 0;
		for(int i = 0; i < curr_size; i++){
			char * itr = art[i];
			for(int j = 0; j < curr_size; j++){
				if(*itr == '#') ret++;
				itr++;
			}
		}
		return ret;
	}

	// check how grid should be split
	int multiplicity = 0;
	if((curr_size % 2) != 0){ 
		multiplicity = 3; // if dimensions are divisibe by 3 (and only 3)
	}
	else{ 
		multiplicity = 2; // if dimensions are divisible by two
	}


	int num_divs = curr_size / multiplicity;
	char ** new_art = malloc(sizeof(char*)*(curr_size+num_divs)); // allocate new 2D array to store transformed art
	for(int i = 0; i < (curr_size+num_divs); i++){
		new_art[i] = malloc(curr_size+num_divs+1);
	}

	// iterate over subsections
	for(int i = 0; i < num_divs; i++){
		for(int j = 0; j < num_divs; j++){
			// calculate start coords and pass to transform()
			transform(multiplicity,i*multiplicity,j*multiplicity,new_art);
		}
	}

	// free old art and replace with new_art
	for(int i = 0; i < curr_size; i++){
		free(art[i]);
	}
	free(art);
	art = new_art;
	curr_size += num_divs; // update size to reflect transformation

	// recurse
	return execute(itr-1);

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

	while(!feof(file)){
  		char ch = fgetc(file);
  		if(ch == '\n'){
			num_rules++;
		}
	}
	num_rules++; // off by one if we don't do this
	fseek(file, 0, SEEK_SET);

	init();

	while((read = getline(&line, &len, file)) != -1){
		add_rule(line,curr_idx);

		if(line){
			free(line);
			line = NULL;
		}
		curr_idx++;
	}

	if(line){
			free(line);
			line = NULL;
	}
	fclose(file);

	/*
	for(int i = 0; i < num_rules; i++){
		print_rule(i);
	}*/

	printf("Result: %d\n", execute(18));
	//print_art();

	destroy();

	return 0;
}