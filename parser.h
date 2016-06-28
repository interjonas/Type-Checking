#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#pragma once

class scope {
public:
	std::string scopeID;
	std::string publicArray[20];
	std::string privateArray[20];
	scope *parent;
	scope(std::string name)
	{
		scopeID = name;
		parent = NULL;
	}
};
