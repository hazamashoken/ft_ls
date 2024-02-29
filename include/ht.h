#ifndef HT_H
# define HT_H

#include <stdint.h>
#include <unistd.h>

#include "libft.h"

# ifndef HT_INITIAL_CAPACITY
#  define HT_INITIAL_CAPACITY 16
# endif

# define FNV_OFFSET 14695981039346656037UL
# define FNV_PRIME 1099511628211UL

typedef struct s_ht_entry {
	const char *key;
	void *value;
}	t_ht_entry;


typedef struct s_ht {
	t_ht_entry * entries;
	size_t  capacity;
	size_t  size;
}	t_ht;

typedef struct s_hti {
	const char* key;
	void *value;
	t_ht* _table;
	size_t _index;
}	t_hti;

t_ht*		ht_create(void);
void		ht_destory(t_ht* table, void free_value(void *));
void		*ht_get(t_ht* table, const char * key);
const char*	ht_set(t_ht *table, const char * key, void* value);
size_t		ht_length(t_ht* table);
t_hti		ht_iterator(t_ht* table);
bool		ht_next(t_hti* it);

#endif