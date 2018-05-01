#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_component_neighbours(char *cmdline, int *pos)
{
	char *temp;
	int gidx,cidx;
	vertexid_t vid;
	if(strlen(cmdline)<strlen("g nei 0 0 1")){
		printf("\nWrong command usage\ncorrect usage g nei gno cno vno\n");
		return;
	}
	temp=strtok(cmdline," ");
	temp=strtok(NULL," ");
	temp=strtok(NULL," ");
	gidx=atoi(temp);
	temp=strtok(NULL," ");
	cidx=atoi(temp);
	temp=strtok(NULL," ");
	vid=(vertexid_t)atoi(temp);
	component_neighbours(grdbdir,gidx,cidx,vid);	
}
