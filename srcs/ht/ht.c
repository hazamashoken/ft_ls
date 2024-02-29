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

static uint64_t hash_key(const char* key)
{
	uint64_t hash;
	const char *p;

	hash = FNV_OFFSET;
	p = key;
	while (*p)
	{
		hash ^= (uint64_t)(unsigned char)(* p);
		hash *= FNV_PRIME;
		++p;
	}

	return (hash);

}
void *ht_get(t_ht* table, const char * key)
{
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

	while (table->entries[index].key != NULL)
	{
		if (ft_strcmp(key, table->entries[index].key) == 0)
			return (table->entries[index].value);
		++index;
		if (index >= table->capacity)
		{
			index = 0;
		}
	}
	return (NULL);
}

static const char* ht_set_entry(t_ht_entry* entries, size_t capacity, const char* key, void* value, size_t* psize)
{
	uint64_t	hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

	while(entries[index].key != NULL)
	{
		if (ft_strcmp(key, entries[index].key) == 0)
		{
			entries[index].value = value;
			return (entries[index].key);
		}

		++index;
		if (index >= capacity)
			index = 0;
	}

	if (psize != NULL)
	{
		key = ft_strdup(key);
		if (key == NULL)
		{
			return (NULL);
		}
		(*psize)++;
	}
	entries[index].key = (char *)key;
	entries[index].value = value;
	return (key);
}

static bool ht_expand(t_ht* table)
{
	size_t new_capacity;
	t_ht_entry* new_entries;
	size_t i;

	new_capacity = table->capacity * 2;
	if (new_capacity < table->capacity)
	{
		return (false);
	}
	new_entries = ft_calloc(new_capacity, sizeof(t_ht_entry));
	if( new_entries == NULL)
	{
		return (false);
	}
	i = 0;
	while(i < table->capacity)
	{
		t_ht_entry entry = table->entries[i];
		if (entry.key != NULL)
			ht_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
		i++;
	}
	free(table->entries);
	table->entries = new_entries;
	table->capacity = new_capacity;

	return (true);
}

const char* ht_set(t_ht *table, const char * key, void* value)
{
	if (value == NULL)
		return (NULL);

	if (table->size >= table->capacity / 2)
	{
		if (!ht_expand(table))
			return (NULL);
	}

	return ht_set_entry(table->entries, table->capacity, key, value, &table->size);
}

size_t ht_length(t_ht* table)
{
	return (table->size);
}

t_hti ht_iterator(t_ht* table)
{
	t_hti it;
	it._table = table;
	it._index = 0;

	return (it);
}

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