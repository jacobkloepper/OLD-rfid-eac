#ifndef DB_INTERFACE_H_
#define DB_INTERFACE_H_

#include "util.h"
#include "user.h"
#include "sqlite3.h"

class db_interface {
public:
	db_interface(const std::string& _filename);

	// Get a vector of Users from TABLE users
	std::vector<User> read_users();

private:
	// C string for interfacing with sqlite3
	std::string filename;
	std::vector<std::vector<std::string>> data;

	// Read and parse database
	void read_db();
	void parse_table(const std::string&);
};


#endif // DB_INTERFACE_H_