/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Define the RBG LED pins
#define LEDR (22u)
#define LEDG (23u)
#define LEDB (24u)
// Define the AUDIO pin
#define AUDIO (3u)
// Define the wake up pin for the LoPy
#define WAKE_UP (5u) // This is digital pin 5

// This constant is the chip select pin number for the SPI communaction between the
// Arduino Nano 33 BLE and the SD card reader module 
const int CS = 10;
// This constant is the amount of features needed to calculate in the model
const int FEATURE_AMOUNT = 46;
// This constant is the size of the name used to save the fall signal on the SD card
const int NAME_SIZE = 10;
// This constant is the size of the signal, that we want to calculate features on
const int SIGNAL_SIZE = 400;
// The threshold off the signal vector magnitude
const float THRESHOLD = 1.5;

#endif  // CONSTANTS_H_
