#include "ht.h"

bool ht_next(t_hti* it)
{
	t_ht* table;
	size_t i;

	table = it->_table;
	while (it->_index < table->capacity)
	{
		i = it->_index;
		it->_index++;
		if (table->entries[i].key != NULL)
		{
			t_ht_entry entry = table->entries[i];
			it->key = entry.key;
			it->value = entry.value;
			return true;
		}
	}
	return false;
}