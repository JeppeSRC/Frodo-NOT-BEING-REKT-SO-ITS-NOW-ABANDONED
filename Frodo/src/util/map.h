#pragma once
#include <fd.h>
#include "list.h"

namespace FD {

template<typename K, typename D>
struct FD_MAP_PAIR {
	K key;
	D data;
};

template<typename K, typename D>
class Map {
private:
	List<D> data;
	List<K> keys;

	inline D& Add(K key) {
		keys.Push_back(key);

		uint_t size = keys.GetSize();
		data.Resize(size);

		return data[size - 1];
	}

public:
	Map(uint32 size = 0) {
		data.Reserve(size);
		keys.Reserve(size);
	}

	~Map() {

	}

	inline Map<K, D>& operator=(const Map<K, D>& map) {
		data = map.data;
		keys = map.keys;
		return *this;
	}

	inline void Reserve(uint32 size) {
		data.Reserve(size);
		keys.Reserve(size);
	}

	inline void Remove(K key) {
		uint_t index = keys.Find(key);
		if (index == (uint_t)-1) return;

		keys.RemoveIndex(index);
		data.RemoveIndex(index);
	}

	__forceinline D& operator[](K key) {
		uint_t loc = keys.Find(key);
		if (loc != (uint_t)-1) {
			return data[loc];
		}

		return Add(key);
	}

	__forceinline void Add(D item, K key) {

		data << item;
		keys << key;
	}

	__forceinline D Retrieve(K key) const {
		uint_t index = keys.Find(key);

		if (index == (uint_t)-1) return D();

		return data.Get(index);
	}


	inline uint_t GetItems() const { return keys.GetSize(); }

	inline List<K> GetKeyList() const { return keys; }
	inline List<D> GetDataList() const { return data; }

	inline FD_MAP_PAIR<K, D> GetPair(uint_t index) { return { keys[index], data[index] }; }
};

}