#include <string>
#include <iostream>

#include "parser.h"


std::string fetch_value_from_key(std::string input, std::string key)
{
	size_t key_start_pos = input.find(key);
	size_t key_delimit = input.find(':', key_start_pos) + 2;
	size_t key_end_pos = input.find('\n', key_start_pos);

	try
	{
		return input.substr(key_delimit, key_end_pos - key_delimit);
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "Key not found within output!" << std::endl;
		exit(1);
	}
}