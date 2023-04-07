This code scans for Iguanavation Bluetooth Modules like the Iguana Smart Sack available at https://www.amazon.com/Smart-Sack-Iguana/dp/B07SYBHXJ1/
If a module is observed, LED 1 is turned on. Then the LED can be turned off by pressing Button 1.

A wireless notification system can be created using one or more modules.
Load the shake awake code into the iguanavation bluetooth modules (the shake awake code for the module is in a separate repo).
Then when someone wants to notify another person, they can pick up a nearby module and shake it.
The LED on this receiver will light up to indicate that someone has created a notification (perhaps as a "help wanted" request)

This repo is for the Nordic nRF52840-DK.
All required Nordic SDK files are included in this repo. (SDK 15.3)
IDE: Segger SES for ARM 7.10a (free to use for Nordic microcontrollers)

