#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include "TCPTest.h"
#include <string>
#include <vector>

int main() {
	std::cout << "Testing TCP\n";
	TCPTest::test_tcp("127.0.0.1", "80");
	return EXIT_SUCCESS;
}