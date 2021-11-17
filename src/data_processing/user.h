#ifndef RFID_LOGISTICS_USER_H
#define RFID_LOGISTICS_USER_H

#include "util.h"

class User {
public:
	User();
	User(const std::string&, const std::string&, const std::string&);
	User(const std::string&, const std::string&, const std::vector<std::string>&);

	std::pair<std::string, std::string> get_name();
	std::vector<id_type> get_ids();

	void print_user();

private:
	std::string firstname;
	std::string lastname;
	std::vector<id_type> ids;
};

#endif //RFID_LOGISTICS_USER_H
