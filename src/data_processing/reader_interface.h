/* Purpose:
 * Interface with the RFID reader.
 * 	- Accept id input from reader
 * 	- Package id with flags from interface
 * 		- Flags contain info about reader location and purpose.
 *
 *
 */


#ifndef RFID_LOGISTICS_READER_INTERFACE_H
#define RFID_LOGISTICS_READER_INTERFACE_H

#include "util.h"

class ReaderInterface {
public:
	ReaderInterface();
	ReaderInterface(const id_type&, const flag_type&);

	void update_flags(const flag_type&);

	data_pkt get_data() {
		return data;
	}

	// DEBUG
	void print_data();

private:
	data_pkt data;
};

#endif //RFID_LOGISTICS_READER_INTERFACE_H
