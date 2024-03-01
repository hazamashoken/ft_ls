#include "ht.h"

void    ht_destory(t_ht* table, void free_value(void *))
{
	size_t i;

	i = 0;

	while (i < table->capacity)
	{
		free((void *)table->entries[i].key);
		free_value(table->entries[i].value);
		++i;
	}
	free(table->entries);
	free(table);
}