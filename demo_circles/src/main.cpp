// STD
#include <iostream>

// App
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
		return -1;
	}
	return 0;
}
