#include <iostream>

#include "app.h"

int main()
{
	try
	{
		app_t app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cin.get();
		return -1;
	}
	return 0;
}
