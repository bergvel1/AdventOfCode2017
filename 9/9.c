#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given

	char * input_itr = argv[1];
	int score = 0;
	int garbage_chars = 0;
	int curr_depth = 0;
	int in_garbage_flag = 0;
	int cancel_flag = 0; // skip character when == 1 and in garbage

	printf("%s\n",input_itr);

	while(*input_itr){
		char curr_char = *(input_itr);

		if(cancel_flag && in_garbage_flag){
			cancel_flag = 0;
		}
		else{
			if(in_garbage_flag){
				if(curr_char == '!'){
					cancel_flag = 1;
				}
				else if(curr_char == '>'){
					in_garbage_flag = 0;
				}
				else{
					garbage_chars++;
				}
			}
			else{
				if(curr_char == '<'){
					in_garbage_flag = 1;
				}
				else{
					if(curr_char == '{'){
						curr_depth++;
					}
					if(curr_char == '}'){
						score += curr_depth;
						curr_depth--;
					}
				}
			}
		}

		input_itr++;
	}

	printf("Score: %d\nGarbage chars: %d\n",score,garbage_chars);

	return 0;
}