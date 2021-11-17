#ifndef RFID_LOGISTICS_UTIL_H
#define RFID_LOGISTICS_UTIL_H

#include <string>
#include <bitset>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <unordered_map>
#include <map>
#include <ctime>
#include <filesystem>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <windows.h>

// DATA CONSTANTS AND TYPES
const bool DEBUG = false;
const int UID_BYTES = 4;
const int UID_SIZE = UID_BYTES * 8;
const int FLAG_SIZE = 4;
typedef std::bitset<UID_SIZE> id_type;
typedef std::bitset<FLAG_SIZE> flag_type;
struct data_pkt {
	id_type id;
	flag_type flags;
};

// DATA MAPS
static std::unordered_map<flag_type, std::vector<std::string>> flag_map{
	{flag_type(0b0000), {"exit", "main"}},
	{flag_type(0b0001), {"exit", "fishbowl"}},
	{flag_type(0b0010), {"exit", "reception"}},
	{flag_type(0b0011), {"exit", "staircase"}},
	{flag_type(0b0100), {"enter", "main"}},
	{flag_type(0b0101), {"enter", "fishbowl"}},
	{flag_type(0b0110), {"enter", "reception"}},
	{flag_type(0b1000), {"pop"}},
	{flag_type(0b1110), {"error"}},
	{flag_type(0b1111), {"default"}}
};
static std::map<uint8_t, std::string> months{
	{1, "January"},
	{2, "February"},
	{3, "March"},
	{4, "April"},
	{5, "May"},
	{6, "June"},
	{7, "July"},
	{8, "August"},
	{9, "September"},
	{10, "October"},
	{11, "November"},
	{12, "December"}
};

// DATA REPRESENTATION
inline std::bitset<4> hex_to_bin(char ch) {
	std::bitset<4> bits;
	bits.reset();

	ch = toupper(ch);

	switch (ch) {
	case '0':
		bits.reset();
		break;
	case '1':
		bits.set(0);
		break;
	case '2':
		bits.set(1);
		break;
	case '3':
		bits.set(0);
		bits.set(1);
		break;
	case '4':
		bits.set(2);
		break;
	case '5':
		bits.set(0);
		bits.set(2);
		break;
	case '6':
		bits.set(1);
		bits.set(2);
		break;
	case '7':
		bits.set(0);
		bits.set(1);
		bits.set(2);
		break;
	case '8':
		bits.set(3);
		break;
	case '9':
		bits.set(0);
		bits.set(3);
		break;
	case 'A':
		bits.set(1);
		bits.set(3);
		break;
	case 'B':
		bits.set(0);
		bits.set(1);
		bits.set(3);
		break;
	case 'C':
		bits.set(2);
		bits.set(3);
		break;
	case 'D':
		bits.set(0);
		bits.set(2);
		bits.set(3);
		break;
	case 'E':
		bits.set(1);
		bits.set(2);
		bits.set(3);
		break;
	case 'F':
		bits.set(0);
		bits.set(1);
		bits.set(2);
		bits.set(3);
		break;
	default:
		std::cout << "Invalid hex input: [" << ch << "]" << std::endl;
		//throw std::runtime_error("invalid hex input");
	}

	return bits;
}
inline id_type hex_to_id(const std::string& hex) {
	int hex_num{ 0 };
	id_type _id{};

	for (const auto& ch : hex) {
		std::bitset<4> hexbit = hex_to_bin(ch);
		for (int i{ 0 }; i < 4; i++) {
			if (hexbit[i] == 1) {
				_id.set(UID_SIZE - ((4 - i) + hex_num * 4));
			}
		}
		hex_num++;
	}

	return _id;
}

// STRING PARSING
inline std::vector<std::string> tokenize(const std::string& message, const std::string& delim) {
	// Given a string message and a string delimiter, split the message by the delimiter and return it as a vector of strings.
	// Each token ends in the delimiter (ie., message = "this/is/a/message/", delim = "/", toks = {"this", "is", "a", "message"})
	std::string token{};
	std::vector<std::string> toks{};
	
	size_t start_pos{ 0 };
	size_t end_pos{ 0 };

	// Split line based on delimiter
	end_pos = message.find(delim);
	while (end_pos != std::string::npos) {
		token = message.substr(start_pos, end_pos - start_pos);
		toks.push_back(token);

		start_pos = end_pos + delim.length();
		end_pos = message.find(delim, start_pos);
	}

	return toks;
}

// SERIAL (WINDOWS)
inline unsigned long long binary_str_to_dec_ull(const std::string& str) {
	// Given a string of binary, return the decimal representation.
	unsigned int _val{ 0 };

	if (str.size() > 64) {
		std::cout << "Bitstring too large (size > 16). Return 0" << std::endl;
		return 0;
	}
	for (size_t i{ 0 }; i < str.size(); i++) {
		if (str.at(str.size() - 1 - i) == '1') {
			_val += std::pow(2, i);
		}
	}

	return _val;
}
inline std::pair<id_type, flag_type> parse_serial(uint8_t* data) {
	// Given a pointer to a data packet, parse the packet into id and flags.
	std::pair<id_type, flag_type> _tup{};

	// Extract UID info
	std::string _id_str{};
	for (size_t i{ 0 }; i < UID_SIZE; i++) {
		_id_str.push_back(data[i]);
	}
	_tup.first = id_type(binary_str_to_dec_ull(_id_str));

	// Extract flag info
	std::string _flag_str{};
	for (size_t i{ 0 }; i < FLAG_SIZE; i++) {
		_flag_str.push_back(data[i + UID_SIZE]);
	}
	_tup.second = flag_type(binary_str_to_dec_ull(_flag_str));

	return _tup;
}
inline void read_serial(std::pair<id_type, flag_type>& data_ptr) {
	// Provided a pointer to a string, read from serial in (COM3), parse the data, and stick it in that pointer.
	// Open serial port
	HANDLE serialHandle;
	serialHandle = CreateFile("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	// Do some basic settings
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);

	GetCommState(serialHandle, &serialParams);
	serialParams.BaudRate = 115200;
	serialParams.ByteSize = 8;
	serialParams.StopBits = TWOSTOPBITS;
	serialParams.Parity = NOPARITY;
	SetCommState(serialHandle, &serialParams);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 50;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;

	SetCommTimeouts(serialHandle, &timeout);

	DWORD dwRead;
	BOOL waiting = FALSE;
	uint8_t buff[100]{ 0 };
	OVERLAPPED osReader{ 0 };

	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL) {
		std::cout << "Error creating osreader, abort" << std::endl;
		return;
	}

	PurgeComm(serialHandle, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);
	if (!waiting) {
		// Read
		if (!ReadFile(serialHandle, buff, UID_SIZE + FLAG_SIZE, &dwRead, NULL)) {
			if (GetLastError() != ERROR_IO_PENDING) {
				// ERROR
				std::cout << "Error in ReadFile()" << std::endl;
				//throw std::runtime_error("Error in ReadFile()");
			}
			else {
				waiting = TRUE;
			}
		}
		else {
			// read correctly
			data_ptr = parse_serial(buff);
		}
	}

	CloseHandle(serialHandle);
}

// WRITING (CSV)
template <class T>
inline void write_to_csv(const T& ctr, std::ofstream& f) {
	// Write a line whose elements are in a container to a file formatted as a .csv
	// For debugging, elements also piped to cout
	for (auto it = ctr.begin(); it != ctr.end(); it++) {
		f << *it;
		std::cout << *it;
		if (it != ctr.end() - 1) {
			f << ",";
			std::cout << ",";
		}
	}
	f << std::endl;
	std::cout << std::endl;
}


#endif //RFID_LOGISTICS_UTIL_H
