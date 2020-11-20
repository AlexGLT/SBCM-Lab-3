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

	std::cout << "Input number C: ";
	std::cin >> number;
	auto numberС = std::make_shared<fieldElement>(number);

	auto generator = std::make_shared<fieldElement>("100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000001");

	std::cout << "Result of addition: ";
	auto addition = PolAdd(numberA, numberB);
	std::cout << addition->bitString << std::endl;

	std::cout << "Result of multiplication: ";
	auto multiplication = PolMul(numberA, numberB, generator);
	std::cout << multiplication->bitString << std::endl;

	std::cout << "Result of square: ";
	auto square = PolSquare(numberA, generator);
	std::cout << square->bitString << std::endl;

	std::cout << "Result of pow: ";
	auto pow = PolPow(numberA, numberС, generator);
	std::cout << pow->bitString << std::endl;

	std::cout << "Inverse element of numberA: ";
	auto inv = PolInv(numberA, generator);
	std::cout << inv->bitString << std::endl;
}