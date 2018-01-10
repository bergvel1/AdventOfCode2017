#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define GEN_A_START 703 //65
#define GEN_B_START 516 //8921

#define GEN_A_FACTOR 16807
#define GEN_B_FACTOR 48271

#define MOD_FACTOR 2147483647
#define NUM_ITER 5000000

int compare(int64_t a, int64_t b){
	if(((a << 48) >> 48) == ((b << 48) >> 48)) return 1;
	else return 0;
}

int main(){
	int64_t gen_a = GEN_A_START;
	int64_t gen_b = GEN_B_START;

	int64_t to_judge_a = 0;
	int64_t to_judge_b = 0;

	//uint32_t a_itr = 0;
	//uint32_t b_itr = 0;
	uint32_t itr = 0;
	uint32_t judge = 0;


	/* PART ONE
	//printf("--Gen. A--  --Gen. B--\n");
	for(int i = 0; i < NUM_ITER; i++){
		gen_a = (gen_a * GEN_A_FACTOR) % MOD_FACTOR;
		gen_b = (gen_b * GEN_B_FACTOR) % MOD_FACTOR;
		judge += compare(gen_a,gen_b);

		//printf("%lld %lld\n",gen_a,gen_b);
	}*/

	while(/*(a_itr < NUM_ITER) || (b_itr < NUM_ITER)*/ itr < NUM_ITER){
		if(!to_judge_a){
			gen_a = (gen_a * GEN_A_FACTOR) % MOD_FACTOR;
			//a_itr++;

			if((gen_a % 4) == 0) to_judge_a = gen_a;
		}
		if(!to_judge_b){
			gen_b = (gen_b * GEN_B_FACTOR) % MOD_FACTOR;
			//b_itr++;

			if((gen_b % 8) == 0) to_judge_b = gen_b;
		}

		if(to_judge_a && to_judge_b){
			judge += compare(to_judge_a,to_judge_b);
			itr++;
			to_judge_a = 0;
			to_judge_b = 0;
		}
	}

	printf("Final count: %u\n",judge);
}