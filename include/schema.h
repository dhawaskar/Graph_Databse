#ifndef __SCHEMA_H
#define __SCHEMA_H


#include "enum.h"

#define ATTR_NAME_MAXLEN	256

enum schema_type { VERTEX, EDGE };

typedef enum schema_type schema_type_t;

/* Base types */
enum base_types
{
	CHARACTER,	/* Fixed length character string */
	VARCHAR,	/* Variable length character string */
	BOOLEAN,	/* Boolean */
	ENUM,		/* Enum type */
	INTEGER,	/* Integer */
	FLOAT,		/* Float */
	DOUBLE,		/* Double */
	DATE,		/* Date string */
	TIME,		/* Time string (24 hour format) */
	BASE_TYPES_MAX
};

struct attribute {
	char name[ATTR_NAME_MAXLEN];	/* Name */
	enum_t e;			/* enum_t if ENUM */
	enum base_types bt;		/* Base type */
	struct attribute *next;
};

struct schema {
	struct attribute *attrlist;		/* Attributes list */
};

typedef enum base_types base_types_t;
typedef struct attribute *attribute_t;
typedef struct schema *schema_t;

extern char *base_types_str[];
extern int base_types_len[];

void schema_attribute_init(
        attribute_t attr,
        void *name,
        enum base_types bt,
        enum_t e);

void schema_attribute_print(
	FILE *out, attribute_t attr, enum_list_t el);
void schema_attribute_insert(schema_t s, attribute_t attr);
void schema_attribute_remove(schema_t s, attribute_t attr);
attribute_t schema_attribute_last(schema_t s);

void schema_init(schema_t *s);
int schema_file_init(int gidx, int cidx, char *name);
int schema_count(schema_t s);
int schema_size(schema_t s);
base_types_t schema_find_type_by_name(schema_t s, char *name);
attribute_t schema_find_attr_by_name(schema_t s, char *name);
void schema_print(FILE *out, schema_t s, enum_list_t el);

schema_t schema_read(int fd, enum_list_t el);
schema_t schema_write(schema_t s, int fd, int append);

int
schema_union(
        schema_type_t st,
        schema_t s1,
        schema_t s2,
        char *grdbdir,
        int gno,
        int cidx);


#endif
