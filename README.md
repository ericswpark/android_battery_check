# android_battery_check

Tool to check battery health on Android phones

# Warning

This tool was tested on the following devices:

- Samsung Galaxy S8+ (dream2lte)
- Samsung Note 20 Ultra 5G (c2q)

The health information may be incorrect as it is calculated based on the data sent over by the battery driver, which may or may not be accurate.
In addition, the battery driver implementation is different across Android devices.

# Usage

1. Download the latest release from the "Releases" tab.
2. Make sure the `adb` utility is in your PATH.
3. Enable USB debugging under Developer Options.
4. Connect your phone to your computer using one of the following methods:
    - Wirelessly pair ADB. This is recommended as you do not need cables. Use `adb pair` with the pairing code. Note that this only works on devices with Android 11 or later. Older Android revisions may still be able to connect over Wi-Fi using `adb connect`.
    - Use a cable.
6. Trust the computer if the dialog shows up on your phone.
7. Run the executable in `cmd`.

Note: in some cases, the device may report a higher battery design capacity than the actual value.
(For example, my Note 20 Ultra reported a battery design capacity of 4,500 mAh, when the actual design capacity was 4,370 mAh.)
If you would like to specify a custom design capacity, run the executable with the `-c` flag.

# Building

Brief overview: get CMake, create a `build` directory, configure and generate the project files.

For debugging:

    cmake -H. -Bbuild/Debug -DCMAKE_BUILD_TYPE=Debug

For release:

    cmake -H. -Bbuild/Release -DCMAKE_BUILD_TYPE=Release

Then run a build:

    cmake --build build/Debug
    cmake --build build/Release


# Troubleshooting

If the program crashes or doesn't produce any output, it's due to one of the following:

- The ADB tool and the supporting dependencies are not in your PATH or in the current directory
- Your device is not connected
- ADB is not enabled in developer options
- You didn't "trust" the current connected computer for ADB connections (unauthorized)
- Your device's battery driver returned unexpected values
