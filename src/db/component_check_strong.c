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


struct vertex_node * get_node_strong(vertexid_t vid){
	struct vertex_node *ptr;
	if(header==NULL){
		return NULL;
	}else{
		ptr=header;
		while(ptr!=NULL){
			if(ptr->vid==vid) return ptr;
			ptr=ptr->next;			
		}
		return NULL;
	}
}


// For a given node, check whether it has been visited(check whether it is in vertex_visited).
int whether_visited_strong(int id, int vertex_visited[], int number_of_vertex){
	for(int i=0;i<number_of_vertex;i++){
		if(id==vertex_visited[i]){
			return 1;
		}
	}
	return 0;
}



// Run DFS.
// Output is an array of all of the nodes that can be strongly reached from our initial node.
// Initial node must have been set up before calling this function.
// In our case. it is stack[si] which would be stack[0].
void dfs_strong(char *grdbdir, int gidx, int cidx,int stack[],int vertex_visited[],int vi,int si,int number_of_vertex){


	// Helper commands that can print out the current node information
	
	printf("                                                -------------------\n");
	printf("                                                The current index(vi) of vertex_visited is : %d\n", vi);
	printf("                                                The current index(si) of stack is: %d\n", si);
	printf("                                                The curent item from vertex_visited is: %d\n", vertex_visited[vi]);
	printf("                                                The current item from stack is : %d\n", stack[si]);
	printf("                                                ===================\n");
	

	// si is the index of our current node in stack.
	// reaching -1 means we are finished.
	if(si==-1){return;}

	// Get the Current node's neighbours information.Using Sandesh code.
	struct vertex_node * node;
	node=get_node_strong(stack[si]);
	if(node==NULL){
		component_neighbours(grdbdir,gidx,cidx,stack[si]);
		node=get_node_strong(stack[si]);
		if(node==NULL){
			printf("No use sandesh");
			return;
		}
	}
	int number_of_neighours=node->str_count;
	int *neighbours=node->strong_nei;

	// Variable indicating whether all neighbour of current node has been visited.
	int all_neighbours_visited = 1;

	// Current node has no neighbours other than the previous node.
	if(number_of_neighours==0){
		printf("\n                                   %d has no other neighbours other than previous node.pop stack\n",stack[si]);
		si--;
		neighbours=NULL;
		return dfs_strong(grdbdir,gidx,cidx,stack,vertex_visited,vi,si,number_of_vertex);
	}
	
	//	Current node has a neighbour that has not been visited.
	for(int i=0;i<number_of_neighours;i++){	
		if(whether_visited_strong(neighbours[i],vertex_visited,number_of_vertex)==0){
			all_neighbours_visited = 0;
			printf("\n                                         neighbour %d has not been visited.push stack\n",neighbours[i]);
			vi++;
			vertex_visited[vi] = neighbours[i];
			si++;
			stack[si] = neighbours[i];
			neighbours=NULL;
			return dfs_strong(grdbdir,gidx,cidx,stack,vertex_visited,vi,si,number_of_vertex);
		}
	}

	// Current node's all neighbours have been visited.
	if(all_neighbours_visited == 1){
		printf("\n                                                %d 's all neighbours have been visited.pop stack\n", stack[si]);
		si--;
		neighbours=NULL;
		return dfs_strong(grdbdir,gidx,cidx,stack,vertex_visited,vi,si,number_of_vertex);
	}
	neighbours=NULL;
	return;
	
}



int check_connection_strong(char *grdbdir, int gidx, int cidx, vertexid_t vid1, vertexid_t vid2){


	int id1=(int)vid1;
	int id2=(int)vid2;

	// Initialization of variables that will be used by DFS to store values.
	int number_of_vertex=1024;
	int vertex_visited[number_of_vertex];
	int stack[number_of_vertex];
	for(int i=0;i<number_of_vertex;i++){
		vertex_visited[i]=0;
		stack[i]=0;
	}
	int vi = 0;
	int si = 0;
	vertex_visited[vi] = id1;
	stack[si] = id1;//This will be what we need: All nodes that we can reach(the DFS path).
	
	
	// Call DFS function. which will fill our variables with results.
	dfs_strong(grdbdir,gidx,cidx,stack,vertex_visited,vi,si,number_of_vertex);

	// Our results: The DFS path.
	printf("All nodes that we can reach from %d:  ",id1);
	/*for(int i=0;i<number_of_vertex;i++){
		printf("%d  ", vertex_visited[i]);
	}*/

	// Check if we can find id2 in our DFS path.
	for(int i=0;i<number_of_vertex;i++){
		if(vertex_visited[i]==id2){
			printf("\n%d and %d are connected !\n",id1,id2);
			return 1;
		}
	}
	printf("\n%d and %d are NOT connected strong!\n",id1,id2);
	return 0;
}





