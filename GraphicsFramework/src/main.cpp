#include <iostream>
#include <fstream>
#include <string>

int main()
{
	std::fstream file("data/logs/test.log");

	std::cout << "Hello World!" << std::endl;

	if (file.is_open())
	{
		std::string line;
		std::getline(file, line);

		std::cout << line << std::endl;
	}

	else
	{
		std::cout << "could not open file!";
	}

	std::cin.get();
}