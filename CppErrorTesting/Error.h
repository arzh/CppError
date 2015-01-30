#pragma once

// go like Error type

#include <string>
#include <memory>

std::nullptr_t nil;

class error {
public:
	virtual const std::string Error() const = 0;
};


class err_stat : public error {
public:
	const std::string type;
	const std::string msg;

	err_stat(const std::string& _type, const std::string& _msg) :
		type(_type),
		msg(_msg) {}

	virtual const std::string Error() const {
		return type + ": " + msg + "\n";
	}
};

//typedef std::shared_ptr<ierror> error;

class Error : public std::shared_ptr < error > {
public:
	Error(error* err) :
		std::shared_ptr<error>(err) {}

	Error(const std::string& _t, const std::string& _m) :
		std::shared_ptr<error>(new err_stat(_t, _m)) {}

	Error(const std::string _m) :
		std::shared_ptr<error>(new err_stat("ERROR", _m)) {}

	template<class T>
	bool IsType() {
		T* test = dynamic_cast<T*>(this->get());
		return (test != nil);
	}

	template<class T>
	const T AsType() {
		T* typePtr = dynamic_cast<T*>(this->get());
		return *typePtr;
	}

	operator std::string() const {
		return get()->Error();
	}
};


std::ostream& operator<<(std::ostream& ostr, const Error& err) {
	std::string s = err;
	ostr << s;
	return ostr;
}

#include <stdarg.h>
#include <assert.h>

template<typename... Args>
Error Errorf(const char* fmt, Args... args) {

	va_list alist;
	va_start(alist, fmt);

	unsigned int total = vsnprintf(NULL, 0, fmt, alist) + 1; // calc size + 1 char for null term

#ifndef IGNORE_ERROR_SIZE_CHECK
	assert(total < 2048); // Might want to check the error message you are creating if it's this large.
#endif
	
	char* buffer = new char[total]();

	vsprintf(buffer, fmt, alist);
	va_end(alist);

	std::string str = std::string(buffer);
	delete[] buffer;

	return Error(str);
}

