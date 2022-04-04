#include <iostream>
#include <string>
#include "Animal.h"

Animal::Animal() : health(100), physicArmor(0), magicArmor(0), level(1) {}
void Animal::SetName(std::string a) { name = a; }
std::string Animal::GetName() { return name; }