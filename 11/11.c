#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if (argc < 2)
		return -1; // no input given

	char * input = argv[1];

	int n = 0, s = 0, ne = 0, nw = 0, se = 0, sw = 0, dist = 0, max_dist = 0;;

	char * curr_dir = strtok(input,",");

	while(curr_dir){
		if(strcmp(curr_dir,"n") == 0){
			if(s > 0){
				s--;
				dist--;
			}
			else if((sw > 0) && (se > 0)){
				if(sw >= se){
					sw--;
					nw++;
				}
				else if (se > sw){
					se--;
					ne++;
				}
			}
			else if(sw > 0){
				sw--;
				nw++;
			}
			else if(se > 0){
				se--;
				ne++;
			}

			else{
				n++;
				dist++;
			}
		}
		if(strcmp(curr_dir,"s") == 0){
			if(n > 0){
				n--;
				dist--;
			}
			else if((nw > 0) && (ne > 0)){
				if(nw >= ne){
					nw--;
					sw++;
				}
				else if (ne > nw){
					ne--;
					se++;
				}
			}
			else if(nw > 0){
				nw--;
				sw++;
			}
			else if(ne > 0){
				ne--;
				se++;
			}
			else{
				s++;
				dist++;	
			}
		}

		if(strcmp(curr_dir,"ne") == 0){
			if(sw > 0){
				sw--;
				dist--;
			}

			else if((nw > 0) && (s > 0)){
				if(nw >= s){
					nw--;
					n++;
				}
				else if (s > nw){
					s--;
					se++;
				}
			}
			else if(nw > 0){
				nw--;
				n++;
			}
			else if(s > 0){
				s--;
				se++;
			}
			
			else{
				ne++;
				dist++;
			}

		}
		if(strcmp(curr_dir,"nw") == 0){
			if(se > 0){
				se--;
				dist--;
			}

			else if((ne > 0) && (s > 0)){
				if(ne >= s){
					ne--;
					n++;
				}
				else if (s > ne){
					s--;
					sw++;
				}
			}
			else if(ne > 0){
				ne--;
				n++;
			}
			else if(s > 0){
				s--;
				sw++;
			}
			
			else{
				nw++;
				dist++;
			}
			

		}
		if(strcmp(curr_dir,"se") == 0){
			if(nw > 0){
				nw--;
				dist--;
			}

			else if((sw > 0) && (n > 0)){
				if(sw >= n){
					sw--;
					s++;
				}
				else if (n > sw){
					n--;
					ne++;
				}
			}
			else if(sw > 0){
				sw--;
				s++;
			}
			else if(n > 0){
				n--;
				ne++;
			}
			
			else{
				se++;
				dist++;
			}

		}
		if(strcmp(curr_dir,"sw") == 0){
			if(ne > 0){
				ne--;
				dist--;
			}

			else if((se > 0) && (n > 0)){
				if(se >= n){
					se--;
					s++;
				}
				else if (n > se){
					n--;
					nw++;
				}
			}
			else if(se > 0){
				se--;
				s++;
			}
			else if(n > 0){
				n--;
				nw++;
			}
			
			else{
				sw++;
				dist++;
			}

		}

		// clear up "opposites" introduced above? (e.g. if (n == s) then n = s = 0?)
		// if doing this, remember to remove "=" from conditionals above

		if(dist > max_dist){
			max_dist = dist;
		}

		curr_dir = strtok(NULL,",");
	}

	int total = n + s + ne + nw + se + sw;
	printf("n: %d\ns: %d\nne: %d\nnw: %d\nse: %d\nsw: %d\ntotal: %d\ndistance: %d\n",n,s,ne,nw,se,sw,total,max_dist);


}