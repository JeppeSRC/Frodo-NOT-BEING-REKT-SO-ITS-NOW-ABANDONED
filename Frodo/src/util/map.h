#pragma once
#include <fd.h>
#include "list.h"

template<typename K, typename D>
class Map {
private:
	List<D> data;
	List<K> keys;

	inline D& Add(K key) {
		keys.Push_back(key);

		size_t size = keys.GetSize();
		data.Resize(size);

		return data[size - 1];
	}

public:
	Map(unsigned int size = 0) {
		data.Reserve(size);
		keys.Reserve(size);
	}

	~Map() {
		
	}

	inline void Reserve(unsigned int size) {
		data.Reserve(size);
		keys.Reserve(size);
	}

	inline void Remove(K key) {
		size_t index = keys.Find(key);
		if (index == (size_t)-1) return;

		keys.RemoveIndex(index);
		data.RemoveIndex(index);
	}

	__forceinline D& operator[](K key) {
		size_t loc = keys.Find(key);
		if (loc != (size_t)-1) {
			return data[loc];
		}

		return Add(key);
	}

	__forceinline void Add(D item, K key) {

		data << item;
		keys << key;
	}

	__forceinline D Retrieve(K key) const {
		size_t index = keys.Find(key);
		
		if (index == (size_t)-1) return D();

		return data.Get(index);
	}


	inline size_t GetItems() const { return keys.GetSize(); }

	inline List<K> GetKeyList() const { return keys; }
	inline List<D> GetDataList() const { return data; }
};
