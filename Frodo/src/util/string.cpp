#include "string.h"
#include "list.h"
#include <string>
#include <core/log.h>

String::String(const char* string) {
	if (!string) {
		length = 0;
		str = nullptr;
		return;
	}
	length = strlen(string);
	str = new char[length + 1];
	str[length] = '\0';
	memcpy(str, string, length);
}

String::String(const char* string, size_t length) {
	if (!string) {
		this->length = 0;
		str = nullptr;
		return;
	}
	this->length = length;
	str = new char[length + 1];
	str[length] = 0;
	memcpy(str, string, length);
}

String::String(const String& string) {
	this->length = string.length;

	str = new char[length + 1];
	str[length] = 0;
	memcpy(str, string.str, length);
}

String::String(String&& string) {
	this->length = 0;
	this->str = nullptr;
	*this = std::move(string);
}

String::~String() {
	delete[] str;
	str = nullptr;
}

String& String::operator=(const String& string) {
	if (this != &string) {
		delete[] str;
		length = string.length;

		str = new char[length + 1];
		str[length] = 0;
		memcpy(str, string.str, length);
	}
	return *this;
}

String& String::operator=(String&& string) {
	if (this != &string) {
		delete[] str;
		length = string.length;

		str = string.str;

		string.length = 0;
		string.str = nullptr;
	}
	return *this;
}

String& String::Append(const String& string) {
	size_t newlen = length + string.length;
	char* tmpstr = str;
	str = new char[newlen + 1];
	str[newlen] = 0;
	memcpy(str, tmpstr, length);
	memcpy(str + length, string.str, string.length);
	length = newlen;

	delete[] tmpstr;
			
	return *this;
}

String& String::Remove(const String& string) {
	FD_ASSERT(length > string.length);

	size_t index = Contains(string);
	size_t newlen = length - string.length;

	char* tmp = str;

	str = new char[newlen + 1];
	memcpy(str, tmp, index);
	memcpy(str + index, tmp + index + string.length, newlen - index);

	length = newlen;
	str[length] = '\0';

	delete[] tmp;

	return *this;
}

char String::operator[](size_t index) {
	return str[index];
}

bool String::operator==(const String& string) {
	if (length != string.length) return false;

	for (size_t i = 0; i < length; i++) {
		if (str[i] != string.str[i]) return false;
	}

	return true;
}

bool String::operator!=(const String& string) {
	if (length != string.length) return false;

	for (size_t i = 0; i < length; i++) {
		if (str[i] == string.str[i]) return false;
	}

	return true;
}

bool String::StartsWith(const String& string) {
	if (length < string.length) return false;

	for (size_t i = 0; i < string.length; i++)
		if (str[i] != string.str[i]) return false;

	return true;
}

bool String::EndsWith(const String& string) {
	if (length < string.length) return false;

	for (size_t i = 0; i < string.length; i++)
		if (str[length - i - 1] != string.str[string.length - i - 1]) return false;

	return true;
}

size_t String::Contains(const String& string) {
	if (length < string.length) return (size_t)-1;

	for (size_t i = 0; i < length; i++) {
		bool match = true;
		if (i + string.length > length) return (size_t)-1;
		for (size_t j = 0; j < string.length; j++) {
					
			if (str[i + j] != string.str[j]) {
				match = false;
				break;
			}
		}

		if (match) {
			return i;
		}
	}

	return (size_t)-1;
}

List<String*> String::Split(const char delimiter) {

	List<String*> list(128, 32);
		
	Split(delimiter, list);

	return list;
}

void String::Split(const char delimiter, List<String*>& list) {
	size_t lastindex = 0;

	for (size_t i = 0; i < length; i++) {
		if (str[i] == delimiter) {
				
			list << new String(str + lastindex, i - lastindex);
			lastindex = i + 1;
		}
	}

	if (lastindex < length)
		list << new String(str + lastindex, length - lastindex);

}

 