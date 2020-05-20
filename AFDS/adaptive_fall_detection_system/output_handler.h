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
  * @file:    output_handler.h
  * @date:    26-03-2020 09:32:41
  * @author:  TensorFlow Authors + Morten Sahlertz
  *
  * @brief    Handler for handling the prediction of whether or not a fall has occured and the outputs to the User Interface and the LoPy
  *
  *
  *
  *
**/

#ifndef OUTPUT_HANDLER_H_
#define OUTPUT_HANDLER_H_

#include "constants.h"
#include "tensorflow/lite/c/c_api_internal.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"

// Called by the main loop to handle the prediction from the TensorFlow Lite model and the outputs to UI and the LoPy

/**
* @brief  start_audio(): // Starts the Audio PWM signal to generate the alarm
*
* @param none
* @return void
*
**/
void start_audio();

/**
* @brief  stop_audio(): // Stops the Audio PWM signal
*
* @param none
* @return void
*
**/
void stop_audio();

/**
* @brief  wake_up_lopy(): // Set the wake up pin for the LoPy to high
*
* @param none
* @return void
*
**/
void wake_up_lopy();

/**
* @brief  stop_wake_up_lopy(): // Set the wake up pin for the LoPy to low
*
* @param none
* @return void
*
**/
void stop_wake_up_lopy();

/**
* @brief  HandleOutput(): Handler for the prediction of whether not a fall has occured or not
*
*   Handles the prediction of whether or not a fall has been predicted. If a fall has been
*   predicted, the wake up pin is set to a high output (3.3V) to wake up the LoPy and the
*   alarm audio is turned on. If a fall has not been predicted the functions returns immediately.
*
* @param tflite::ErrorReporter* error_reporter: This is pointer to the generated tflite::ErrorReporter, which writes UART messages incase of a reported error
* @param int prediction: The prediction value from the predict_fall function.
* @return void
*
**/
void HandleOutput(tflite::ErrorReporter* error_reporter, int prediction);

#endif  // OUTPUT_HANDLER_H_
