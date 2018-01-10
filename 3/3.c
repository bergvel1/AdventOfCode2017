#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int ** gen_spiral(int size, int target, int* target_x, int* target_y){
	int ** spiral = malloc(size*sizeof(int*));

	for(int i = 0; i < size; i++){
		spiral[i] = malloc(size*sizeof(int));
	}

	// spiral start idxs
	int curr_x = size/2;
	int curr_y = size/2;
	int curr_num = 1;
	int direction = 0; // 0 = east, 1 = north, 2 = west, 3 = south
	int edge_len = 1; // current spiral edge length
	int edge_len_counter = edge_len;
	int edges_taken = 0; // number of edges processed at current length

	while(curr_num <= (pow(size,2))){
		//printf("[%d][%d] = %d\n", curr_x, curr_y, curr_num);
		spiral[curr_x][curr_y] = curr_num;
		if (curr_num == target){
			*target_x = curr_x;
			*target_y = curr_y;
		}

		if (edge_len_counter == 0){
			// switch directions
			direction = (direction+1)%4;
			//printf("Dir: %d, ET: %d\n",direction, edges_taken);

			if(edges_taken == 1){
				// increase edge_len
				edge_len++;
				edges_taken = 0;
				//printf("EL: %d\n",edge_len);
			}
			else edges_taken++;

			edge_len_counter = edge_len-1;
		}
		else{
			edge_len_counter--;
		}

		if(direction == 0)
			curr_x++;
		else if(direction == 1)
			curr_y--;
		else if(direction == 2)
			curr_x--;
		else if(direction == 3)
			curr_y++;

		curr_num++;
	}

	return spiral;
}

void print_spiral(int ** spiral, int size){
	for(int x = 0; x < size; x++){
		for(int y = 0; y < size; y++){
			printf("%d ",spiral[y][x]);
		}
		printf("\n");
	}
}

int descend(int ** spiral, int size, int x, int y){
	int curr_num = spiral[x][y];

	// base case
	if(curr_num == 1)
		return 0;

	int e_num = curr_num;
	int n_num = curr_num;
	int w_num = curr_num;
	int s_num = curr_num;

	if(x < (size-1))
		e_num = spiral[x+1][y];
	if(y > 0)
		n_num = spiral[x][y-1];
	if(x > 0)
		w_num = spiral[x-1][y];
	if(y < (size-1))
		s_num = spiral[x][y+1];

	
	if ((e_num < n_num) && (e_num < w_num) && (e_num < s_num))
		return 1+descend(spiral,size,x+1,y);
	if ((n_num < e_num) && (n_num < w_num) && (n_num < s_num))
		return 1+descend(spiral,size,x,y-1);
	if ((w_num < e_num) && (w_num < n_num) && (w_num < s_num))
		return 1+descend(spiral,size,x-1,y);
	if ((s_num < e_num) && (s_num < n_num) && (s_num < w_num))
		return 1+descend(spiral,size,x,y+1);

	printf("Descent error!\n");
	exit(1);
}

int main(int argc, char* argv[]){
	if (argc < 3)
		return -1;

	int size = atoi(argv[1]);
	int target_num = atoi(argv[2]);

	if (size % 2 == 0)
		return -1; //no even side lengths allowed!

	int target_num_x;
	int target_num_y;
	int ** spiral = gen_spiral(size, target_num, &target_num_x, &target_num_y);

	//print_spiral(spiral,size);
	printf("Target %d at [%d][%d]\n",target_num,target_num_x,target_num_y);
	int steps = descend(spiral,size,target_num_x,target_num_y);

	printf("%d steps\n",steps);

	// memory cleanup
	for(int i = 0; i < size; i++){
		free(spiral[i]);
	}
	free(spiral);

	return 0;
}