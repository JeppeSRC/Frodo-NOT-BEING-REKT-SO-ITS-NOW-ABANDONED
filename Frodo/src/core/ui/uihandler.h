#pragma once

#include <fd.h>
#include <core/event/eventlistener.h>
#include <util/list.h>

namespace FD {

class FDAPI UIHandler : public EventListener {
private:
	friend class UIItem;
private:
	List<UIItem*> items;
	UIItem* inFocus;

public:
	UIHandler();
	~UIHandler();

	void Add(UIItem* item);

	void Update();

	inline UIItem* operator[](size_t index) const { return items.Get(index); }

	inline size_t GetSize() const { return items.GetSize(); }
	inline const List<UIItem*>& GetItems() const { return items; }

public:
	bool OnMouseActionButtonPressed(unsigned int button) override;
	bool OnMouseActionButtonReleased(unsigned int button) override;
	bool OnMouseActionMove(ivec2 position) override;

	bool OnKeyboardActionKeyPressed(uint32 key) override;
};

}