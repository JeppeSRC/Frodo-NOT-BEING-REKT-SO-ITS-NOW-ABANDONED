#pragma once

#include <fd.h>
#include <core/event/eventlistener.h>
#include <util/list.h>

class FDAPI UIHandler : public EventListener {
private:
	friend class UIItem;
private:
	List<UIItem*> items;


public:
	UIHandler();
	~UIHandler();

	void Add(UIItem* item);

	void Update();
	
	inline UIItem* operator[](size_t index) const { return items.Get(index); }

	inline size_t GetSize() const { return items.GetSize(); }
	inline const List<UIItem*>& GetItems() const { return items; }
};
