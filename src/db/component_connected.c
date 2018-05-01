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


int component_connected_strong(char *grdbdir, int gidx, int cidx, vertexid_t id1, vertexid_t id2){
	return check_connection(grdbdir,gidx,cidx,id1,id2,0);
}

int component_connected_weak(char *grdbdir, int gidx, int cidx, vertexid_t id1, vertexid_t id2){
	return check_connection(grdbdir,gidx,cidx,id1,id2,1);
}
