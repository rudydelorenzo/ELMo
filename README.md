![ELMo Banner](misc/banner.png)
# Welcome to the ELMo Library

## What's ELMo?
ELMo is a library that simplifies communication between ELM327 devices and ESP32 microprocessors. The library also allows this communication to happen synchronously. This allows for code to be written with linear logic to avoid finding yourself in [*callback hell*](http://callbackhell.com/). This library is supposed a barebones, yet easy-to-use middle man between your logic and the ELM327 device.

## How do I get it?
**Method 1: Arduino Library manager**
1. Open the library manager in your IDE of choice (Arduino IDE or PlatformIO)
2. Search for `ELMo`
3. Click install
4. `include` ELMo in your project
5. Profit!

**Method 2: GitHub Releases**
1. Go to the Releases page
2. Look at the latest version, denoted by the ![Latest Release](misc/latest_release.svg) tag
3. Download the source as a `.zip` file
4. Extract the `.zip` file to your `libraries` directory

## How do I use it?
Advanced instructions and function definitions can be found in the [Wiki](https://github.com/rudydelorenzo/ELMo/wiki). To get started, however, all you need to do is:
1. Create an ELMo instance with `ELMo ELM;`
2. Customize any of the settings if needed, or give the defaults a shot
3. Initialize the connection to the device with `ELM.initialize();`
4. Send messages to the device using `ELM.send("message");`

## License
This project is licensed under the MIT License.
