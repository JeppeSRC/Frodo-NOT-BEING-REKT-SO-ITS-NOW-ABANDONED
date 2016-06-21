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

	bool noDelete;

public:
	String() { str = nullptr; length = 0; }
	String(const char* string);
	String(char* stirng, size_t length, bool noCopy = false);
	String(const String& string);
	String(const String* string);
	String(String&& string);
	~String();

	String& operator=(const String& string);
	String& operator=(String&& string);

	String& Append(const String& string);
	__forceinline String& operator<<(const String& string) { return Append(string); }

	String& Remove(const String& string);
	String& Remove(size_t start, size_t end);
	String& RemoveBlankspace();

	size_t Count(const String& string, size_t offset = 0) const;

	inline bool IsNull() const { return (str == nullptr && length == 0); }

	char operator[](size_t index) const;

	bool operator==(const String& string);
	bool operator!=(const String& string);
	String operator+(const String& string);
	__forceinline void operator+=(const String& string) { Append(string); }

	bool StartsWith(const String& string) const;
	bool EndsWith(const String& string) const;
	size_t Find(const String& string, size_t offset = 0) const;

	List<String*> Split(const char delimiter) const;
	void Split(const char delimiter, List<String*>& list) const;

	inline char* operator*() const { return str; }

	inline wchar_t* GetWCHAR() const {
		wchar_t* tmp = new wchar_t[length + 1];
		swprintf_s(tmp,length+1,  L"%S", str);
		tmp[length] = '\0';
		return tmp;
	}
};
 