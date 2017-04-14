#pragma once
#include <fd.h>
#include "uihandler.h"
#include "uiitem.h"
#include "uibutton.h"
#include "uigroup.h"
#include "uislider.h"
#include "uitextbox.h"
#include "uitext.h"

namespace FD {

FD_EXTERN_TEMPLATE template FDAPI UIText* UIItem::GetText<UIText>(const String&);
FD_EXTERN_TEMPLATE template FDAPI UITextAutoResize* UIItem::GetText<UITextAutoResize>(const String&);
FD_EXTERN_TEMPLATE template FDAPI UITextHorizontalScroll* UIItem::GetText<UITextHorizontalScroll>(const String&);

}