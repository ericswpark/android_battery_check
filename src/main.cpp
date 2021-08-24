#include <iostream>
#include <format>

#include "cmd.h"
#include "parser.h"

void print_statistics(double battery_health, double max_capacity, double design_capacity, int level)
{
	// Set output precision
	std::cout.precision(4);		// 2 decimal places (xx.xx)

	std::cout << "Estimated battery health: " << battery_health << "%" << std::endl;
	std::cout << std::endl;

	// Output detailed statistics
	std::cout << "These results were calculated with the following raw data:" << std::endl;
	std::cout << "Current max capacity: " << max_capacity << "mAh" << std::endl;
	std::cout << "Battery design capacity: " << design_capacity << "mAh" << std::endl;
	std::cout << "Current charge level: " << level << "%" << std::endl;
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	// Get output from ADB
	std::string dumpsys_output = exec("adb shell dumpsys battery");
	std::string sysclass_charge_full_output = exec("adb shell cat /sys/class/power_supply/battery/charge_full");

	// Calculate battery statistics
	int level = std::stoi(fetch_value_from_key(dumpsys_output, "level:"));
	int current_charge_level = std::stoi(fetch_value_from_key(dumpsys_output, "Charge counter:"));
	int battery_design_capacity = -1;
	try
	{
		battery_design_capacity = std::stoi(sysclass_charge_full_output) / 1000;
	}
	catch (const std::invalid_argument& ia)
	{
		std::cerr << "Error parsing battery design capacity." << std::endl;
		return 1;
	}
	
	double current_max_capacity = (double)current_charge_level / (double)level * 100 / 1000;
	double battery_health = current_max_capacity / (double)battery_design_capacity * 100;

	print_statistics(battery_health, current_max_capacity, battery_design_capacity, level);

	// Check custom argument
	if (argc == 2 && strcmp(argv[1], "-c") == 0)
	{
		// Enter custom amount
		std::cout << "Recalculating health based on custom battery design capacity." << std::endl;
		std::cout << "Enter custom battery design capacity: ";
		int custom_battery_design_capacity;
		std::cin >> custom_battery_design_capacity;

		double custom_battery_health = current_max_capacity / (double)custom_battery_design_capacity * 100;

		print_statistics(custom_battery_health, current_max_capacity, custom_battery_design_capacity, level);
	}

	// In case the user directly ran the program, stop at end and wait for input
	std::cout << "Press Enter to exit." << std::endl;
	std::cin.get();
}
