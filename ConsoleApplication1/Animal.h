#pragma once
#include <iostream>
#include <string>

class Animal {
	std::string element;
	std::string name;

	int health, physicArmor, magicArmor, level;

public:
	Animal();
	void SetName(std::string a);
	std::string GetName();
};