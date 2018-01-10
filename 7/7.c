#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH (15)
#define DEBUG (0)

typedef struct node_s {
	char * name;
	int weight;
	int tower_weight;
	int num_children;
	char ** child_names;
	struct node_s * parent;
} node_t;

node_t * nodes = NULL;
int count = 0;

// data string example: "fwft (72) -> ktlj, cntj, xhth"
void init_node(node_t * node, char * data){
	node->parent = NULL;

	// determine number of child nodes
	char * data_itr = data;
	int children;
	for (children=0; data_itr[children]; (data_itr[children]==',' || data_itr[children]=='>') ? children++ : *data_itr++); // https://stackoverflow.com/a/4235884 [black magic!]
	node->num_children = children;

	// node name
	node->name = malloc(MAX_NAME_LENGTH);
	char * buffer = strtok(data, " ");
	//printf("1: %s\n",buffer);
	strcpy(node->name,buffer);

	// node weight
	buffer = strtok(NULL, ")");
	node->weight = atoi(buffer+1);
	node->tower_weight = -1; // placeholder
	//printf("2: %d\n",node->weight);

	// if there are no children, we're done
	if(node->num_children == 0){
		if (DEBUG) printf("\t%s (%d)\n",node->name,node->weight);
		node->child_names = NULL;
		return;
	} 

	// move past " -> " (if it exists)
	if(!strtok(NULL, " "))
		return;

	node->child_names = malloc(node->num_children*sizeof(char*));

	// child names
	int i;
	for(i = 0; i < (node->num_children); i++){
		//printf("3pre_%d: %s\n",i,buffer);
		buffer = strtok(NULL, ",");
		//printf("3post_%d: %s\n",i,buffer+((i==0) ? 0 : 1));
		node->child_names[i] = malloc(MAX_NAME_LENGTH);
		strcpy(node->child_names[i],buffer+((i==0) ? 0 : 1)); // bad hack to trim front whitespace...

		int len = strlen(node->child_names[i]);	// ...and another to trimp newlines from the rear
		if (len > 0 && node->child_names[i][len-1] == '\n') node->child_names[i][--len] = '\0';
	}

	if(DEBUG){
		printf("\t%s (%d) ",node->name,node->weight);

		for(int i = 0; i < node->num_children; i++){
			printf("%s ",node->child_names[i]);
		}

		printf("\n");
	}
}

void free_node(node_t * node){
	free(node->name);

	for(int i = 0; i < node->num_children; i++){
		free(node->child_names[i]);
	}

	free(node->child_names);
}

node_t * find_node(char * name){
	for(int i = 0; i < count; i++){
		node_t * curr_node = &nodes[i];
		int res = strcmp(name,curr_node->name);
		if(res == 0)
			return(curr_node);
	}

	printf("find error!\n");
	return NULL; // not found
}

void print_nodes(){
	for(int i = 0; i < count; i++){
		node_t * node = &nodes[i];
		printf("%s (%d,%d) ",node->name,node->weight,node->tower_weight);

		for(int i = 0; i < node->num_children; i++){
			printf("%s ",node->child_names[i]);
		}

		if(node->parent) printf("[%s]",node->parent->name);

		printf("\n");
	}
}

void connect_nodes(){
	for(int i = 0; i < count; i++){
		node_t * curr_node = &nodes[i];

		for(int j = 0; j < curr_node->num_children; j++){
			node_t * child = find_node(curr_node->child_names[j]);

			child->parent = curr_node;
		}
	}
}

node_t * find_root(int curr_idx){
	if(curr_idx >= count)
		return NULL; // cycle detected (no root)

	node_t * curr_node = &nodes[curr_idx];

	if (curr_node->parent == NULL)
		return curr_node;

	else return find_root(curr_idx+1);
}

int get_set_tower_weight(node_t * node){
	int ret = node->weight;

	for(int i = 0; i < node->num_children; i++){
		node_t * child = find_node(node->child_names[i]);
		ret += get_set_tower_weight(child);
	}

	node->tower_weight = ret;
	return ret;
}

int check_weight(node_t * node){
	if (node->num_children < 2){
		return 0; // can't be unbalanced with less than 2 children
	}

	if (node->num_children == 2){
		node_t * l = find_node(node->child_names[0]);
		node_t * r = find_node(node->child_names[1]);

		if (l->tower_weight == r->tower_weight)
			return 0; // balanced!

		int l_ret = check_weight(l);
		int r_ret = check_weight(r);

		return ((l_ret > r_ret) ? l_ret : r_ret);
	}

	node_t * unbalanced = NULL; // the problem node
	for(int i = 0; i < node->num_children; i++){
		node_t * curr_node_1 = find_node(node->child_names[i]);

		for(int j = i+1; j < node->num_children; j++){
			node_t * curr_node_2 = find_node(node->child_names[j]);
			if(curr_node_1->tower_weight != curr_node_2->tower_weight){
				unbalanced = curr_node_1;
				break;
			}
		}
	}

	// check to make sure last child isn't the problem
	node_t * second_last = find_node(node->child_names[node->num_children-2]);
	if(unbalanced == second_last){
		node_t * first = find_node(node->child_names[0]);
		node_t * last = find_node(node->child_names[node->num_children-1]);
		if (first->tower_weight != last->tower_weight){
			unbalanced = last;
		}
	}

	if (!unbalanced) return 0;
	//printf("Unbalanced: %s\n",unbalanced->name);

	// need to determine if the imbalance is due to a child or the node's own weight
	int ret = check_weight(unbalanced);

	if(!ret){
		int idx = ((strcmp(unbalanced->name,node->child_names[0]))==0) ? 1 : 0;
		node_t * other = find_node(node->child_names[idx]);
		int tower_weight_diff = unbalanced->tower_weight - other->tower_weight;
		ret = unbalanced->weight - tower_weight_diff;

		printf("%s needs to be %d!\n", unbalanced->name,ret);
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

    while(!feof(file)){
  		char ch = fgetc(file);
  		if(ch == '\n'){
			count++;
		}
	}

	nodes = malloc(count*sizeof(node_t));
	int curr_idx = 0;
	fseek(file, 0, SEEK_SET);

    while ((read = getline(&line, &len, file)) != -1) {
    	node_t * curr_node = &nodes[curr_idx];
    	init_node(curr_node,line);

    	//printf("%d\n", numbers[curr_idx]);
    	curr_idx++;
    	if(line){
    		free(line);
    		line = NULL;
    	}
    }

    fclose(file);
	if (line)
        free(line);

    connect_nodes();

    node_t * root = find_root(0);
    printf("root: %s weight: %d\n",root->name,get_set_tower_weight(root));

    print_nodes();
    check_weight(root);

    for(int i = 0; i < count; i++){
    	free_node(&nodes[i]);
    }

  	free(nodes);
	return 0;
}