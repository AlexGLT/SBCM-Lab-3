#include "PolynomialBasis.h"

fieldElement::fieldElement()
{
	bitString = "1";

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

	for (int i = 0; i < number->size; i++)
	{
		std::cout << number->value[i];
	}

	std::cout << std::endl;
}

std::shared_ptr<fieldElement> ZeroEraser(std::shared_ptr<fieldElement> number)
{
	int zeroCount = 0;

	for (int i = 0; i < number->size; i++)
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

	if (zeroCount > 0)
	{
		auto optimizedNumber = std::make_shared<fieldElement>(number->size - zeroCount);

		std::copy(number->value + zeroCount, number->value + number->size, optimizedNumber->value);

		return optimizedNumber;
	}

	return number;
}

void FieldStandart(std::shared_ptr<fieldElement> number)
{
	int zeroCount = 191 - number->size;

	if (zeroCount > 0)
	{
		int newSize = number->size + zeroCount;

		auto* optimizedNumberValue = new int[newSize];
		std::fill(&optimizedNumberValue[0], &optimizedNumberValue[zeroCount], 0);
		std::copy(number->value, number->value + number->size, optimizedNumberValue + zeroCount);

		delete[] number->value;
		number->value = nullptr;

		number->size = newSize;
		number->value = optimizedNumberValue;
	}
}

bool LongComp(std::shared_ptr<fieldElement> numberA, std::shared_ptr<fieldElement> numberB, bool severe)
{
	if (numberA->size < numberB->size)
		return false;
	if (numberA->size > numberB->size)
		return true;

	for (long long i = 0; i < numberA->size; i++)
	{
		if (numberA->value[i] >= numberB->value[i])
		{
			return true;
		}
		else if (numberA->value[i] < numberB->value[i])
		{
			return false;
		}
		else
		{
			continue;
		}
	}

	switch (severe)
	{
	case false:
		return true;

	case true:
		return false;
	}
}

std::shared_ptr<fieldElement> LongShiftBits(std::shared_ptr<fieldElement> number, int shift)
{
	if (number->size == 1 && shift < 0)
	{
		return number;
	}

	auto highNumber = std::make_shared<fieldElement>(number->size + shift);

	if (shift > 0)
	{
		std::fill(&highNumber->value[number->size], &highNumber->value[highNumber->size], 0);
		shift = 0;
	}

	std::copy(number->value, number->value + number->size + shift, highNumber->value);

	return highNumber;
}

std::shared_ptr<fieldElement> PolAdd(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement)
{
	auto sumElement = std::make_shared<fieldElement>(firstElement->size);

	for (int i = 0; i < firstElement->size; i++)
	{
		sumElement->value[i] = (firstElement->value[i] + secondElement->value[i]) % 2;

		sumElement->bitString += static_cast<char>(sumElement->value[i] + 48);
	}

	return sumElement;
}

std::shared_ptr<fieldElement> PolDiv(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement)
{
	auto remainder = std::make_shared<fieldElement>(firstElement->size);
	std::copy(firstElement->value, firstElement->value + firstElement->size, remainder->value);

	auto divisor = ZeroEraser(secondElement);

	while (LongComp(remainder, divisor, false))
	{
		auto maxDivisor = LongShiftBits(divisor, (remainder->size - divisor->size));

		remainder = PolAdd(remainder, maxDivisor);

		remainder = ZeroEraser(remainder);
	}

// 	for (int i = 0; i < divisor->size; i++)
// 	{
// 		if (remainder->value[i] == divisor->value[i])
// 		{
// 			continue;
// 		}
// 		else if (i == divisor->size - 1)
// 		{
// 			remainder = PolAdd(remainder, divisor);
// 		}
// 		else
// 		{
// 			break;
// 		}
// 	}

	FieldStandart(remainder);

	return remainder;
}

std::shared_ptr<fieldElement> PolMul(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> secondElement, std::shared_ptr<fieldElement> generator, bool text)
{
	auto numberA = ZeroEraser(firstElement);

	auto numberB = ZeroEraser(secondElement);

	auto numberC = std::make_shared<fieldElement>(numberA->size + numberB->size - 1);
	std::fill(&numberC->value[0], &numberC->value[numberC->size], 0);

	for (int i = numberB->size - 1; i >= 0; i--)
	{
		auto intermediateMul = std::make_shared<fieldElement>(numberA->size + numberB->size);
		std::fill(&intermediateMul->value[0], &intermediateMul->value[intermediateMul->size], 0);

		for (int j = numberA->size - 1; j >= 0; j--)
		{
			intermediateMul->value[j + i] = static_cast<unsigned long long>(numberA->value[j]) * static_cast<unsigned long long>(numberB->value[i]);
		}

		numberC = PolAdd(numberC, intermediateMul);
	}

	numberC = ZeroEraser(numberC);

	numberC = PolDiv(numberC, generator);

	numberC->bitString = "";

	if (text)
	{
		numberC->bitString = "";

		for (int i = 0; i < numberC->size; i++)
		{
			numberC->bitString += static_cast<char>(numberC->value[i] + 48);
		}
	}

	return numberC;
}

std::shared_ptr<fieldElement> PolSquare(std::shared_ptr<fieldElement> firstElement, std::shared_ptr<fieldElement> generator, bool text)
{
	auto numberC = std::make_shared<fieldElement>(firstElement->size * 2 - 1);
	std::fill(&numberC->value[0], &numberC->value[numberC->size], 0);

	for (int i = 0; i < firstElement->size; i++)
	{
		numberC->value[2 * i] = firstElement->value[i];
	}

	numberC = PolDiv(numberC, generator);

	numberC->bitString = "";

	if (text)
	{
		numberC->bitString = "";

		for (int i = 0; i < numberC->size; i++)
		{
			numberC->bitString += static_cast<char>(numberC->value[i] + 48);
		}
	}

	return numberC;
}

std::shared_ptr<fieldElement> PolPow(std::shared_ptr<fieldElement> numberA, std::shared_ptr<fieldElement> numberB, std::shared_ptr<fieldElement> generator, bool text)
{
	auto numberC = std::make_shared<fieldElement>();

	for (long long i = 0; i < numberB->size; i++)
	{
		if (numberB->value[i] == 1)
		{
			numberC = PolMul(numberC, numberA, generator, false);
		}

		if (i != numberB->size - 1)
		{
			numberC = PolSquare(numberC, generator, false);
		}
	}

	if (text)
	{
		numberC->bitString = "";
	
		for (int i = 0; i < numberC->size; i++)
		{
			numberC->bitString += static_cast<char>(numberC->value[i] + 48);
		}
	}

	return numberC;
}

std::shared_ptr<fieldElement> PolInv(std::shared_ptr<fieldElement> numberA, std::shared_ptr<fieldElement> generator)
{
	std::string pow;

	for (int i = 0; i < 191; i ++)
	{
		if (i == 190)
		{
			pow += "0";
		}
		else
		{
			pow += "1";
		}
	}

	return PolPow(numberA, std::make_shared<fieldElement>(pow), generator);
}

char PolTr(std::shared_ptr<fieldElement> numberA, std::shared_ptr<fieldElement> generator)
{
	auto traceC = numberA;
	auto conjugateC = numberA;

	for (int i = 0; i < 190; i++)
	{
		conjugateC = PolSquare(conjugateC, generator, false);

		traceC = PolAdd(traceC, conjugateC);
	}

	if (traceC->value[190] == 0)
	{
		return '0';
	}
	else if (traceC->value[190] == 1)
	{
		return '1';
	}

	return 'F';
}