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
  * @file:    main_functions.h
  * @date:    26-03-2020 09:32:41
  * @author:  TensorFlow Authors + Morten Sahlertz
  *
  * @brief    Main functions setup() and loop() for the system
  *
  *
  *
  *
**/

#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

// Initializes all data needed. The name is important, and needs
// to be setup() for Arduino compatibility.
/**
* @brief  setup(): Setup all the needed ports, the TensorFlow Lite model and rest of the data for the system to run
*
*   Setups all the needed parts for the system to run:
*   
*   1: Starts the Serial connections. (One for LoPy and one for debugging) <br>
*   2: Setup logging with the TensorFlow MicroErrorReporter, this error reporter print to the debugging Serial Connection incase of an error. <br>
*   3: Map the model into a usable data structure. <br>
*   4: Pull in all the operation implementations needed for the model. <br>
*   5: Build an interpreter to run the model with. <br>
*   6: Allocate memory from the tensor_arena for the model's tensors. <br>
*   7: Obtain pointers to the model's input and output tensors. <br>
*   8: Check if the model input size and dimensions are correct. <br>
*   9: Calculate the input length of the model. <br>
*   10: Setup pinMode for Arduino Nano 33 BLE pins. <br>
*   11: Check if the MicroSD card is connected. <br>
*   12: Setup and start the LSM9DS1 IMU. <br>
*   13: Check if the LSM9DS1 IMU has started. <br>
*   14: Wait 10 seconds to enter the main loop.
*
* @return void
*
**/
void setup();

// Runs one iteration of data gathering and inference. This should be called
// repeatedly from the application code. The name needs to be loop() for Arduino
// compatibility.
/**
* @brief  loop(): Gathers the IMU data, checks if threshold is exceeded, save an extra amount of data, calculates the features and then runs inference. Incase of a predicted fall, the system wakes up the LoPy and waits for reply.
*
*   The main loop, with the functionality of gathering data and performing inferens from it.
*   
*   1: (Get data) Gathers enough new IMU data, so inferens can be run <br>
*   2: (Check for threshold) Gathers new IMU data, as long as the threshold is not exceeded. <br>
*   3: (Save extra amount of signal) When the threshold is exceeded gather an extra amount of IMU data. <br>
*   4: (Predict fall) Calculate the features and run inferens to predict if a fall has occured. If a fall was predicted, go to 5 if not go to 7. <br>
*   5: (LoPy communication) Wake up the LoPy, start the audio alarm, save the signal to the microSD card and then wait for reply from the LoPy. <br>
*   6: (Reset fall variables) If a fall occured, the system sets the wakeup pin low and stops the audio. It then sets the boolean got_data variable to false. <br>
*   7: (Reset variables) Here the system resets the threshold variable, so it is below the defined threshold, and resets the extra signal variable. If a fall was predicted the system starts from 1 and if not the system starts from 2. <br>
*
* @return void
*
**/
void loop();

#endif  // MAIN_FUNCTIONS_H_
