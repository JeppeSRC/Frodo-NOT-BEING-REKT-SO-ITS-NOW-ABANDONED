#pragma once
#include <fd.h>
#include <stdio.h>
 
class FDAPI String {
private:
	template<typename T>
	friend class List;
public:
	char* str;
	size_t length;

public:
	String() { str = nullptr; length = 0; }
	String(const char* string);
	String(const char* stirng, size_t length);
	String(const String& string);
	String(String&& string);
	~String();

	String& operator=(const String& string);
	String& operator=(String&& string);

	String& operator<<(const String& string);

	char operator[](size_t index);

	bool operator==(const String& string);
	bool operator!=(const String& string);

	bool StartsWith(const String& string);
	bool EndsWith(const String& string);
	size_t Contains(const String& string);

	List<String*> Split(const char delimiter);
	void Split(const char delimiter, List<String*>& list);

	inline char* operator*() const { return str; }

	inline wchar_t* GetWCHAR() const {
		wchar_t* tmp = new wchar_t[length + 1];
		swprintf_s(tmp,length+1,  L"%S", str);
		tmp[length] = '\0';
		return tmp;
	}
};
 