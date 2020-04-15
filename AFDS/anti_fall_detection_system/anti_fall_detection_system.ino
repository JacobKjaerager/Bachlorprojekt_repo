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

#include <TensorFlowLite.h>

// TensorFlowLite necessary includes
#include "fall_model_f46.h" // This is the model generated for the fall detection system
#include "main_functions.h"
#include "constants.h"
#include "output_handler.h"
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Includes for the functionality of the arduino
#include <SPI.h>
#include <SD.h>
#include <Math.h>

// Homemade includes
#include "features.h"
#include "sd_read.h"

// Global variables

// for the SD card reader
const int chipSelect = 10;
File file;

// for timing varies operations
unsigned long time_start;
unsigned long time_end;

// Generate feature array
float features[46];

// Generate arrays for all of the signal
float acc_x_arr[299];
float acc_y_arr[299];
float acc_z_arr[299];
float gyro_x_arr[299];
float gyro_y_arr[299];
float gyro_z_arr[299];

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 70 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(fall_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::ops::micro::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);
//  if ((input->dims->size != 2) || (input->dims->data[0] != 1) ||
//      (input->dims->data[1] != 46)) {
//        TF_LITE_REPORT_ERROR(micro_error_reporter, "Bad input");
//        return;
//      }

  // Keep track of how many inferences we have performed.
  inference_count = 0;

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  readSignalValues(chipSelect, "TESTTTT.CSV", acc_x_arr, acc_y_arr, acc_z_arr,
                   gyro_x_arr, gyro_y_arr, gyro_z_arr);

  Serial.println("Starting calculations:");
  time_start = micros();
  feature_calculation(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      (sizeof(acc_x_arr)/sizeof(float)));
  time_end = micros();
  Serial.print("Calculation time: ");
  Serial.print(time_end-time_start);
  Serial.println(" micros");

  Serial.println("Inferens starting!");
}

// The name of this function is important for Arduino compatibility.
void loop() {
  // Calculate an x value to feed into the model. We compare the current
  // inference_count to the number of inferences per cycle to determine
  // our position within the range of possible x values the model was
  // trained on, and use this to calculate a value.
  float position = static_cast<float>(inference_count) /
                   static_cast<float>(kInferencesPerCycle);
  float x_val = position * kXrange;

  // Generate pointer to our features array
  const float* fall_features_pointer = features;

  readSignalValues(chipSelect, "TESTTTT.CSV", acc_x_arr, acc_y_arr, acc_z_arr,
                   gyro_x_arr, gyro_y_arr, gyro_z_arr);

  Serial.println("Starting calculations:");
  time_start = micros();
  feature_calculation(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      (sizeof(acc_x_arr)/sizeof(float)));
  time_end = micros();
  Serial.print("Calculation time: ");
  Serial.print(time_end-time_start);
  Serial.println(" micros");
 
  // Place our calculated feature values in the model's input tensors
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = fall_features_pointer[i];
  }

  Serial.println("Starting inferens:");
  time_start = micros();
  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on x_val: %f\n",
                           static_cast<double>(x_val));
    return;
  }
  time_end = micros();
  Serial.print("Inferens time: ");
  Serial.print(time_end-time_start);
  Serial.println(" micros");

  // Read the predicted y value from the model's output tensor
  float y_val = output->data.f[0];
  float y_val2 = output->data.f[1];

  Serial.println("First signal");
  Serial.print("Chance of not fall: ");
  Serial.println(y_val);
  Serial.print("Chance of fall: ");
  Serial.println(y_val2);

  // Increment the inference_counter, and reset it if we have reached
  // the total number per cycle
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) inference_count = 0;

  delay(500);

  readSignalValues(chipSelect, "TESTTT.CSV", acc_x_arr, acc_y_arr, acc_z_arr,
                   gyro_x_arr, gyro_y_arr, gyro_z_arr);

  feature_calculation(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      (sizeof(acc_x_arr)/sizeof(float)));

  // Place our calculated feature values in the model's input tensors
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = fall_features_pointer[i];
  }

  interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on x_val: %f\n",
                           static_cast<double>(x_val));
    return;
  }

  // Read the predicted y value from the model's output tensor
  y_val = output->data.f[0];
  y_val2 = output->data.f[1];

  Serial.println("Second signal");
  Serial.print("Chance of not fall: ");
  Serial.println(y_val);
  Serial.print("Chance of fall: ");
  Serial.println(y_val2);

  // Increment the inference_counter, and reset it if we have reached
  // the total number per cycle
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) inference_count = 0;

  delay(500);

  // Output the results. A custom HandleOutput function can be implemented
  // for each supported hardware target.
  //HandleOutput(error_reporter, x_val, y_val);
}
