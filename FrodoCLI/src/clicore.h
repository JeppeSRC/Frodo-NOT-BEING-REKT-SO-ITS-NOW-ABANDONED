#pragma once

#include <msclr/marshal_cppstd.h>
#include <util/string.h>
#include <string>

namespace FDCLI {

template<typename T>
public ref class CLIClass {
protected:
	T* instance;

	bool keepInstance;

public:
	CLIClass() {
		instance = nullptr;
		keepInstance = false;
	}

	CLIClass(T* instance, bool keepInstance) {
		this->instance = instance;
		this->keepInstance = keepInstance;
	}

	virtual ~CLIClass() {
		if (!keepInstance) delete instance;
	}

	!CLIClass() {
		if (!keepInstance) delete instance;
	}

	T* GetHandle() {
		return instance;
	}
};


inline static FD::String system_string_to_string(System::String^ string) {
	return FD::String(msclr::interop::marshal_as<std::string>(string).c_str());
}

inline static System::String^ string_to_system_string(FD::String string) {
	return gcnew System::String(*string);
}

}