#pragma once
using namespace std;

template <typename T> class ObjectPool
{
	public:

	vector<T> pool;

	ObjectPool()
	{
		pool = {};
	}

	void Add(T value)
	{
		pool.push_back(value);
	}
	void Remove(int index)
	{
		pool.erase(pool.begin() + index);
	}
};