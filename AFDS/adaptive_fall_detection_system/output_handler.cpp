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

/**
  *
  *
  * @file:    output_handler.cpp
  * @date:    26-03-2020 09:32:41
  * @author:  TensorFlow Authors + Morten Sahlertz
  *
  * @brief    Handler for handling the prediction of whether or not a fall has occured and the outputs to the User Interface and the LoPy
  *
  *
  *
  *
**/

#include "output_handler.h"

#include "Arduino.h"
#include "constants.h"

// Track whether the function has run at least once
bool initialized = false; /**<  Bool for checking whether or not, the function has run at least once   */

void start_audio() {
  // Start Audio PWM signal
  analogWrite(AUDIO, 50);
}

void stop_audio() {
  // Stop Audio PWM signal
  analogWrite(AUDIO, 0);
}

void wake_up_lopy() {
  // Set Wake up pin high
  digitalWrite(WAKE_UP, HIGH); 
}

void stop_wake_up_lopy() {
  // Set Wake up pin low
  digitalWrite(WAKE_UP, LOW); 
}

void HandleOutput(tflite::ErrorReporter* error_reporter, int prediction) {
  // Do this only once
  if (!initialized) {
    // Set the LED pin to output
    pinMode(WAKE_UP, OUTPUT);
    pinMode(AUDIO, OUTPUT);
    initialized = true;
  }

  if (prediction == 1) {
    // Set LoPy wake up pin to high, to wake the LoPy
    wake_up_lopy();   
    // Turn on green LED and Audio to indicate a fall has happened
    digitalWrite(LEDG, LOW);
    start_audio();
  }
  // If a fall wasn't predicted the output handler returns to the loop, and starts collecting data again
  else {
    return;
  }
}
