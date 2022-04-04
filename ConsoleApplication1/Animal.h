#pragma once
#include <iostream>
#include <string>

class Animal {
	std::string element;
	std::string name;

	int health, physicArmor, magicArmor, level, exp;

public:
	Animal();
	const void GetAllData();

	void SetName(const std::string& a);
	std::string GetName();

	void SetPhysicArmor(const int& a);
	int GetPhysicArmor();

	void SetMagicArmor(const int& a);
	int GetMagicArmor();

	void SetLevel(const unsigned int& a);
	void AddExp(const unsigned int& a);
	int GetLevel();

};