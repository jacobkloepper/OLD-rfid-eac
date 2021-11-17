#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>

int main() {

	// Hard-coded: MY Paradox key fob's raw, FSK-demodulated, manchester-encoded output (96 bits) from least to most significant bits
	std::string bitstring{"000011110101010101010101010101011010101010101010010110010110100110011010100101100101010110011010"};
	std::reverse(bitstring.begin(), bitstring.end()); // We have to reverse the above string to be read as a number

	std::bitset<96> bits {bitstring};


	uint32_t hi2 = 0, hi = 0, lo = 0;

	// The first byte is a preamble, so skip it.
	// This for-loop decodes the bitstring
	for (uint8_t i = 8; i < 96; i += 2) {

		// not manchester data
		if (bits[i] == bits[i + 1]) {
			std::cout << "error on man-bit ["<< int(i) << "]" << std::endl;
		}

		// Shift each word by one. Transition between words.
		hi2 = (hi2 << 1) | (hi >> 31);
		hi = (hi << 1) | (lo >> 31);
		lo <<= 1;

		// Whenever there is a HI-LO transition, count it as HI in the decoded bitstream
		if (bits[i] && !bits[i+1]) {
			lo |= 1;  // 10
		}
	}

	// uid is bits 10-33
	uint32_t uid = ((hi & 0x3) << 26 | (lo >> 10));
	// fc is bits 26-33
	uint32_t fc = ((hi & 0x3) << 6) | (lo >> 26);
	// cardnum is bits 10-25
	uint32_t cardnum = (lo >> 10) & 0xFFFF;
	// chksum is bits 2-9
	uint8_t chksum = (lo >> 2) & 0xFF;


	// Debug
	std::cout << "hi2: " << std::bitset<32>(hi2) << std::endl;
	std::cout << " hi: " << std::bitset<32>(hi) << std::endl;
	std::cout << " lo: " << std::bitset<32>(lo) << std::endl;
	std::cout << std::endl;

	// Final output
	std::cout << "Paradox - ID: " << std::hex << (hi >> 10) << std::hex << std::setw(8) << std::setfill('0') << ((hi & 0x3) << 26 | (lo >> 10));
	std::cout << " FC: " << std::dec << fc;
	std::cout << " Card: " << std::dec << cardnum;
	std::cout << ", Checksum: " << std::hex << int(chksum);
	std::cout << std::endl;
	return 0;
}