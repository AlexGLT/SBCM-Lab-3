#include <iostream>
#include <memory>

struct fieldElement
{
	int size;

	int* value;

	std::string bitString;
	std::string hexString;

	fieldElement();
	fieldElement(int size);
	fieldElement(std::string str);
	
	~fieldElement();
};

void showBigInteger(std::shared_ptr<fieldElement> number, std::string numberName);

std::shared_ptr<fieldElement> ZeroEraser(std::shared_ptr<fieldElement> number);

void FieldStandart(std::shared_ptr<fieldElement> number);

std::shared_ptr<fieldElement> PolAdd(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement);
std::shared_ptr<fieldElement> PolDiv(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement);
std::shared_ptr<fieldElement> PolMul(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement, std::shared_ptr<fieldElement> generator, bool text = true);
std::shared_ptr<fieldElement> PolSquare(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> generator, bool text = true);
std::shared_ptr<fieldElement> PolPow(std::shared_ptr<fieldElement> numberA, std::shared_ptr<fieldElement> numberB, std::shared_ptr<fieldElement> generator);