#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"
#include "graph.h"
#include "tuple.h"
#include <unistd.h>

// Save id as a new line into the visited.txt file.
// Increment visited index.
// Update visited index.
void save_into_visited(int id,int*visited_len){
	FILE *fp;
	char str[1024];
	fp = fopen("/tmp/visited.txt","a");
	if(fp == NULL){
		perror("Error in save_into_visited: ");		
		return;
	}
	fprintf(fp,"%d\n",id);
	fclose(fp);
	*visited_len = *visited_len+1;
	return;
}


// Check whether this id is in visited.txt file.
// If exist : return 1.
// Otherwise : return 0.
int whether_visited(int id){
	FILE *fp;
	char str[1024];
	fp = fopen("/tmp/visited.txt","r");
	if(fp == NULL){
		perror("Error in whether_visited: ");
		return 0;
	}
	while(!feof(fp)){
		fgets(str,1024,fp);
		if(id == atoi(str)){
			//printf("Id found in visited!\n");
			fclose(fp);
			return 1;
		}
	}			
	fclose(fp);
	return 0;
}


// Save id as a new line into the stack.txt file.
// Increment stack index.
// Update stack index.
void stack_push(int new_id,int*current_stack_index,int*current_id,int*visited_len){
	
	save_into_visited(new_id,visited_len);

	*current_id = new_id;
	FILE *fp;
	char str[1024];
	fp = fopen("/tmp/stack.txt","a");
	sprintf(str,"%d",new_id);
	if(fp == NULL){
		perror("Error in stack_push: ");		
		return;
	}
	fprintf(fp,"%s\n",str);
	fclose(fp);
	*current_stack_index = *current_stack_index+1;
	return;
}


// Copy old stack.txt into a new file without the last id(line).
// Replace stack.txt with new file.
// Decrement stack index.
// Update stack index.
void stack_pop(int*current_stack_index,int*current_id){
	FILE *fp_old;
	FILE *fp_new;
	char str[1024];
	fp_old = fopen("/tmp/stack.txt","r");
	fp_new = fopen("/tmp/tmp.txt","a");
	if(fp_old == NULL || fp_new == NULL){
		perror("Error in stack_pop: ");
		return;
	}
	for(int i=0;i<=*current_stack_index;i++){
		fgets(str,1024,fp_old);
		fprintf(fp_new, "%s",str);
		if(i==*current_stack_index-1){
			*current_id = atoi(str);
			break;
		}
	}
	fclose(fp_old);
	fclose(fp_new);
	if(remove("/tmp/stack.txt")!=0 || rename("/tmp/tmp.txt","/tmp/stack.txt")){
		perror("Error in stack_pop deleting and renaming file : ");
		return;
	}
	*current_stack_index = *current_stack_index-1;
	return;
}


// Run DFS.
// Output is an array of all of the nodes that can be strongly reached from our initial node.
// Initial node must have been set up before calling this function.
void dfs(char *grdbdir, int gidx, int cidx,int number_of_vertex,int*current_stack_index,int*current_id,int*visited_len,int neighbour_type){

	// si is the index of our current node in stack.
	// reaching -1 means we are finished.
	if(*current_stack_index==-1){return;}

	// Initialize Current node's neighbours information.
	int all_neighbours_visited = 1;
	FILE *fp_nei;
	char nei[1024];
	char filename_nei[1024];
	
	// Check whether we want do strong or weak based on neighbour_type.
	if(neighbour_type == 0){
		sprintf(filename_nei,"/tmp/%d%d%d_str.txt",gidx,cidx,*current_id);
	}else if(neighbour_type == 1){
		sprintf(filename_nei,"/tmp/%d%d%d_all.txt",gidx,cidx,*current_id);
	}else{
		perror("Neighbour_type must be 0 or 1!: ");
		return;
	}

	component_create_neighbours(grdbdir,gidx,cidx,*current_id);

	// Check all neighbours of current id.
	fp_nei=fopen(filename_nei,"r");
	while(!feof(fp_nei)){
		fgets(nei,1024,fp_nei);
		int nei_value = atoi(nei);
		if(nei_value == 0){break;}
		if(whether_visited(nei_value)==0){
			all_neighbours_visited = 0;
			//printf("\n                       neighbour %d has not been visited so we push stack\n",nei_value);
			stack_push(nei_value,current_stack_index,current_id,visited_len);
			fclose(fp_nei);
			return dfs(grdbdir,gidx,cidx,number_of_vertex,current_stack_index,current_id,visited_len,neighbour_type);
		}
	}
	fclose(fp_nei);

	// Current node's all neighbours have been visited.
	if(all_neighbours_visited == 1){
		//printf("\n                     %d 's all neighbours have been visited soo we pop stack\n", *current_id);
		stack_pop(current_stack_index,current_id);
		return dfs(grdbdir,gidx,cidx,number_of_vertex,current_stack_index,current_id,visited_len,neighbour_type);
	}
	return;
}


int check_connection(char *grdbdir, int gidx, int cidx, vertexid_t vid1, vertexid_t vid2,int neighbour_type){

	int id1=(int)vid1;
	int id2=(int)vid2;

	// Initialization of variables that will be used by DFS to store values.
	int number_of_vertex=1024;
	remove("/tmp/stack.txt");
	remove("/tmp/visited.txt");
	int current_stack_index = -1;
	int current_id;
	int visited_len = 0;
	stack_push(id1,&current_stack_index,&current_id,&visited_len);
	
	// Call DFS function. which will fill our variables with results.
	dfs(grdbdir,gidx,cidx,number_of_vertex,&current_stack_index,&current_id,&visited_len,neighbour_type);

	// Check whether id2 is in the path(visited.txt).
	if(whether_visited(id2)==1){
		return 1;
	}
	return 0;
}





