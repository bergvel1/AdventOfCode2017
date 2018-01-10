#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int process1 (int* input_ints, size_t len){
	int acc = 0;

	if (input_ints[0] == input_ints[len-1])
		acc += input_ints[0];

	for(size_t i = 0; i < (len-1); i++){
		if(input_ints[i] == input_ints[i+1])
			acc += input_ints[i];
	}

	return acc;
}

int process2 (int* input_ints, size_t len){
	int acc = 0;

	for(size_t i = 0; i < len; i++){
		if(input_ints[i] == input_ints[(i+(len/2))%len])
			acc += input_ints[i];
	}

	return acc;
}

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given

	char* input_str = argv[1];
	size_t len = strlen(input_str);
	int* input_ints = malloc(len*sizeof(int));

	for(size_t i = 0; i < len; i++){
		input_ints[i] = input_str[i] - '0';
	}

	int ret = process2(input_ints,len);

	free(input_ints);

	printf("Result: %d\n",ret);
	return 0;
}