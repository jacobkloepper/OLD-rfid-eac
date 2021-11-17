#ifndef LOGGER_H_
#define LOGGER_H_

#include "util.h"
#include "db_interface.h"

class Logger {
public:
	Logger(const data_pkt&, const std::string&);

	// WRITING
	void write();

	// DEBUG
	void print_data();

private:
	data_pkt data;
	bool exists; // TESTING
	std::string db_filename;
	std::string filename;

	// WRITING
	bool check_log();
	void create_log();
	void write_log();

	bool check_pop();
	void create_pop();
	void write_pop();

	// DATA RETRIEVAL
	std::pair<std::string, std::string> get_username();
	unsigned int get_entry_num();
};

#endif /* LOGGER_H_ */
