#ifndef __ENUMERATION_H
#define __ENUMERATION_H

#include <string.h>
#include "string_pool.h"

#define ENUM_NAME_LEN	256

struct grdb_enum {
	char name[ENUM_NAME_LEN];
	string_pool_t pool;
	struct grdb_enum *next;
};

typedef struct grdb_enum *enum_t;
typedef enum_t enum_list_t;

void enum_init(enum_t *e);
int enum_file_open(char *grdbdir, int gidx, int cidx);
void enum_print(FILE *out, enum_t e);
void enum_insert(enum_t *e, char *s);
char *enum_find_by_idx(enum_t e, int idx);
int enum_find_idx_by_name(enum_t e, char *s);

void enum_set_name(enum_t e, char *name);
char *enum_get_name_ptr(enum_t e);

void enum_list_init(enum_list_t *el);
int enum_list_count(enum_list_t el);
void enum_list_print(FILE *out, enum_list_t el);
void enum_list_insert(enum_list_t *el, enum_t e);
enum_t enum_list_find_by_name(enum_list_t el, char *name);
int enum_list_find_idx_by_name(enum_list_t el, char *name);
enum_t enum_list_find_by_idx(enum_list_t el, int idx);
enum_list_t enum_list_read(enum_list_t *el, int fd);
enum_list_t enum_list_write(enum_list_t el, int fd);

int
enum_list_union(
        enum_list_t el1,
        enum_list_t el2,
        char *grdbdir,
        int gno,
        int cidx);


#endif
