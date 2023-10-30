# WiFi Setup for Windows

## Background Information

Relavent manufacturer home pages:
- [WiFi Kit 32](https://docs.heltec.org/en/node/esp32/wifi_kit_32/index.html)
- [ESP32 development environment](https://docs.heltec.org/en/node/esp32/esp32_general_docs/index.html)

WiFi Kit 32 uses the ESP32 development environment.

WiFi Kit 32 uses a 2.4 Ghz antenna [1].

As of the initial time this page was written, these instructions had only been tested on a x64 Windows 10 machine.

## Initial Setup

Generally follow the [quick start instructions](https://docs.heltec.org/en/node/esp32/esp32_general_docs/quick_start.html). The instructions have some errors or are otherwise 
unclear at various points which the following comments seek to clarify. 

### Installing the USB Driver

To install the USB driver, generally follow the instructions for Windows on the page on 
[establishing serial connection](https://docs.heltec.org/general/establish_serial_connection.html#for-windows). These instructions appear to be somewhat out of date, however,
as clarified below.

Download the CP210x Windows Driver from the The Silicon Labs driver [download page](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads). (This
worked successfully for an x64 Windows 10 machine. The CP210x Universal Windows Driver did _not_ work as it did not include the necessary executable file.)

To determine whether one's machine is x86 or x64, try the following. Open the System Information app (which may be found by typing "system information" in the Start menu 
search bar). Within the System Information app, look for the "System Type" entry.

### Setting Up the Development Environment/Framework

Ignore the first screenshot in the quick start instructions as it is out of place.

_Do not_ click on the links in the bulleted list representing the three methods to install the devlopment framework. These links incorrectly go to a page with instructions for
ESP8266 which is _not_ what we're working with. Instead, just keep scrolling down on the quick start instructions page and follow the instructions found there. 

Of the three methods described, only installing the framework "Via Arduino Board Manager" was tested and it was found to work successfully.

### Example

It is necessary to slect the correct
board. Do this in the Arduino coding environment by selecting `Tools` > `Board` > `Heltec ESP32 Arduino` > `WiFi Kit 32`. (Some of the other options in the final submenu may
also work, `WiFi LoRa 32(V2)` for example.

In selecting an example, the `File` > `Examples` > `Heltec-Example` menu was not present. As an alternative, `File` > `Examples` > `WiFi` > `WiFi Scan` was used.

After uploading the code, you will see a message `Leaving...  Hard resetting via RTS pin...`. This is not an error [2].

To see the output, open the Serial Monitor and use the appropriate drop down menu to set the symbol rate to 115200 [baud](https://en.wikipedia.org/wiki/Baud). 
(This appears to be the symbol rate that should be used for all WiFi Kit 32 applications.)

The program will typically begin running immeditately once the code is uploaded, so when the Serial Monitor is opened, one will not see the output associated with portions
of the progragram that ran _before_ it was opened. To see the output for the entire program, press the `RST` button on the microprocessor to start the program over.

The output should report detected WiFi networks.

## Remotely Controlling a Motor

Edit [`SimpleWiFiServerMotor.ino`](https://github.com/kentchesley/2023_Fall_Labs_Team_1/blob/main/Project/WiFi_Setup/SimpleWiFiServerMotor/SimpleWiFiServerMotor.ino) so that
the `ssid` and `password`  variables represent the credentials for the WiFi network to be used. (It is uncertain how this will work with `eduroam` WiFi since that doesn't have 
a single set
of login credentials, but instead each member of UMD has their own credentials. A hotspot could be used instead if this proves to be an issue.) Then upload the program to the 
board.

Once the program is uploaded, open the Serial Monitor. You should see the IP address for the microcontroller. (If you don't see it, try presseing the `RST` button on the
microcontroller.) Using a web browser on any machine connected to the WiFi netwrork (including Nvidia), go to `http://<IP address>` and use the links there to remotely turn the right motor on the robot on or off.

## References

[1] https://docs.heltec.org/en/node/esp32/wifi_kit_32/index.html.

[2] https://rntlab.com/question/hard-resetting-via-rts-pin/.

