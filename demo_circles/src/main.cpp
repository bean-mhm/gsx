#include <iostream>

#include "app.cpp"

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
	}
    return 0;
}
