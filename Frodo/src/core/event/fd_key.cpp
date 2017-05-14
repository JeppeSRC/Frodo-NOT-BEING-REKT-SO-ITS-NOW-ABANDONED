#include "fd_key.h"

namespace FD {

FD_KEYMAP_LAYOUT KeyMap::layout;

Map<byte, uint64> KeyMap::map(256);

void KeyMap::InitializeUS() {
	map.Clear();

	map.Add(0xFF, 0);
	map.Add(FD_KEY_0, MAKE_KEY(0, 0, ')', '0'));
	map.Add(FD_KEY_1, MAKE_KEY(0, 0, '!', '1'));
	map.Add(FD_KEY_2, MAKE_KEY(0, 0, '@', '2'));
	map.Add(FD_KEY_3, MAKE_KEY(0, 0, '#', '3'));
	map.Add(FD_KEY_4, MAKE_KEY(0, 0, '$', '4'));
	map.Add(FD_KEY_5, MAKE_KEY(0, 0, '%', '5'));
	map.Add(FD_KEY_6, MAKE_KEY(0, 0, '^', '6'));
	map.Add(FD_KEY_7, MAKE_KEY(0, 0, '&', '7'));
	map.Add(FD_KEY_8, MAKE_KEY(0, 0, '*', '8'));
	map.Add(FD_KEY_9, MAKE_KEY(0, 0, '(', '9'));
	map.Add(FD_KEY_A, MAKE_KEY(0, 0, 'A', 'a'));
	map.Add(FD_KEY_B, MAKE_KEY(0, 0, 'B', 'b'));
	map.Add(FD_KEY_C, MAKE_KEY(0, 0, 'C', 'c'));
	map.Add(FD_KEY_D, MAKE_KEY(0, 0, 'D', 'd'));
	map.Add(FD_KEY_E, MAKE_KEY(0, 0, 'E', 'e'));
	map.Add(FD_KEY_F, MAKE_KEY(0, 0, 'F', 'f'));
	map.Add(FD_KEY_G, MAKE_KEY(0, 0, 'G', 'g'));
	map.Add(FD_KEY_H, MAKE_KEY(0, 0, 'H', 'h'));
	map.Add(FD_KEY_I, MAKE_KEY(0, 0, 'I', 'i'));
	map.Add(FD_KEY_J, MAKE_KEY(0, 0, 'J', 'j'));
	map.Add(FD_KEY_K, MAKE_KEY(0, 0, 'K', 'k'));
	map.Add(FD_KEY_L, MAKE_KEY(0, 0, 'L', 'l'));
	map.Add(FD_KEY_M, MAKE_KEY(0, 0, 'M', 'm'));
	map.Add(FD_KEY_N, MAKE_KEY(0, 0, 'N', 'n'));
	map.Add(FD_KEY_O, MAKE_KEY(0, 0, 'O', 'o'));
	map.Add(FD_KEY_P, MAKE_KEY(0, 0, 'P', 'p'));
	map.Add(FD_KEY_Q, MAKE_KEY(0, 0, 'Q', 'q'));
	map.Add(FD_KEY_R, MAKE_KEY(0, 0, 'R', 'r'));
	map.Add(FD_KEY_S, MAKE_KEY(0, 0, 'S', 's'));
	map.Add(FD_KEY_T, MAKE_KEY(0, 0, 'T', 't'));
	map.Add(FD_KEY_U, MAKE_KEY(0, 0, 'U', 'u'));
	map.Add(FD_KEY_V, MAKE_KEY(0, 0, 'V', 'v'));
	map.Add(FD_KEY_W, MAKE_KEY(0, 0, 'W', 'w'));
	map.Add(FD_KEY_X, MAKE_KEY(0, 0, 'X', 'x'));
	map.Add(FD_KEY_Y, MAKE_KEY(0, 0, 'Y', 'y'));
	map.Add(FD_KEY_Z, MAKE_KEY(0, 0, 'Z', 'z'));
	map.Add(FD_KEY_GRAVE, MAKE_KEY(0, 0, '~', '`'));
	map.Add(FD_KEY_MINUS, MAKE_KEY(0, 0, '_', '-'));
	map.Add(FD_KEY_EQUALS, MAKE_KEY(0, 0, '+', '='));
	map.Add(FD_KEY_LBRACKET, MAKE_KEY(0, 0, '{', '['));
	map.Add(FD_KEY_RBRACKET, MAKE_KEY(0, 0, '}', ']'));
	map.Add(FD_KEY_BACKSLASH, MAKE_KEY(0, 0, '|', '\\'));
	map.Add(FD_KEY_SEMICOLON, MAKE_KEY(0, 0, ':', ';'));
	map.Add(FD_KEY_APOSTROPHE, MAKE_KEY(0, 0, '"', '\''));
	map.Add(FD_KEY_COMMA, MAKE_KEY(0, 0, '<', ','));
	map.Add(FD_KEY_PERIOD, MAKE_KEY(0, 0, '>', '.'));
	map.Add(FD_KEY_SLASH, MAKE_KEY(0, 0, '?', '/'));
	map.Add(FD_KEY_SPACE, MAKE_KEY(0, 0, 0, ' '));  

}

void KeyMap::InitializeSWE() {
	map.Clear();

	map.Add(0xFF, 0);
	map.Add(FD_KEY_0, MAKE_KEY('}',	0, '=', '0'));
	map.Add(FD_KEY_1, MAKE_KEY(0,	0, '!', '1'));
	map.Add(FD_KEY_2, MAKE_KEY('@', 0, '"', '2'));
	map.Add(FD_KEY_3, MAKE_KEY('£',	0, '#', '3'));
	map.Add(FD_KEY_4, MAKE_KEY('$',	0, '¤', '4'));
	map.Add(FD_KEY_5, MAKE_KEY('€',	0, '%', '5'));
	map.Add(FD_KEY_6, MAKE_KEY(0,	0, '&', '6'));
	map.Add(FD_KEY_7, MAKE_KEY('{',	0, '/', '7'));
	map.Add(FD_KEY_8, MAKE_KEY('[',	0, '(', '8'));
	map.Add(FD_KEY_9, MAKE_KEY(']',	0, ')', '9'));
	map.Add(FD_KEY_A, MAKE_KEY(0,	0, 'A', 'a'));
	map.Add(FD_KEY_B, MAKE_KEY(0,	0, 'B', 'b'));
	map.Add(FD_KEY_C, MAKE_KEY(0,	0, 'C', 'c'));
	map.Add(FD_KEY_D, MAKE_KEY(0,	0, 'D', 'd'));
	map.Add(FD_KEY_E, MAKE_KEY(0,	0, 'E', 'e'));
	map.Add(FD_KEY_F, MAKE_KEY(0,	0, 'F', 'f'));
	map.Add(FD_KEY_G, MAKE_KEY(0,	0, 'G', 'g'));
	map.Add(FD_KEY_H, MAKE_KEY(0,	0, 'H', 'h'));
	map.Add(FD_KEY_I, MAKE_KEY(0,	0, 'I', 'i'));
	map.Add(FD_KEY_J, MAKE_KEY(0,	0, 'J', 'j'));
	map.Add(FD_KEY_K, MAKE_KEY(0,	0, 'K', 'k'));
	map.Add(FD_KEY_L, MAKE_KEY(0,	0, 'L', 'l'));
	map.Add(FD_KEY_M, MAKE_KEY(0,	0, 'M', 'm'));
	map.Add(FD_KEY_N, MAKE_KEY(0,	0, 'N', 'n'));
	map.Add(FD_KEY_O, MAKE_KEY(0,	0, 'O', 'o'));
	map.Add(FD_KEY_P, MAKE_KEY(0,	0, 'P', 'p'));
	map.Add(FD_KEY_Q, MAKE_KEY(0,	0, 'Q', 'q'));
	map.Add(FD_KEY_R, MAKE_KEY(0,	0, 'R', 'r'));
	map.Add(FD_KEY_S, MAKE_KEY(0,	0, 'S', 's'));
	map.Add(FD_KEY_T, MAKE_KEY(0,	0, 'T', 't'));
	map.Add(FD_KEY_U, MAKE_KEY(0,	0, 'U', 'u')); 
	map.Add(FD_KEY_V, MAKE_KEY(0,	0, 'V', 'v'));
	map.Add(FD_KEY_W, MAKE_KEY(0,	0, 'W', 'w'));
	map.Add(FD_KEY_X, MAKE_KEY(0,	0, 'X', 'x'));
	map.Add(FD_KEY_Y, MAKE_KEY(0,	0, 'Y', 'y'));
	map.Add(FD_KEY_Z, MAKE_KEY(0,	0, 'Z', 'z'));
	map.Add(FD_KEY_GRAVE, MAKE_KEY(0, 0, '½', '§'));
	map.Add(FD_KEY_MINUS, MAKE_KEY('\\', 0, '?', '+'));
	map.Add(FD_KEY_EQUALS, MAKE_KEY(0, 0, '`', '´'));
	map.Add(FD_KEY_LBRACKET, MAKE_KEY(0, 0, 'Å', 'å'));
	map.Add(FD_KEY_RBRACKET, MAKE_KEY('~', 0, '^', '¨'));
	map.Add(FD_KEY_BACKSLASH, MAKE_KEY(0, 0, '*', '\''));
	map.Add(FD_KEY_SEMICOLON, MAKE_KEY(0, 0, 'Ö', 'ö'));
	map.Add(FD_KEY_APOSTROPHE, MAKE_KEY(0, 0, 'Ä', 'ä'));
	map.Add(FD_KEY_COMMA, MAKE_KEY(0, 0, ';', ','));
	map.Add(FD_KEY_PERIOD, MAKE_KEY(0, 0, ':', '.'));
	map.Add(FD_KEY_SLASH, MAKE_KEY(0, 0, '_', '-'));
	map.Add(FD_KEY_SPACE, MAKE_KEY(0, 0, 0, ' '));
}

void KeyMap::Init(FD_KEYMAP_LAYOUT layout) {
	KeyMap::layout = layout;
	switch (layout) {
	case FD_KEYMAP_LAYOUT_SWE:
		InitializeSWE();
		break;
	case FD_KEYMAP_LAYOUT_US:
		InitializeUS();
		break;
	}
}


uint32 KeyMap::GetChar(FD_KEY key, uint32 modifiers) {
	uint32 res = (uint32)map.Retrieve(key);
	
	if (res == 0)
		return (uint32)-1;

	if (modifiers & FD_MODIFIER_SHIFT)
		return GET_SHIFT(res);
	else if (modifiers & FD_MODIFIER_ALT)
		return GET_ALT(res);
	else if (modifiers & FD_MODIFIER_ALTGR)
		return GET_ALTGR(res);
	else
		return GET_ORG(res);
}

}