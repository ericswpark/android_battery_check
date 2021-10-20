#include <iostream>

#include "cmd.h"
#include "parser.h"
#include "version.h"

void print_statistics(double battery_health, double max_capacity, double design_capacity, int level)
{
	// Set output precision
	std::cout.precision(4);		// 2 decimal places (xx.xx)

	std::cout << "=> Estimated battery health: " << battery_health << "%" << std::endl;
	std::cout << std::endl;

	// Output detailed statistics
	std::cout << "These results were calculated with the following raw data:" << std::endl;
	std::cout << "=> Current max capacity: " << max_capacity << "mAh" << std::endl;
	std::cout << "=> Battery design capacity: " << design_capacity << "mAh" << std::endl;
	std::cout << "=> Current charge level: " << level << "%" << std::endl;
	std::cout << std::endl;
}

void exit_confirm()
{
	std::cout << "Press Enter to exit." << std::endl;
	std::cin.get();
}

void print_version_info()
{
	std::cout << "android_battery_check v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
}

int main(int argc, char **argv)
{
	print_version_info();

	// Get output from ADB
	std::string dumpsys_output = exec("adb shell dumpsys battery");
	std::string sysclass_charge_full_output = exec("adb shell cat /sys/class/power_supply/battery/charge_full");

	// Calculate battery statistics
	int level, current_charge_level, battery_design_capacity;

	try
	{
		level = std::stoi(fetch_value_from_key(dumpsys_output, "level:"));
		current_charge_level = std::stoi(fetch_value_from_key(dumpsys_output, "Charge counter:"));
	}
	catch (const std::out_of_range& oor)
	{
		std::cerr << "Key not found within output! Check the adb output above." << std::endl;
		exit_confirm();
		exit(1);
	}

	// Add check for charge level
	if (current_charge_level == 0)
	{
		std::cout << "Warning: your device reported an invalid charge level value." << std::endl;
		std::cout << "Your device's battery driver may not support reporting the battery capacity." << std::endl;
		std::cout << "Statistics calculated by this tool may be inaccurate." << std::endl;
	}

	
	if (argc == 2 && strcmp(argv[1], "-c") == 0)
	{
		std::cout << "Enter custom battery design capacity: ";
		std::cin >> battery_design_capacity;
		std::cin.ignore();
	}
	else
	{
		try
		{
			battery_design_capacity = std::stoi(sysclass_charge_full_output) / 1000;
		}
		catch (const std::invalid_argument& ia)
		{
			std::cerr << "Error parsing battery design capacity. Your device may not support reporting this value through the battery driver." << std::endl;
			std::cerr << "Please manually define the battery design capacity using the `-c` flag." << std::endl;
			exit_confirm();
			exit(1);
		}
	}
	
	double current_max_capacity = (double)current_charge_level / (double)level * 100 / 1000;
	double battery_health = current_max_capacity / (double)battery_design_capacity * 100;

	print_statistics(battery_health, current_max_capacity, battery_design_capacity, level);

	// In case the user directly ran the program, stop at end and wait for input
	exit_confirm();
}
