#include "db_interface.h"

db_interface::db_interface(const std::string& _filename) {
	filename = _filename;
}

static int callback(void* ptr, int argc, char** argv, char** azColName) {
	// The sqlite3_exec(...) function takes a void pointer argument which is passed to the first argument of this callback.
	// In db_interface::read_db(), the an std::stringstream is pointed to by a void*.
	// In this callback, we can simply cast the void* to an std::stringstream*, dereference, and operate on the object as normal.
	// Done as: (*(std::stringstream*)ptr)
	// Because we grab the entire table, an '/' is inserted to delimit each item in each row in the std::stringstream.
	// And ">" is inserted between each row.

	// TEST: printing
	for (int i = 0; i < argc; i++) {
		(*(std::stringstream*)ptr) << argv[i] << '/';
	}

	(*(std::stringstream*)ptr) << ">";

	return 0;
}

void db_interface::parse_table(const std::string& table) {
	// Split by row delimiters
	std::vector<std::string> token_table {};

	// ">" is the inter-row delimiter
	token_table = tokenize(table, ">");

	// "/" is the intra-row item delimiter
	for (auto line : token_table) {
		data.push_back(tokenize(line, "/"));
	}

}

void db_interface::read_db() {
	// The sql execution callback requires a void* for a passed argument.
	std::stringstream data_table{};
	void* ptr = &data_table;

	sqlite3* db;
	char* messageError;

	std::string sql = "SELECT * FROM users;";

	int exit = sqlite3_open(filename.c_str(), &db);

	exit = sqlite3_exec(db, sql.c_str(), callback, ptr, &messageError);

	parse_table(data_table.str());

	
	if (exit != SQLITE_OK) {
		std::cout << filename << std::endl;
		std::cout << "Error in selectData function" << std::endl;
		sqlite3_free(messageError);
	}
	else {
		//std::cout << "Records selected" << std::endl;
	}
}

std::vector<User> db_interface::read_users() {
	std::vector<User> users{};

	std::string _firstname{};
	std::string _lastname{};
	std::vector<std::string> _ids{};

	read_db();

	for (auto row : data) {
		_firstname = row.at(0);
		_lastname = row.at(1);

		for (size_t i{ 2 }; i < row.size(); i++) {
			_ids.push_back(row.at(i));

		}

		users.emplace_back(_firstname, _lastname, _ids);
		_ids.clear();
	}

	return users;
}