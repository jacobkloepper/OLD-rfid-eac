#include "util.h"
#include "user.h"
#include "reader_interface.h"
#include "db_interface.h"
#include "logger.h"

int main() {
	// LOCAL PATHS
	std::string db_path {"data/user_data/users.db"};
	std::string poplog_path {"data/logs/pop"};
	std::string log_path {"data/logs/attendance"};

	std::cout << "Starting" << std::endl;

	while(true) {
		// Scan RFID reader, obtain UID and flags
		std::pair<id_type, flag_type> data;
		read_serial(data); 

		// START DEBUG
		// Zero ID: serial timeout
		if (data.first == id_type(0)) {
			continue;
		}
		// STOP ID: end main loop
		if (data.first == id_type(75287082)) {
			std::cout << "Stop receieved!" << std::endl;
			break;
		}
		// END DEBUG

		// Simulates the reader reading the RFID tag and sending ID to MCU
		ReaderInterface RI = ReaderInterface(data.first, data.second);

		// Simulates the server receiving the data packet
		Logger logger = Logger(RI.get_data(), db_path);

		// Simulates the server using the data packet to update the log file (outputs)
		logger.write();
	}

	std::cout << "Done." << std::endl;
    return 0;
}