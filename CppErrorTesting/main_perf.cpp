#include <vld.h>

#include <iostream>
#include <sstream>
#include "Windows.h"
#include "Error.h"



Error Test1_ss() {
	const char* str = "Another string";

	std::stringstream ss;
	ss << "SS: This is just test 1 " << 223 << " " << str << " " << 0.0;
	return Error(ss.str());
}

Error Test1_fmt() {
	const char* str = "Another string";

	//return Errorf("FT: This is just test 1 %d %s %f", 223, str, 0.0);
	return Errorf("streamsize -> uint conversion failed for file: %s| streamsize: %d | uint: %u", str, 345, 10290);

}

#define Frequency QueryPerformanceFrequency
#define Counter   QueryPerformanceCounter

LARGE_INTEGER GetElaps(LARGE_INTEGER& start, LARGE_INTEGER& end, LARGE_INTEGER& freq) {
	LARGE_INTEGER elaps;
	elaps.QuadPart = end.QuadPart - start.QuadPart;

	elaps.QuadPart *= 1000000;
	elaps.QuadPart /= freq.QuadPart;
	return elaps;
}

int main(void) {
	const unsigned int sampleSize = 1;

	LARGE_INTEGER freq;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER ssTestElaps, fmtTestElaps;

	Frequency(&freq);

	// stringstream test
	if (false) {
		Counter(&start);
		for (int i = 0; i < sampleSize; ++i) {
			Error err = Test1_ss();
			//std::cout << err->Error();
		}
		Counter(&end);
		ssTestElaps = GetElaps(start, end, freq);
	}


	// fmt test
	Counter(&start);
	for (int i = 0; i < sampleSize; ++i) {
		Error err = Test1_fmt();
		std::string s = err;
		std::cout << err;
	}
	Counter(&end);
	fmtTestElaps = GetElaps(start, end, freq);

	//system("cls");

	std::cout << "Sample Size: " << sampleSize << std::endl;
	std::cout << "==================================" << std::endl;
	//std::cout << "| SST |   " << ssTestElaps.QuadPart << std::endl;
	std::cout << "| FMT |   " << fmtTestElaps.QuadPart << std::endl;

	system("pause");

	return 0;
}