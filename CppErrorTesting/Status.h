#pragma once
#include <string>
#include <sstream>
#include <memory>
#include <ostream>

class Stat {
public:
	static const short error = -1;
	static const short success = 0;
	const short type;

	Stat(short t) :
		type(t) {}

	virtual ~Stat() {};
	virtual const std::string Type() const = 0;
	virtual const std::string Desc() const = 0;
};

class Status {
public:
	std::shared_ptr<Stat> stat;

	class basic : public Stat {
	public:
		const std::string err;
		
		basic(short t, const std::string& e) :
			Stat(t),
			err(e) {}

		virtual ~basic() {}

		virtual const std::string Type() const {
			std::stringstream ss;
			if (type < success) {
				ss << "ERROR";
			} else {
				ss << "SUCCESS";
			}

			ss << "[ " << type << " ]";
			return ss.str();
		}

		virtual const std::string Desc() const {
			return err;
		}
	};

	Status(Stat* s) :
		stat(s) {}

	Status(Status&& s) :
		stat(s.stat) {}

	Status(short type, const std::string& desc) :
		stat(new basic(type, desc)) {}

	virtual ~Status() {}

	static Status Success() {
		return Status(new basic(basic::success, ""));
	}

	static Status Error(const char* msg) {
		return Status(new basic(basic::error, msg));
	}

	const std::string Type() const {
		return stat->Type();
	}

	const std::string Desc() const {
		return stat->Desc();
	}

	operator bool() const {
		return stat->type >= Stat::success;
	}

	operator std::string() const {
		return stat->Type() + " " + stat->Desc();
	}

	friend std::ostream& operator<<(std::ostream& ostr, const Status& status);

};

std::ostream& operator<<(std::ostream& ostr, const Status& status) {
	ostr << status.Type() << " " << status.Desc();
	return ostr;
}