#pragma once

#include <memory>


template<typename T>
class List {
private:
	T* data;
	size_t size;
	size_t allocated;
	size_t extraReserve;

public:
	List() {
		data = nullptr;
		size = 0;
		allocated = 0;
		extraReserve = 1;
	}

	List(size_t reserve, size_t extra_reserve = 1) {
		data = new T[reserve];
		size = 0;
		allocated = reserve;
		extraReserve = extra_reserve;
	}

	List(const List<T>& list) {
		
		data = new T[list.allocated];

		size = list.size;
		allocated = list.allocated;

		memcpy(data, list.data, list.size * sizeof(T));
	}

	List(List<T>&& list) {
		data = nullptr;
		*this = std::move(list);
	}

	~List() {
		delete[] data;
	}

	__forceinline T& operator[](size_t index) {
		return data[index];
	}

	__forceinline T Get(size_t index) const {
		return data[index];
	}

	inline List<T>& operator=(const List<T>& list) {
	
		delete[] data;
			
		data = new T[list.allocated];

		size = list.size;
		allocated = list.allocated;

		memcpy(data, list.data, list.size * sizeof(T));

		return *this;
	}

	__forceinline List<T>& operator=(List<T>&& list) {
		if (this != &list) {
			delete[] data;
			data = list.data;
			size = list.size;
			allocated = list.allocated;

			list.data = nullptr;
			list.size = 0;
			list.allocated = 0;
		}

		return *this;
	}

	__forceinline void Push_back(T item) {
		if (size == allocated)
			Reserve(allocated + extraReserve);

		data[size++] = item;
	}

	__forceinline void operator<<(T item) {
		Push_back(item);
	}

	inline size_t Find(T item) const {
		for (size_t i = 0; i < size; i++)
			if (data[i] == item) return i;

		return (size_t)-1;
	}

	inline void Reserve(size_t count) {
		if (count <= allocated) return;

		T* tmp = data;

		data = new T[count];
		memcpy(data, tmp, GetSizeInBytes());

		delete[] tmp;

		allocated = count;
	}

	inline void Resize(size_t count) {
		if (count > allocated)
			Reserve(count);

		size = count;
	}

	inline T Remove(T item) {
		for (size_t i = 0; i < size; i++)
			if (data[i] == item) {
				return RemoveIndex(i);
			}

		return T();
	}

	inline T RemoveIndex(size_t index) {
		T tmp = data[index];

		size--;
		memcpy(data + index, data + index + 1, (size - index) * sizeof(T));
		return tmp;
	}


	inline void Clear() {
		memset(data, 0, GetSizeInBytes());
		size = 0;
	}

	inline void Free(bool isArray = false) {
		if (isArray)
			for (size_t i = 0; i < size; i++) {
				delete[] data[i];
			}
		else
			for (size_t i = 0; i < size; i++) {
				delete data[i];
			}
	}

	inline void SetExtraReserve(size_t extra) { this->extraReserve = extra; }

	inline T* GetData() { return data; }
	inline const size_t GetSize() const { return size; }
	inline const size_t GetSizeInBytes() const { return size * sizeof(T); }
	inline const size_t GetAllocated() const { return allocated; }
	inline const size_t GetAllocatedInBytes() const { return allocated * sizeof(T); }

	inline const size_t GetExtraReserve() const { return extraReserve; }
};
