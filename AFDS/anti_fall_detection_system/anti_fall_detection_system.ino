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
#include "fall_predictor.h"
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Includes for the functionality of the arduino
#include <SPI.h>
#include <SD.h>
#include <Math.h>

// Includes for SD, IMU and help functions
#include "help_functions.h"
#include "sd_driver.h"
#include "SetupIMU.h"

// For timing varies operations
unsigned long time_start;
unsigned long time_end;

// Variable setup
int idx = 0;
int extra_signal = 0;
int fall_nr = 0;
char incoming_byte[2];
float threshold = 1;
bool got_data = false;

// Generate feature array
float features[FEATURE_AMOUNT];

// Generate char_array for naming saved fall data
char fall_name_array[NAME_SIZE];

// Generate arrays for accelerometer and gyroscope values of the signal
float acc_x_arr[SIGNAL_SIZE];
float acc_y_arr[SIGNAL_SIZE];
float acc_z_arr[SIGNAL_SIZE];
float gyro_x_arr[SIGNAL_SIZE];
float gyro_y_arr[SIGNAL_SIZE];
float gyro_z_arr[SIGNAL_SIZE];

// Int for the input size of the model
int input_length;

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
  // Check if the model input size and dimensions are correct
  input = interpreter->input(0);
  output = interpreter->output(0);
  if ((input->dims->size != 2) || (input->dims->data[0] != 1) ||
      (input->dims->data[1] != 46)) {
        error_reporter->Report("Bad input tensor parameters in model");
        return;
  }

  // Calculate the input length of the model
  input_length = input->bytes / sizeof(float);

  // Keep track of how many inferences we have performed.
  inference_count = 0;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Initiate RBG LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  // For some reason these are turned of when HIGH
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  Serial1.begin(9600);
  Serial.begin(9600); // This serial communication is only needed for debugging

  // Make sure the system doesn't start until UART is connected, only needed for debugging
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
    digitalWrite(LEDR, LOW);
    // Do not do anything more:
    while (1);
  }

  // Setup IMU with default settings and starts it
  SetupIMUDefaults();
  StartIMU();

  if (!IMU.begin()) {
    error_reporter->Report("Failed to initialize IMU");
    digitalWrite(LEDR, LOW);
    // Do not do anything more: 
    while (1);
  }

  // Wait ten seconds until the system starts
  delay(10000);
  Serial.println("Inferens starting!");
}

// The name of this function is important for Arduino compatibility.
void loop() {

  while (got_data == false) { // Global variable
    if (IMU.accelerationAvailable() == true && IMU.gyroscopeAvailable() == true) {
      // Read the accelerometer and gyroscope output registers
      IMU.readAccel();
      IMU.readGyro();
      // Push the read values to the arrays
      push_array(IMU.calcAccel(IMU.ax), acc_x_arr, SIGNAL_SIZE);
      push_array(IMU.calcAccel(IMU.ay), acc_y_arr, SIGNAL_SIZE);
      push_array(IMU.calcAccel(IMU.az), acc_z_arr, SIGNAL_SIZE);
      push_array(IMU.calcGyro(IMU.ax), gyro_x_arr, SIGNAL_SIZE);
      push_array(IMU.calcGyro(IMU.ay) + 90, gyro_y_arr, SIGNAL_SIZE); // Added +90 for calibration
      push_array(IMU.calcGyro(IMU.az), gyro_z_arr, SIGNAL_SIZE);
      idx++;
      if (idx >= (SIGNAL_SIZE / 2)) {
        idx = 0;
        got_data = true;
      }
    }
  }
  // Main part of the loop, pushing accelerometer and gyroscopes values to the arrays, and recalculating
  // the threshold value, to check if the person is in a free fall
  while (threshold < THRESHOLD) { // Global variable
    if (IMU.accelerationAvailable() == true && IMU.gyroscopeAvailable() == true) {
      // Read the accelerometer and gyroscope output registers
      IMU.readAccel();
      IMU.readGyro();
      // Push the read values to the arrays
      push_array(IMU.calcAccel(IMU.ax), acc_x_arr, SIGNAL_SIZE);
      push_array(IMU.calcAccel(IMU.ay), acc_y_arr, SIGNAL_SIZE);
      push_array(IMU.calcAccel(IMU.az), acc_z_arr, SIGNAL_SIZE);
      push_array(IMU.calcGyro(IMU.ax), gyro_x_arr, SIGNAL_SIZE);
      push_array(IMU.calcGyro(IMU.ay) + 90, gyro_y_arr, SIGNAL_SIZE); // Added +90 for calibration
      push_array(IMU.calcGyro(IMU.az), gyro_z_arr, SIGNAL_SIZE);
      // Calculate new threshold value
      threshold = sqrt(sq(acc_x_arr[0]) + sq(acc_y_arr[0]) + sq(acc_z_arr[0]));
    }
  }

  Serial.println("R:"); // For debugging
  // After threshold is exceded, continue to record values for half of the signal size
  while (extra_signal < (SIGNAL_SIZE / 2)) {
    if (IMU.accelerationAvailable() == true && IMU.gyroscopeAvailable() == true) {
      // Read the accelerometer and gyroscope output registers
      IMU.readAccel();
      IMU.readGyro();
      // Push the read values to the arrays
      push_array(IMU.calcAccel(IMU.ax), acc_x_arr, SIGNAL_SIZE);
      push_array(IMU.calcAccel(IMU.ay), acc_y_arr, SIGNAL_SIZE);
      push_array(IMU.calcAccel(IMU.az), acc_z_arr, SIGNAL_SIZE);
      push_array(IMU.calcGyro(IMU.ax), gyro_x_arr, SIGNAL_SIZE);
      push_array(IMU.calcGyro(IMU.ay) + 90, gyro_y_arr, SIGNAL_SIZE); // Added +90 for calibration
      push_array(IMU.calcGyro(IMU.az), gyro_z_arr, SIGNAL_SIZE);
      // Calculate new threshold value
      extra_signal++;
    }
  }

  if (got_data == true) {
    
    // Calculate the features of the saved signal, these are going to be the
    // input to our tensorflow lite model
    feature_calculation(features, acc_x_arr, acc_y_arr, acc_z_arr,
                        gyro_x_arr, gyro_y_arr, gyro_z_arr,
                        SIGNAL_SIZE);

    // Generate pointer to our features array
    const float* fall_features_pointer = features;

    // Place our calculated features values in the model's input tensors
    for (int i = 0; i < input_length; ++i) {
      input->data.f[i] = fall_features_pointer[i];
    }

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      error_reporter->Report("Invoke failed");
      return;
    }

    // Get prediction value
    int prediction = predict_fall(interpreter->output(0)->data.f);

    // Handle the output
    HandleOutput(error_reporter, prediction);

    // In case of a fall
    if(prediction == 1) {
      // Increment number of falls
      fall_nr++;
      
      // Function to generate a new name for the file 'FALL' + fall_nr + '.csv'
      generate_fall_name(fall_name_array, fall_nr);

      Serial.println(fall_name_array);
      // Save the data to the SD card
      writeSignalValues(CS, fall_name_array, acc_x_arr, acc_y_arr,
                        acc_z_arr, gyro_x_arr, gyro_y_arr,
                        gyro_z_arr, SIGNAL_SIZE, true);

      // Stay in this loop untill the LoPy sends the "OK" message indicating it has send the fall alarm
      while(incoming_byte[0] != 'O' && incoming_byte[1] != 'K') {
        if (Serial1.available() > 0) {
        // Read the incoming byte:
        Serial1.readBytes(incoming_byte, 2);
        }
      }
      
      // Stop the audio warning and set the LoPy wake up pin to low
      digitalWrite(WAKE_UP, LOW);
      digitalWrite(LEDG, HIGH);
      analogWrite(AUDIO, 0);

      // Reset variables
      incoming_byte[0] = '0';
      incoming_byte[1] = '0';
      got_data = false;
     
    }
    // This is only for testing and debugging
    Serial.println(prediction);

  }

  // Reset variables
  extra_signal = 0;
  threshold = 1;
}
