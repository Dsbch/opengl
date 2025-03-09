#include "pch.h"
#include "file.h"

std::string open_and_read(const std::string& name)
{
	std::ifstream f(name, std::ifstream::in);

	std::stringstream s;

	std::string line;
	while (f)
	{

		if (std::getline(f, line))
		{
			s << line << std::endl;
		}
	}

	f.close();

	return s.str();
}
