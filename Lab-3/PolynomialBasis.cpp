#include "PolynomialBasis.h"

fieldElement::fieldElement()
{
	bitString = "0";

	size = 1;

	value = new int{ 1 };
}

fieldElement::fieldElement(int size)
{
	std::string bitString = "";
	
	this->size = size;

	value = new int[size];
}

fieldElement::fieldElement(std::string str)
{
	bitString = str;
	size = str.size();

	value = new int[size];

	for (int i = 0; i < size; i++)
	{
		value[i] = static_cast<int>(str[i]) - 48;
	}
}

fieldElement::~fieldElement()
{
	delete[] value;
	value = nullptr;
}

void showBigInteger(std::shared_ptr<fieldElement> number, std::string numberName)
{
	std::cout << numberName << ": " << std::endl;

	std::cout << "|";

	for (int i = 0; i < number->size; i++)
	{
		std::cout << number->value[i] << "|";
	}

	std::cout << std::endl;
}

void ZeroEraser(std::shared_ptr<fieldElement> number)
{
	int zeroCount = 0;

	for (int i = 0; i < number->size - 191; i++)
	{
		if (number->value[i] == 0)
		{
			zeroCount++;
		}
		else
		{
			break;
		}
	}

	if (number->size - zeroCount == 0)
	{
		zeroCount--;
	}

	if (zeroCount > 0)
	{
		int newSize = number->size - zeroCount;

		auto* optimizedNumberValue = new int[newSize];
		std::fill(&optimizedNumberValue[0], &optimizedNumberValue[newSize], 0);
		std::copy(number->value + zeroCount, number->value + number->size, optimizedNumberValue);

		delete[] number->value;
		number->value = nullptr;

		number->size = newSize;
		number->value = optimizedNumberValue;
	}
}

std::shared_ptr<fieldElement> PolAdd(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement)
{
	auto sumElement = std::make_shared<fieldElement>(firstElement->size);

	for (int i = 0; i < 191; i++)
	{
		sumElement->value[i] = (firstElement->value[i] + secondElement->value[i]) % 2;

		sumElement->bitString += static_cast<char>(sumElement->value[i] + 48);
	}

	return sumElement;
}