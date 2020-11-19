#include <iostream>
#include <bitset>
#include <ctime>

#include "PolynomialBasis.h"

int main()
{
    srand(time(NULL));
	
	std::string number;

	std::cout << "Input number A: ";
	std::cin >> number;
	auto numberA = std::make_shared<fieldElement>(number);

	std::cout << "Input number B: ";
	std::cin >> number;
	auto numberB = std::make_shared<fieldElement>(number);

	auto generator = std::make_shared<fieldElement>("100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000001");

	std::cout << "Result of addition: ";
	auto addition = PolAdd(numberA, numberB);
	std::cout << addition->bitString << std::endl;
}