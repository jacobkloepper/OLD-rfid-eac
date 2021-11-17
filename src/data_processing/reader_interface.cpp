/* Purpose:
 * Interface with the RFID reader.
 * 	- Accept id input from reader
 * 	- Package id with flags from interface
 * 		- Flags contain info about reader location and purpose.
 *
 *
 */

 /* TODO:
  * 	1.
  *
  */

#include "reader_interface.h"

ReaderInterface::ReaderInterface() {
	data.id = id_type(0);
	data.flags = flag_type(0b1111);
}

ReaderInterface::ReaderInterface(const id_type& _id, const flag_type& _flags) {
	data.id = _id;
	data.flags = _flags;
}

void ReaderInterface::update_flags(const flag_type& _flags) {
	data.flags = _flags;
}

void ReaderInterface::print_data() {
	std::cout << "DATA LEAVING INTERFACE: " << std::endl;
	std::cout << "\t" << std::setw(7) << "ID: " << data.id << std::endl;
	std::cout << "\t" << std::setw(7) << "Flags: " << data.flags << std::endl;
}
