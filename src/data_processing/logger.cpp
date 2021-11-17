// Server-side. Receives data packet from reader_interface, processes to append a log.

#include "logger.h"

Logger::Logger(const data_pkt& _data, const std::string& _db_filename) {
	data = _data;
	exists = false;
	db_filename = _db_filename;
}
void Logger::print_data() {
	std::cout << "DATA AT SERVER: " << std::endl;
	std::cout << "\t" << std::setw(7) << "ID: " << data.id << std::endl;
	std::cout << "\t" << std::setw(7) << "Flags: " << data.flags << std::endl;
}

// WRITE
void Logger::write() {
	// DEFAULT FLAG
	if (data.flags == flag_type(0b1111)) {
		std::cout << "Default flag" << std::endl;
	}

	// ERROR FLAG
	else if (data.flags == flag_type(0b1110)) {
		std::cout << "Error flag" << std::endl;
	}

	// POP FLAG
	else if (data.flags == flag_type(0b1000)) {
		//std::cout << "Writing pop..." << std::endl; //DEBUG
		write_pop();
	}

	// ATTENDANCE FLAG
	else {
		//std::cout << "Writing log..." << std::endl; //DEBUG
		write_log();
	}
}

// ATTENDANCE LOGGING
bool Logger::check_log() {
	// Get today's filename
	time_t now = std::time(0);
	tm* ltm = localtime(&now);

	std::stringstream _filename{};
	_filename << "../data/logs/attendance/" << 1900 + ltm->tm_year << "/" << months.at(1 + ltm->tm_mon) << "/" << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year << ".csv";
	filename = _filename.str();

	// Try to open file, return status of file's existence
	std::ifstream f(filename);

	bool file_open = f.good();
	f.close();

	return file_open;
}
void Logger::create_log() {
	// Create new file using filename used when checking existence
	std::ofstream f;

	// TODO: DOES NOT CREATE IF PARENT DIRECTORY DOES NOT EXIST, fix with boost?
	f.open(filename);

	// Print header
	std::vector<std::string> header{ "NO", "FIRST", "LAST", "TIME", "DIRECTION", "LOCATION" };

	write_to_csv(header, f);
	f.close();
}
void Logger::write_log() {
	exists = check_log();

	if (!exists) {
		create_log();
	}

	// Line to write to [log].csv
	std::vector<std::string> line{};

	// Get current entry number
	line.push_back(std::to_string(get_entry_num()));

	// Map data.id to username
	std::pair<std::string, std::string> names { get_username() };

	line.push_back(names.first);
	line.push_back(names.second);

	// Get current time
	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::stringstream current_time{};
	current_time << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec; // Might be a bit of weirdness? Constants adding to each field?
	line.push_back(current_time.str());

	// Get direction
	std::string direction{};
	direction = flag_map.at(data.flags).at(0);
	line.push_back(direction);

	// Get location
	std::string location{};
	location = flag_map.at(data.flags).at(1);
	line.push_back(location);

	// Write to log file
	std::ofstream f;
	f.open(filename, std::ofstream::app);
	write_to_csv(line, f);
	f.close();
}


// POP LOGGING
bool Logger::check_pop() {
	// Get today's filename
	time_t now = std::time(0);
	tm* ltm = localtime(&now);

	std::stringstream _filename{};
	_filename << "../data/logs/pop/" << 1900 + ltm->tm_year << "/" << months.at(1 + ltm->tm_mon) << ".csv";
	filename = _filename.str();

	/// Try to open file, return status of file's existence
	std::ifstream f;

	f.open(filename);
	bool file_open = f.good();
	f.close();

	return file_open;
}
void Logger::create_pop() {
	// Create new file using filename used when checking existence
	std::ofstream f;

	// TODO: DOES NOT CREATE IF PARENT DIRECTORY DOES NOT EXIST, fix with boost?
	f.open(filename);

	// Print header
	std::vector<std::string> header{ "NO", "FIRST", "LAST", "DATE", "TIME", "STALE?" };

	write_to_csv(header, f);
	f.close();
}
void Logger::write_pop() {
	exists = check_pop();

	if (!exists) {
		create_pop();
	}

	// Line to write to [log].csv
	std::vector<std::string> line{};

	// Get current entry number
	line.push_back(std::to_string(get_entry_num()));

	// Map data.id to username
	std::pair<std::string, std::string> names { get_username() };

	line.push_back(names.first);
	line.push_back(names.second);

	// Get current date
	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::stringstream current_date{};
	current_date << ltm->tm_mday;
	line.push_back(current_date.str());

	// Get current time
	std::stringstream current_time{};
	current_time << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec; // Might be a bit of weirdness? Constants adding to each field?
	line.push_back(current_time.str());

	// When a pop is logged, it is nonstale (ie., it has not been reported, only logged)
	std::string stale = "False";
	line.push_back(stale);

	// Write to log file
	std::ofstream f;
	f.open(filename, std::ofstream::app);
	write_to_csv(line, f);
	f.close();
}

// DATA RETRIEVAL
std::pair<std::string, std::string> Logger::get_username() {
// Map input ID to db user
	db_interface dbi = db_interface(db_filename);
	std::vector<User> users = dbi.read_users();
	std::vector<id_type> _ids;
	std::vector<id_type>::iterator itr;

	// Scan all users for key ID
	for (auto user : users) {
		_ids = user.get_ids();
		itr = std::find(_ids.begin(), _ids.end(), data.id);

		if (itr != _ids.end()) {
			return user.get_name();
		}
	}

	// ID doesn't exist. Return ID seen.
	//return data.id; //TODO
	return std::make_pair("InvalidID", "InvalidID");
}
unsigned int Logger::get_entry_num() {
	std::ifstream f(filename);
	std::string dummy; // would prefer to not need this. Required for std::getline
	unsigned int entry_num{ 0 };

	while (std::getline(f, dummy)) {
		entry_num++;
	}
	f.close();

	return entry_num;
}
