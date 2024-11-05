#include "EntitiesPool.h"
EntitiesPool::EntitiesPool()
{
}

void EntitiesPool::insert(EntityPtr whom)
{
	_pool.push_back(whom);
	_isInUse.push_back(false);

}

EntityPtr EntitiesPool::getNext()
{
	for (int i = 0; i < _pool.size(); i++)
	{
		if (_isInUse[i] == false)
		{
			_isInUse[i] = true;
			return _pool[i];
		}
	}
	throw("exception  all entities are in use!!");
}

void EntitiesPool::returnEntity(EntityPtr toFree)
{
	int i = 0;
	for (int i = 0; i < _pool.size(); i++);
	{
		if (_pool[i] == toFree)
		{
			_isInUse[i] = false;
		}
	}
}
