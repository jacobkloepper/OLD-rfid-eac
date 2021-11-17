#include "user.h"

  // Default constructor
User::User() {
	firstname = "DEFAULT";
	lastname = "DEFAULT";
	ids.clear();
}

// Custom constructor: given name and id (hex string)
User::User(const std::string& _firstname, const std::string& _lastname, const std::string& _id) {
	firstname = _firstname;
	lastname = _lastname;
	ids.push_back(hex_to_id(_id));
}

// Custom constructor: given name and vector of ids (hex strings)
User::User(const std::string& _firstname, const std::string& _lastname, const std::vector<std::string>& _id_vec) {
	firstname = _firstname;
	lastname = _lastname;

	// TODO: use class methods
	for (const auto& id : _id_vec) {
		ids.push_back(hex_to_id(id));
	}

}

// GETTERS
std::pair<std::string, std::string> User::get_name() {
	return std::make_pair(firstname, lastname);
}
std::vector<id_type> User::get_ids() {
	return ids;
}

// Given an object with name and ID, print their name and all IDs
void User::print_user() {
	std::cout << get_name().first << " " << get_name().second << ":" << std::endl;

	for (size_t i{ 0 }; i < get_ids().size(); i++) {
		std::cout << "\tID " << i << ": " << " (" << get_ids().at(i) << ")" << std::endl;
	}
}