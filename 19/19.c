#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int x_dim = 0;
int y_dim = 0;

char ** maze = NULL;

int steps = -1;

void init_maze(){
	printf("Init %dx%d maze...\n",x_dim,y_dim);

	maze = calloc(y_dim,sizeof(char*));
	for(int y = 0; y < y_dim; y++){
		maze[y] = malloc(x_dim+1); // plus 1 for terminating byte
	}
}

void destroy_maze(){
	assert(maze);

	for(int y = 0; y < y_dim; y++){
		free(maze[y]);
	}
	free(maze);
}

void print_maze(){
	assert(maze);

	for(int y = 0; y < y_dim; y++){
		printf("%s",maze[y]);
	}
}

// 0 -> N
// 1 -> E
// 2 -> S
// 3 -> W
void explore(int x, int y, int dir){
	steps++;
	char curr = (maze[y])[x];
	if(curr == ' ') return; // either finished or went off-route

	if(curr == '+'){
		// change directions
		if(dir == 0){
			if(x > 0){
				if((maze[y])[x-1] != ' ') return explore(x-1,y,3);
			}
			if(x < (x_dim-1)){
				if((maze[y])[x+1] != ' ') return explore(x+1,y,1);
			}
		}
		if(dir == 1){
			if(y > 0){
				if((maze[y-1])[x] != ' ') return explore(x,y-1,0);
			}
			if(y < (y_dim-1)){
				if((maze[y+1])[x] != ' ') return explore(x,y+1,2);
			}
		}
		if(dir == 2){
			if(x > 0){
				if((maze[y])[x-1] != ' ') return explore(x-1,y,3);
			}
			if(x < (x_dim-1)){
				if((maze[y])[x+1] != ' ') return explore(x+1,y,1);
			}
		}
		if(dir == 3){
			if(y > 0){
				if((maze[y-1])[x] != ' ') return explore(x,y-1,0);
			}
			if(y < (y_dim-1)){
				if((maze[y+1])[x] != ' ') return explore(x,y+1,2);
			}
		}
	}
	else{
		if(isalpha(curr)) printf("%c",curr);
		if(dir == 0){
			return explore(x,y-1,dir);
		}
		if(dir == 1){
			return explore(x+1,y,dir);
		}
		if(dir == 2){
			return explore(x,y+1,dir);
		}
		if(dir == 3){
			return explore(x-1,y,dir);
		}
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

    while(!feof(file)){
  		char ch = fgetc(file);
  		if(ch == '\n'){
			y_dim++;
		}
	}
	y_dim++; // off by one if we don't do this
	fseek(file, 0, SEEK_SET);
	read = getline(&line, &len, file);
	x_dim = strlen(line);
	
	init_maze();
	strcpy(maze[curr_idx],line);

	if(line){
		free(line);
		line = NULL;
	}
	curr_idx++;

	
	while ((read = getline(&line, &len, file)) != -1) {
		strcpy(maze[curr_idx],line);
		if(line){
			free(line);
			line = NULL;
		}
		curr_idx++;
	}

	if(line) free(line);
	fclose(file);

	//print_maze();
	int start_x = 0;
	int start_y = 0;
	char * itr = maze[0];
	while((*itr) != '\0'){
		if((*itr) == '|') break;

		start_x++;
		itr++;
	}
	//printf("Start: %d %d\n",start_x,start_y);
	explore(start_x,start_y,2);
	printf("\n Steps: %d\n",steps);

	destroy_maze();

	return 0;
}