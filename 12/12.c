#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT (2000) //(2000)
#define MAX_CHILDREN (10) // without realloc

typedef struct pgm_s{
	//int id; //implicit by idx in pgms
	int reached; // mark this when traversing
	int child_count;
	struct pgm_s ** children;
} pgm_t;

pgm_t * pgms = NULL;

int explore(pgm_t * root,int group_id){
	if((!root) || root->reached)
		return 0;

	root->reached = group_id;

	int ret = 1;
	for(int i = 0; i < root->child_count;i++){
		ret += explore(root->children[i],group_id);
	}

	return ret;
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

    pgms = calloc(COUNT,sizeof(pgm_t));

    while ((read = getline(&line, &len, file)) != -1) {
		//printf("%s\n",line);

		pgm_t * curr_pgm = &pgms[atoi(strtok(line," "))];
		curr_pgm->children = malloc(MAX_CHILDREN*sizeof(pgm_t*));

		strtok(NULL," ");
		char * rest = strtok(NULL, ",\n");
		//printf("rest: %s\n",rest);
		while(rest){
			while (rest && *rest == '\040')
        		rest++; // skip spaces

        	if(curr_pgm->child_count >= MAX_CHILDREN)
        		realloc(curr_pgm->children,((curr_pgm->child_count)*sizeof(pgm_t*))+1);

        	curr_pgm->children[curr_pgm->child_count] = &pgms[atoi(rest)];
			curr_pgm->child_count++;

			rest = strtok(NULL, ",\n");
		}

    	if(line){
    		free(line);
    		line = NULL;
    	}
    }

    fclose(file);
	if (line)
        free(line);

    /*
    for(int i = 0; i < COUNT; i++){
    	printf("%d - %d\n",i,pgms[i].child_count);
    }
    */

    //pgm_t * zero_pgm = &pgms[0];
    int num_groups = 0;

    for(int i = 0; i < COUNT; i++){
    	pgm_t * curr = &pgms[i];
    	if(!(curr->reached)){
    		num_groups++;
    		int trash = explore(curr,num_groups);
    	}
    }

    // free child ptrs

    free(pgms);

    printf("Result: %d\n",num_groups);

	return 0;
}