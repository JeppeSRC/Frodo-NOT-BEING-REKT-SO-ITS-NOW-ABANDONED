#pragma once

#include <msclr/marshal.h>
#include <util/string.h>
#include <string>

template<typename T>
public ref class CLIClass {
protected:
	T* instance;

public:
	CLIClass() {
		instance = nullptr;
	}

	CLIClass(T* instance) {
		this->instance = instance;
	}

	virtual ~CLIClass() {
		delete instance;
	}

	!CLIClass() {
		delete instance;
	}

	T* GetHandle() {
		return instance;
	}
};


inline static String system_string_to_string(System::String^ string) {
	return String(msclr::interop::marshal_as<std::string>(string).c_str());
}

inline static System::String^ string_to_system_string(String string) {
	return gcnew System::String(*string);
}