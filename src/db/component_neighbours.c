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

void component_create_neighbours(char *grdbdir,int gidx,int cidx,vertexid_t vid){
	struct component c;
	int jump_flag1=0,jump_flag2=0;
	struct vertex_node *node;
	char s[BUFSIZE];
	int fd;
	off_t off;
	ssize_t len, size;
	vertexid_t id1, id2;
	int strong_neighbours[1024];
	int weak_neighbours[1024];
	char *buf=NULL;
	int readlen;
	int strong_count=0;
	int weak_count=0;
	FILE *fp_str,*fp_all;
	char buff[1024];
	char filename_str[1024];
	char filename_all[1024];
	//check if node exists	
	component_init(&c);
	c.efd = enum_file_open(grdbdir, gidx,cidx);
	if (c.efd >= 0) {
		enum_list_init(&(c.el));
		enum_list_read(&(c.el), c.efd);
	}
	// Load vertex schema 
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gidx, cidx);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		c.sv = schema_read(fd, c.el);
		close(fd);
	}
	// Load edge schema
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gidx, cidx);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		c.se = schema_read(fd, c.el);
		close(fd);
	}
	// Open vertex file 
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gidx, cidx);
	c.vfd = open(s, O_RDWR);
	// Open edge file 
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e", grdbdir, gidx, cidx);
	c.efd = open(s, O_RDWR);
	// Edges 
	if (c.se == NULL)
		size = 0;
	else
		size = schema_size(c.se);
	readlen = (sizeof(vertexid_t) << 1) + size;
	free(buf);
	buf = malloc(readlen);
	assert (buf != NULL);
	memset(buf, 0, readlen);
	sprintf(filename_str,"/tmp/%d%d%d_str.txt",gidx,cidx,(int)vid);
	sprintf(filename_all,"/tmp/%d%d%d_all.txt",gidx,cidx,(int)vid);
	fp_str=fopen(filename_str,"a");
	fp_all=fopen(filename_all,"a");
	fclose(fp_str);
	fclose(fp_all);
	//printf("\n*************File names created to store neighbours are*************\n%s\n%s\n",filename_str,filename_all);
	for (off = 0;; off += readlen) {
		lseek(c.efd, off, SEEK_SET);
		len = read(c.efd, buf, readlen);
		if (len <= 0)
			break;
		id1 = *((vertexid_t *) buf);
		id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));
		//printf("\n%llu,%llu\n", id1, id2);
		if(id1==vid) {
			fp_str=fopen(filename_str,"r");
			fp_all=fopen(filename_all,"r");
			if(fp_str==NULL || fp_all==NULL ){
				perror("fopen:");
			}
			jump_flag1=0;
			jump_flag2=0;
			while(!feof(fp_str)){
				char read_buffer[1024];
		        	fgets(read_buffer,1024,fp_str);
				//printf("Node str red is %s\n",read_buffer);
				if(((int)atoi(read_buffer))==(int)id2){
					jump_flag1=1;
					//printf("\nStrong node Entry is already appended in file\n");
					fclose(fp_str);
					break;
				}
			}
			while(!feof(fp_all)){
				char read_buffer[1024];
		        	fgets(read_buffer,1024,fp_all);
				//printf("Node all red is %s\n",read_buffer);
				if(((int)atoi(read_buffer))==(int)id2){
					jump_flag2=1;
					//printf("\nAll node Entry is already appended in file\n");
					fclose(fp_all);
					break;
				}
			}
			if(!jump_flag1){
				fclose(fp_str);
				fp_str=fopen(filename_str,"a");
				fprintf(fp_str,"%d\n",(int)id2);
				fclose(fp_str);
			}
			if(!jump_flag2) {
				fclose(fp_all);
				fp_all=fopen(filename_all,"a");
				fprintf(fp_all,"%d\n",(int)id2);
				fclose(fp_all);
			}
		}else if(id2==vid){
			fp_all=fopen(filename_all,"r");
			if(fp_all==NULL ){
				perror("fopen:");
			}
			jump_flag2=0;
			while(!feof(fp_all)){
				char read_buffer[1024];
		        	fgets(read_buffer,1024,fp_all);
				//printf("Node all red is %s\n",read_buffer);
				if((int)atoi(read_buffer)==(int)id1){
					jump_flag2=1;
					//printf("\nAll node Entry is already appended in file\n");
					fclose(fp_all);
					break;
				}
			}
			if(!jump_flag2){
				fclose(fp_all);
				fp_all=fopen(filename_all,"a");
				fprintf(fp_all,"%d\n",id1);
				fclose(fp_all);
			}
		}
	}
}

void component_neighbours(char *grdbdir,int gidx,int cidx,vertexid_t vid){
	char filename_str[1024];
	FILE *fp;
	char buf[1024];
	char *status;
	component_create_neighbours(grdbdir,gidx,cidx,vid);
	sprintf(filename_str,"/tmp/%d%d%d_str.txt",gidx,cidx,(int)vid);
	fp=fopen(filename_str,"r");
	fseek(fp,0,SEEK_SET);
	if(fp==NULL){
		printf("\nSorry!! couldn;t open file may be something wrong in code logic\n");
		return;
	}else{
		printf("\nNeighbours of the given node:");
		do{
			//int len=0;
			bzero(buf,1024);
			status=fgets(buf,1024,fp);
			//len=strlen(buf);
			//printf("Length of red line %d",len);
			printf("\n\t%s\n",buf);
		}while(status);
	}
}
