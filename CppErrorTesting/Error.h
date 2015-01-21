//#pragma once
//
//// go like Error type
//
//#include <string>
//#include <memory>
//
//
//class ierror {
//public:
//	virtual const std::string Error() const = 0;
//};
//
//class err_stat : public ierror {
//public:
//	const std::string type;
//	const std::string msg;
//
//	err_stat(const std::string& _type, const std::string& _msg) :
//		type(_type),
//		msg(_msg) {}
//
//	virtual const std::string Error() const {
//		return type + ": " + msg;
//	}
//};
//
////typedef std::shared_ptr<ierror> error;
//
//static const int nil = 0;
//
//class error : public std::shared_ptr < ierror > {
//public:
//	error(ierror* err) :
//		std::shared_ptr<ierror>(err) {}
//
//	error(const std::string& _t, const std::string& _m) :
//		std::shared_ptr<ierror>(new err_stat(_t, _m)) {}
//
//	error(const std::string _m) :
//		std::shared_ptr<ierror>(new err_stat("ERROR", _m)) {}
//
//	template<class T>
//	bool IsType() {
//		T* test = dynamic_cast<T*>(this->get());
//		return (test != nil);
//	}
//
//	template<class T>
//	const T AsType() {
//		T* typePtr = dynamic_cast<T*>(this->get());
//		return *typePtr;
//	}
//};


#pragma once

// go like Error type

#include <string>
#include <memory>


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
		return type + ": " + msg;
	}
};

//typedef std::shared_ptr<ierror> error;

static const int nil = 0;

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
};

