#include <iostream>

#include "cmd.h"
#include "parser.h"
#include "version.h"

void print_statistics(double battery_health, double max_capacity, double design_capacity, int level, int battery_cycle_count)
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

    // Output cycle count
    std::cout << "=> Current battery cycle count: " << battery_cycle_count << std::endl;
    std::cout << std::endl;
}

void print_health_estimate(double battery_health, double battery_cycle_count)
{
    std::string current_health = battery_health <= 80 ? "bad" : "good";

    std::cout << "Your battery is currently in a " << current_health << " state." << std::endl;
    if (current_health == "bad" || battery_cycle_count >= 500)
    {
        std::cout << "You should consider replacing your battery soon." << std::endl;
    }

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
    std::string sysclass_battery_cycle_output = exec("adb shell cat /sys/class/power_supply/battery/battery_cycle");

	// Calculate battery statistics
	int level, current_charge_level, battery_design_capacity, battery_cycle_count;

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
    
    // Calculate optional cycle count
    try
    {
        battery_cycle_count = std::stoi(sysclass_battery_cycle_output);
    }
    catch (const std::out_of_range& oor)
    {
        std::cerr << "Warning: failed to fetch the battery cycle count." << std::endl;
    }
	catch (...)
	{
		std::cerr << "Warning: an unknown error occurred while parsing the battery cycle count." << std::endl;
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

	print_statistics(battery_health, current_max_capacity, battery_design_capacity, level, battery_cycle_count);
    print_health_estimate(battery_health, battery_cycle_count);

	// In case the user directly ran the program, stop at end and wait for input
	exit_confirm();
}
