#include "ht.h"

t_hti ht_iterator(t_ht* table)
{
	t_hti it;
	it._table = table;
	it._index = 0;

	return (it);
}