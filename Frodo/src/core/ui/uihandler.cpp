#include "uihandler.h"
#include "uiitem.h"

UIHandler::UIHandler() {

}

UIHandler::~UIHandler() {

}

void UIHandler::Add(UIItem* item) {
	item->SetHandler(this);
	items.Push_back(item);
}

void UIHandler::Update() {

}