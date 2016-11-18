#pragma once

#include <fd.h>
#include <core/event/eventlistener.h>
#include <util/list.h>

class FDAPI Menu : public EventListener {
private:
	friend class MenuItem;
private:
	List<MenuItem*> items;


public:
	Menu();
	~Menu();

	void Add(MenuItem* item);

	void Update();
	

};
