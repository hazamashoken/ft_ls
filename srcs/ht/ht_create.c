#include "ht.h"

t_ht* ht_create(void)
{
	t_ht * table = malloc(sizeof(t_ht));
	if (table == NULL)
		return (NULL);
	table->size = 0;
	table->capacity = HT_INITIAL_CAPACITY;

	table->entries = ft_calloc(table->capacity, sizeof(t_ht_entry));
	if (table->entries == NULL)
	{
		free(table);
		return (NULL);
	}
	return (table);
}