/**@file adaptive_fall_detection_system.ino */

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


    @file:    adaptive_fall_detection_system.ino
    @date:    26-03-2020 09:32:41
    @author:  TensorFlow Authors + Morten Sahlertz

    @brief    Main functions setup() and loop() for the system




**/

#include <TensorFlowLite.h>

// TensorFlowLite necessary includes
#include "fall_model_f46.h" // This is the first model generated for the fall detection system
//#include "fall_model_f46_2.h" // This is the second model generated for the fall detection system
//#include "fall_model_f46_3.h" // This is the third model generated for the fall detection system
#include "main_functions.h"
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Includes for the functionality of the arduino
#include <SPI.h>
#include <SD.h>
#include <Math.h>

// Includes for datalogger, IMU and other help functions
#include "constants.h"
#include "feature_calculation.h"
#include "datalogger.h"
#include "SetupIMU.h"

//  : public LSM9DS1Class

class afdsdCtrl1 : public datalogger {
  public:
    // Generate arrays for accelerometer and gyroscope values of the signal
    float acc_x_arr[SIGNAL_SIZE];       /**<  Array used to store the x-axis accelerometer data   */
    float acc_y_arr[SIGNAL_SIZE];       /**<  Array used to store the y-axis accelerometer data   */
    float acc_z_arr[SIGNAL_SIZE];       /**<  Array used to store the z-axis accelerometer data   */
    float gyro_x_arr[SIGNAL_SIZE];      /**<  Array used to store the x-axis gyroscope data   */
    float gyro_y_arr[SIGNAL_SIZE];      /**<  Array used to store the y-axis gyroscope data   */
    float gyro_z_arr[SIGNAL_SIZE];      /**<  Array used to store the z-axis gyroscope data   */

    // Generate feature array
    float features[FEATURE_AMOUNT];     /**<  Array used to store the calculated features from the data signals   */

    // Generate char_array for naming saved fall data
    char fall_name_array[NAME_SIZE];    /**<  Array used to store the generated fall name   */

    // Variable setup
    int idx = 0;                        /**<  Int to be incremented to ensure enough data is stored, so inferens can be performed   */
    int extra_signal = 0;               /**<  Int to be incremented to store more data after the threshold has been exceded   */
    int fall_nr = 0;                    /**<  Int to be incremented when a fall has been predicted   */
    char incoming_byte[2];              /**<  Array used to store the incoming UART bytes from the LoPy */
    float threshold = 1;                /**<  Float for the calculated threshold   */
    bool got_data = false;              /**<  Bool for checking whether or not, that enough data have been collected in the data signals arrays   */
    bool initialized = false;           /**<  Bool for checking whether or not, the function has run at least once   */

    /**
    * @brief  push_array(): Push the new value into the array
    *
    *   Pushes the new value into the chosen array, and pops the last array value
    *   out.
    *
    * @param float new_value: The new value to push into the array.
    * @param float chosen_array[]: Pointer to the chosen array for pushing the new value.
    * @param int array_length: Int for the length of the chosen array
    * @return void:
    *
    **/
    void push_array(float new_value, float chosen_array[], int array_length) {
      for (int i = array_length-1; i >= 0; i--) {
        if (i == 0) {
          chosen_array[i] = new_value;
        }
        else {
          chosen_array[i] = chosen_array[i-1];
        }
      }
    };

    /**
    * @brief  readIMU(): Pushes the read IMU data into the array
    *
    *   Pushes the read IMU values into the chosen arrays, and pops the last array values
    *   out.
    *
    * @param float ax: The new accelerometer X-axis value to push into the array.
    * @param float ay: The new accelerometer Y-axis value to push into the array.
    * @param float az: The new accelerometer Z-axis value to push into the array.
    * @param float gx: The new gyroscope X-axis value to push into the array.
    * @param float gy: The new gyroscope Y-axis value to push into the array.
    * @param float gz: The new gyroscope Z-axis value to push into the array.
    * @return void:
    *
    **/
    void readIMU(float ax, float ay, float az, float gx, float gy, float gz) {
      push_array(-ax, acc_x_arr, SIGNAL_SIZE); // The negative of this value is needed, because the IMU X-axis is reversed compared to the model
      push_array(ay, acc_y_arr, SIGNAL_SIZE);
      push_array(az, acc_z_arr, SIGNAL_SIZE);
      push_array(-gx, gyro_x_arr, SIGNAL_SIZE); // The negative of this value is needed, because the IMU X-axis is reversed compared to the model
      push_array(gy, gyro_y_arr, SIGNAL_SIZE);
      push_array(gz, gyro_z_arr, SIGNAL_SIZE);
    };

    /**
    * @brief  inc_idx(): // Increment the idx variable and set got_data to true, when enough data is stored in the array
    *
    * @param none
    * @return void
    *
    **/
    void inc_idx() {
      idx++;
      if (idx >= (SIGNAL_SIZE / 2)) {
        idx = 0;
        got_data = true;
      }
    };

    /**
    * @brief  calculate_threshold(): // Calculate new threshold value from the newest accelerometer samples
    *
    * @param none
    * @return void
    *
    **/
    void calculate_threshold() {
      threshold = sqrt(sq(acc_x_arr[0]) + sq(acc_y_arr[0]) + sq(acc_z_arr[0]));
    };

    /**
    * @brief  inc_extra(): // Increment the extra variable
    *
    * @param none
    * @return void
    *
    **/
    void inc_extra() {
      extra_signal++;
    };

    
    /**
    * @brief  feature_calculation(): Calculate the features from the signal data from the IMU
    *
    *   Calculates the features from the signals data arrays, accelerometer x-axis, y-axis and z-axis
    *   data arrays and gyroscopes x-axis, y-axus and z-axis data arrays. The calculated features will
    *   be put in the features array.
    *
    * @param none
    * @return void:
    *
    **/
    void feature_calculation() {
      feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                   gyro_x_arr, gyro_y_arr, gyro_z_arr, SIGNAL_SIZE);
    };

    /**
    * @brief  start_audio(): // Starts the Audio PWM signal to generate the alarm
    *
    * @param none
    * @return void
    *
    **/
    void start_audio() {
      // Start Audio PWM signal
      analogWrite(AUDIO, 50);
    };

    /**
    * @brief  stop_audio(): // Stops the Audio PWM signal
    *
    * @param none
    * @return void
    *
    **/
    void stop_audio() {
      // Stop Audio PWM signal
      analogWrite(AUDIO, 0);
    };

    /**
    * @brief  wake_up_lopy(): // Set the wake up pin for the LoPy to high
    *
    * @param none
    * @return void
    *
    **/
    void wake_up_lopy() {
      // Set Wake up pin high
      digitalWrite(WAKE_UP, HIGH);
    };

    /**
    * @brief  stop_wake_up_lopy(): // Set the wake up pin for the LoPy to low
    *
    * @param none
    * @return void
    *
    **/
    void stop_wake_up_lopy() {
      // Set Wake up pin low
      digitalWrite(WAKE_UP, LOW);
    };

    /**
    * @brief  predictor(): Predict whether or not a fall has occured
    *
    *   Uses a pointer to the output tensors of the model to predict whether 
    *   or not a fall has occured.
    *
    * @param float* output: Pointer to the output tensor of the TensorFlow Lite model
    * @return int: 1 = fall detected, 0 = no fall detected.
    *
    **/
    int predictor(float* output) {
      int this_predict = -1;
      // If output[1] is bigger than output[0] this means chance of fall is bigger
      // than the chance of a not fall. The equal is there, because if the model,
      // is fifty / fifty whether it's a fall or not a fall, we choose it to be a
      // fall for safety
      if (output[1] >= output[0]) {
        // For debugging print the chance of fall
        Serial.print("F: ");
        Serial.println(output[1]);
        // Set the prediction to one, which means a fall
        this_predict = 1;
      }
      // If output[0] is bigger than output[1] this means chance of fall is smaller
      // than the chance of not fall.
      else {
        // For debugging print the chance of fall
        Serial.print("F: ");
        Serial.println(output[1]);
        // Set the prediction to zero, which means not a fall
        this_predict = 0;
      }
      // Return the prediction
      return this_predict;
    };

    /**
    * @brief  generate_fall_name(): Generate a fall name from a fall number
    *
    *   Generate a new fall name with "FALL" + fall_number + ".csv", so a new fall
    *   data file can be generated on the MicroSD card.
    *
    * @param char name_array[]: Pointer to the array where the new name is stored
    * @param int fall_number: Int for fall number wanted in the name_array[]
    * @return void:
    *
    **/
    void generate_fall_name(char name_array[], int fall_number) {
      // Generate int to char buffer
      char int_to_char_str[2];
      // Read them as int
      sprintf(int_to_char_str, "%d", fall_number);
      // Makes sure the first 9 number read 0#, where # is the int number
      if(fall_number <= 9) {
        int_to_char_str[1] = int_to_char_str[0];
        int_to_char_str[0] = '0';
      }
      // Generate new array and copy it to the existing array
      char new_array[] = {'F', 'A', 'L', 'L', int_to_char_str[0], int_to_char_str[1], '.', 'c', 's', 'v',  '\0'};
      strcpy(name_array, new_array);
    };

    /**
    * @brief  lopy_communication(): Communication with LoPy
    *
    *   Wake up the LoPy, start the audio alarm, save the signal to the microSD card and then wait for reply from the LoPy. 
    *
    * @param none
    * @return void
    *
    **/
    void lopy_communication() {
      // Set LoPy wake up pin to high, to wake the LoPy
      wake_up_lopy();
      // Turn on green LED and Audio to indicate a fall has happened
      digitalWrite(LEDG, LOW);
      start_audio();
      // Increment number of falls
      fall_nr++;

      // If fall numbers are equal to 100 restart the count at 1
      if (fall_nr >= 100) {
        fall_nr = 1;
      }

      // Function to generate a new name for the file 'FALL' + fall_nr + '.csv'
      generate_fall_name(fall_name_array, fall_nr);

      Serial.println(fall_name_array);
      // Save the data to the SD card
      datalogger::writeSignalValues(CS, fall_name_array, acc_x_arr, acc_y_arr,
                                    acc_z_arr, gyro_x_arr, gyro_y_arr,
                                    gyro_z_arr, SIGNAL_SIZE, true);

      // Stay in this loop untill the LoPy sends the "OK" message indicating it has send the fall alarm
      while (incoming_byte[0] != 'O' && incoming_byte[1] != 'K') {
        if (Serial1.available() > 0) {
          // Read the incoming byte:
          Serial1.readBytes(incoming_byte, 2);
        }
      }
    };

    /**
    * @brief  reset_fall_variables(): // After communication with the LoPy the system sets the wakeup pin low and stops the audio. It then sets the boolean got_data variable to false.
    *
    * @param none
    * @return void
    *
    **/
    void reset_fall_variables() {
      // Stop the audio warning and set the LoPy wake up pin to low
      stop_wake_up_lopy();
      stop_audio();
      digitalWrite(LEDG, HIGH);

      // Reset variables
      incoming_byte[0] = '0';
      incoming_byte[1] = '0';
      got_data = false;
    };

    /**
    * @brief  reset_variables(): // Here the system resets the threshold variable, so it is below the defined threshold, and resets the extra signal variable.
    *
    * @param none
    * @return void
    *
    **/
    void reset_variables() {
      // Reset variables
      extra_signal = 0;
      threshold = 1;
    };

    /**
    * @brief  predict_fall(): // Prediction of whether or not a fall has occured and the handling of the scenarios
    *
    * @param @param float* TFOutput: Pointer to the output tensor of the TensorFlow Lite model
    * @return void
    *
    **/
    void predict_fall(float* TFOutput) {
      // Get prediction value
      int prediction = predictor(TFOutput);

      // In case of a fall
      if (prediction == 1) {
        // Communicate with the LoPy
        lopy_communication();
        // Reset fall variables
        reset_fall_variables();
      }
      // Reset variables
      reset_variables();
      // This is only for testing and debugging
      Serial.println(prediction);
    };

};

// Generate the Ctrl Class object
afdsdCtrl1 Ctrl;

// For timing varies operations
unsigned long time_start;
unsigned long time_end;

// Int for the input size of the model
int input_length;                   /**<  Int for the calculated input size of the TensorFlow Lite model   */

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 70 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  // Start the two serial connections
  Serial1.begin(9600); // Communication with the LoPy 4
  Serial.begin(9600); // This serial communication is only needed for debugging

  // Make sure the system doesn't start until UART is connected, only needed for debugging
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(fall_model_data); // Changed to model nr: 2
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
      "Model provided is schema version %d not equal "
      "to supported version %d.",
      model->version(), TFLITE_SCHEMA_VERSION);
    // Do not do anything more:
    while (1);
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
    // Do not do anything more:
    while (1);
  }

  // Obtain pointers to the model's input and output tensors.
  // Check if the model input size and dimensions are correct
  input = interpreter->input(0);
  output = interpreter->output(0);
  if ((input->dims->size != 2) || (input->dims->data[0] != 1) ||
      (input->dims->data[1] != 46)) {
    error_reporter->Report("Bad input tensor parameters in model");
    // Do not do anything more:
    while (1);
  }

  // Calculate the input length of the model
  input_length = input->bytes / sizeof(float);

  // Set the Chip Select for the MicroSD card adapter low (There is only one slave for the SPI configuration,
  // therefore it is always low.
  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Set the Wake-up pin and audio pin to output
  pinMode(WAKE_UP, OUTPUT);
  pinMode(AUDIO, OUTPUT);

  // Initiate RBG LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  // For some reason these are turned of when HIGH
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  // Makes sure the MicroSD card is connected on the CS
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

  while (Ctrl.got_data == false) {
    if (IMU.accelerationAvailable() == true && IMU.gyroscopeAvailable() == true) {
      // Read the accelerometer and gyroscope output registers
      IMU.readAccel();
      IMU.readGyro();
      // Push the read values to the arrays
      Ctrl.readIMU(IMU.calcAccel(IMU.ax), IMU.calcAccel(IMU.ay), IMU.calcAccel(IMU.az),
                   IMU.calcGyro(IMU.gx), IMU.calcGyro(IMU.gy), IMU.calcGyro(IMU.gz));
      Ctrl.inc_idx();
    }
  }

  while (Ctrl.threshold < THRESHOLD) { // Global variable
    if (IMU.accelerationAvailable() == true && IMU.gyroscopeAvailable() == true) {
      // Read the accelerometer and gyroscope output registers
      IMU.readAccel();
      IMU.readGyro();
      // Push the read values to the arrays
      Ctrl.readIMU(IMU.calcAccel(IMU.ax), IMU.calcAccel(IMU.ay), IMU.calcAccel(IMU.az),
                   IMU.calcGyro(IMU.gx), IMU.calcGyro(IMU.gy), IMU.calcGyro(IMU.gz));
      // Calculate new threshold value
      Ctrl.calculate_threshold();
    }
  }

  Serial.println("R:"); // For debugging
  // After threshold is exceded, continue to record values for half of the signal size
  while (Ctrl.extra_signal < (SIGNAL_SIZE / 2)) {
    if (IMU.accelerationAvailable() == true && IMU.gyroscopeAvailable() == true) {
      // Read the accelerometer and gyroscope output registers
      IMU.readAccel();
      IMU.readGyro();
      // Push the read values to the arrays
      Ctrl.readIMU(IMU.calcAccel(IMU.ax), IMU.calcAccel(IMU.ay), IMU.calcAccel(IMU.az),
                   IMU.calcGyro(IMU.gx), IMU.calcGyro(IMU.gy), IMU.calcGyro(IMU.gz));
      // Calculate new threshold value
      Ctrl.inc_extra();
    }
  }
  
  if (Ctrl.got_data == true) {

    // Calculate features from signals
    Ctrl.feature_calculation();

    // Generate pointer to our features array
    const float* fall_features_pointer = Ctrl.features;

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

    // Predict fall and handle the prediction
    Ctrl.predict_fall(interpreter->output(0)->data.f);
  }
}
