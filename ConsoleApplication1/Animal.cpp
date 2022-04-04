#include <iostream>
#include <string>
#include <math.h>
#include "Animal.h"

Animal::Animal() : health(100), physicArmor(0), magicArmor(0), level(1), exp(0) {}
const void Animal::GetAllData() {
	std::cout << "Name: " << name << std::endl;
	std::cout << "Health: " << health << std::endl;
	std::cout << "Physic Armor: " << physicArmor << std::endl;
	std::cout << "Magic Armor: " << magicArmor << std::endl;
	std::cout << "Level: " << level << std::endl;
	std::cout << "Experience: " << exp << "/" << pow(2,level) << std::endl;
}


void Animal::SetName(const std::string& a) { name = a; }
std::string Animal::GetName() { return name; }

void Animal::SetPhysicArmor(const int& a) { physicArmor = a; }
int Animal::GetPhysicArmor() { return physicArmor; }

void Animal::SetMagicArmor(const int& a) { magicArmor = a; }
int Animal::GetMagicArmor() { return magicArmor; }

void Animal::AddExp(const unsigned int& a) { 
	exp += a;
	if (exp == pow(2, level)) {
		level++;
	}
}
void Animal::SetLevel(const unsigned int& a) { level = a; }
int Animal::GetLevel() { return level; }

