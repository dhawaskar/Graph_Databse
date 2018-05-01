#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_component_connected(char *cmdline, int *pos)
{
	
	char *temp,*cmd;
	int flag_str=0;
	int flag_weak=0;
	int gidx,cidx;
	vertexid_t vid1,vid2;
	if(strlen(cmdline)<strlen("g nei 0 0 1")){
		printf("\nWrong command usage\ncorrect usage g nei gno cno vno\n");
		return;
	}
	temp=strtok(cmdline," ");
	cmd=strtok(NULL," ");
	temp=strtok(NULL," ");
	gidx=atoi(temp);
	temp=strtok(NULL," ");
	cidx=atoi(temp);
	temp=strtok(NULL," ");
	vid1=(vertexid_t)atoi(temp);
	temp=strtok(NULL," ");
	vid2=(vertexid_t)atoi(temp);
	if(strcmp(cmd,"str")==0){
	       flag_str=component_connected_strong(grdbdir,gidx,cidx,vid1,vid2);	
	       if(flag_str) printf("Strong connected path\n");
	       else printf("No strong path exists\n");
	       return;
	}
	else if(strcmp(cmd,"weak")==0) {
		flag_weak=component_connected_weak(grdbdir,gidx,cidx,vid1,vid2);
		if(flag_weak) printf("Yes connected weakly\n");
		else printf("They are not even connected\n");
		return;
	}
	else{
		printf("Wrong usage of command\n");
		return ;
	}
}

