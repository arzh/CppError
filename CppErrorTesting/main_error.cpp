

#include <iostream>
#include <sstream>
#include "Error.h"

Error RetError() {
	return Error("STANDARD MESSAGE");
}

Error RetNil() {
	return nil;
}

class cust_err : public error {
public:
	const std::string Name;
	const int ErrorCode;

	cust_err(const std::string& _name, int _code) :
		Name(_name),
		ErrorCode(_code) {}

	virtual const std::string Error() const {
		std::stringstream ss;
		ss << Name << ":\n" << ErrorCode;
		return ss.str();
	}

	static class Error New(const std::string& _name, int _code) {
		return class Error(new cust_err(_name, _code));
	}
};

Error RetCustom() {
	//return cust_err::New("CUSTOM", 9999);
	return Error(new cust_err("CUSTOM", 9999));
}

void main(void) {
	Error err = RetError();
	if (err != nil) {
		std::cout << err->Error() << std::endl;
	}

	err = RetNil();
	if (err == nil) {
		std::cout << "SUCCESSFUL TEST!" << std::endl;
	}

	err = RetCustom();
	if (err != nil) {
		if (err.IsType<cust_err>()) {
			cust_err e = err.AsType<cust_err>();
			std::cout << "This is a cust_err " << e.Name << " " << e.ErrorCode << "!\n";
		}

		std::cout << err->Error() << std::endl;
	}

	system("PAUSE");
}