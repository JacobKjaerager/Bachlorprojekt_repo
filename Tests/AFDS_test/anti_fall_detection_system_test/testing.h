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

#include "fall_features_data.h"
#include "daily_features_data.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "datalogger.h"
#include "feature_calculation.h"

// Feature amount needed for input to fall algorithm
const int FEATURE_AMOUNT = 46;
const int CS = 10;

datalogger sd;

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(FeatureCalculationFallSignalNr1) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = falling_acc_x_signal_nr1;
  float* acc_y_ptr = falling_acc_y_signal_nr1;
  float* acc_z_ptr = falling_acc_z_signal_nr1;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = falling_gyro_x_signal_nr1;
  float* gyro_y_ptr = falling_gyro_y_signal_nr1;
  float* gyro_z_ptr = falling_gyro_z_signal_nr1;

  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_ptr, acc_y_ptr, acc_z_ptr,
                      gyro_x_ptr, gyro_y_ptr, gyro_z_ptr,
                      300);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* fall_features_pointer = falling_features_data_nr1;

  // Check if the calculated feature data is within 0.1‰ of the features calculated for training algorithm
  // Reason for not checking if equal, is because of small variations in float calculation operations
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], fall_features_pointer[i], abs(0.0001*features[i]));
  }
}

TF_LITE_MICRO_TEST(FeatureCalculationFallSignalNr2) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = falling_acc_x_signal_nr2;
  float* acc_y_ptr = falling_acc_y_signal_nr2;
  float* acc_z_ptr = falling_acc_z_signal_nr2;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = falling_gyro_x_signal_nr2;
  float* gyro_y_ptr = falling_gyro_y_signal_nr2;
  float* gyro_z_ptr = falling_gyro_z_signal_nr2;

  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_ptr, acc_y_ptr, acc_z_ptr,
               gyro_x_ptr, gyro_y_ptr, gyro_z_ptr, 299);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* fall_features_pointer = falling_features_data_nr2;

  // Check if the calculated feature data is within 0.1‰ of the features calculated for training algorithm
  // Reason for not checking if equal, is because of small variations in float calculation operations
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], fall_features_pointer[i], abs(0.0001*features[i]));
  }
}

TF_LITE_MICRO_TEST(FeatureCalculationFallSignalNr3) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = falling_acc_x_signal_nr3;
  float* acc_y_ptr = falling_acc_y_signal_nr3;
  float* acc_z_ptr = falling_acc_z_signal_nr3;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = falling_gyro_x_signal_nr3;
  float* gyro_y_ptr = falling_gyro_y_signal_nr3;
  float* gyro_z_ptr = falling_gyro_z_signal_nr3;

  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_ptr, acc_y_ptr, acc_z_ptr,
                      gyro_x_ptr, gyro_y_ptr, gyro_z_ptr,
                      300);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* fall_features_pointer = falling_features_data_nr3;

  // Check if the calculated feature data is within 0.1‰ of the features calculated for training algorithm
  // Reason for not checking if equal, is because of small variations in float calculation operations
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], fall_features_pointer[i], abs(0.0001*features[i]));
  }
}

TF_LITE_MICRO_TEST(FeatureCalculationDailySignalNr1) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = daily_acc_x_signal_nr1;
  float* acc_y_ptr = daily_acc_y_signal_nr1;
  float* acc_z_ptr = daily_acc_z_signal_nr1;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = daily_gyro_x_signal_nr1;
  float* gyro_y_ptr = daily_gyro_y_signal_nr1;
  float* gyro_z_ptr = daily_gyro_z_signal_nr1;

  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_ptr, acc_y_ptr, acc_z_ptr,
                      gyro_x_ptr, gyro_y_ptr, gyro_z_ptr,
                      299);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* daily_features_pointer = daily_features_data_nr1;

  // Check if the calculated feature data is within 0.1‰ of the features calculated for training algorithm
  // Reason for not checking if equal, is because of small variations in float calculation operations
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], daily_features_pointer[i], abs(0.0001*features[i]));
  }
}

TF_LITE_MICRO_TEST(FeatureCalculationDailySignalNr2) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = daily_acc_x_signal_nr2;
  float* acc_y_ptr = daily_acc_y_signal_nr2;
  float* acc_z_ptr = daily_acc_z_signal_nr2;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = daily_gyro_x_signal_nr2;
  float* gyro_y_ptr = daily_gyro_y_signal_nr2;
  float* gyro_z_ptr = daily_gyro_z_signal_nr2;

  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_ptr, acc_y_ptr, acc_z_ptr,
                      gyro_x_ptr, gyro_y_ptr, gyro_z_ptr,
                      300);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* daily_features_pointer = daily_features_data_nr2;

  // Check if the calculated feature data is within 0.1‰ of the features calculated for training algorithm
  // Reason for not checking if equal, is because of small variations in float calculation operations
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], daily_features_pointer[i], abs(0.0001*features[i]));
  }
}

TF_LITE_MICRO_TEST(FeatureCalculationDailySignalNr3) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = daily_acc_x_signal_nr3;
  float* acc_y_ptr = daily_acc_y_signal_nr3;
  float* acc_z_ptr = daily_acc_z_signal_nr3;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = daily_gyro_x_signal_nr3;
  float* gyro_y_ptr = daily_gyro_y_signal_nr3;
  float* gyro_z_ptr = daily_gyro_z_signal_nr3;

  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_ptr, acc_y_ptr, acc_z_ptr,
                      gyro_x_ptr, gyro_y_ptr, gyro_z_ptr,
                      500);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* daily_features_pointer = daily_features_data_nr3;

  // Check if the calculated feature data is within 0.1‰ of the features calculated for training algorithm
  // Reason for not checking if equal, is because of small variations in float calculation operations
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], daily_features_pointer[i], abs(0.0001*features[i]));
  }
}

TF_LITE_MICRO_TEST(SDFeatureCalculationFallSignalNr1) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  int signal_length = 299;
  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = falling_acc_x_signal_nr1;
  float* acc_y_ptr = falling_acc_y_signal_nr1;
  float* acc_z_ptr = falling_acc_z_signal_nr1;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = falling_gyro_x_signal_nr1;
  float* gyro_y_ptr = falling_gyro_y_signal_nr1;
  float* gyro_z_ptr = falling_gyro_z_signal_nr1;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
  }

  sd.writeSignalValues(CS, "FT1.csv", acc_x_ptr, acc_y_ptr,
                        acc_z_ptr, gyro_x_ptr, gyro_y_ptr,
                        gyro_z_ptr, signal_length, true);

  float acc_x_arr[signal_length];
  float acc_y_arr[signal_length];
  float acc_z_arr[signal_length];
  float gyro_x_arr[signal_length];
  float gyro_y_arr[signal_length];
  float gyro_z_arr[signal_length];

  sd.readSignalValues(CS, "FT1.csv", acc_x_arr, acc_y_arr,
                   acc_z_arr, gyro_x_arr, gyro_y_arr, gyro_z_arr);
  
  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      signal_length);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* falling_features_pointer = falling_features_data_nr1;

  // Check if the calculated feature data is within 5% of the features calculated for training algorithm
  // Reason for not checking if equal, is because of the SD float are saved with only 2 decimals.
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], falling_features_pointer[i], abs(0.05*features[i]));
  }
}

TF_LITE_MICRO_TEST(SDFeatureCalculationFallSignalNr2) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  int signal_length = 299;
  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = falling_acc_x_signal_nr2;
  float* acc_y_ptr = falling_acc_y_signal_nr2;
  float* acc_z_ptr = falling_acc_z_signal_nr2;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = falling_gyro_x_signal_nr2;
  float* gyro_y_ptr = falling_gyro_y_signal_nr2;
  float* gyro_z_ptr = falling_gyro_z_signal_nr2;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
  }

  sd.writeSignalValues(CS, "FT2.csv", acc_x_ptr, acc_y_ptr,
                        acc_z_ptr, gyro_x_ptr, gyro_y_ptr,
                        gyro_z_ptr, signal_length, true);

  float acc_x_arr[signal_length];
  float acc_y_arr[signal_length];
  float acc_z_arr[signal_length];
  float gyro_x_arr[signal_length];
  float gyro_y_arr[signal_length];
  float gyro_z_arr[signal_length];

  sd.readSignalValues(CS, "FT2.csv", acc_x_arr, acc_y_arr,
                   acc_z_arr, gyro_x_arr, gyro_y_arr, gyro_z_arr);
  
  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      signal_length);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* falling_features_pointer = falling_features_data_nr2;

  // Check if the calculated feature data is within 5% of the features calculated for training algorithm
  // Reason for not checking if equal, is because of the SD float are saved with only 2 decimals.
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], falling_features_pointer[i], abs(0.05*features[i]));
  }
}

TF_LITE_MICRO_TEST(SDFeatureCalculationFallSignalNr3) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  int signal_length = 300;
  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = falling_acc_x_signal_nr3;
  float* acc_y_ptr = falling_acc_y_signal_nr3;
  float* acc_z_ptr = falling_acc_z_signal_nr3;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = falling_gyro_x_signal_nr3;
  float* gyro_y_ptr = falling_gyro_y_signal_nr3;
  float* gyro_z_ptr = falling_gyro_z_signal_nr3;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
  }

  sd.writeSignalValues(CS, "FT3.csv", acc_x_ptr, acc_y_ptr,
                        acc_z_ptr, gyro_x_ptr, gyro_y_ptr,
                        gyro_z_ptr, signal_length, true);

  float acc_x_arr[signal_length];
  float acc_y_arr[signal_length];
  float acc_z_arr[signal_length];
  float gyro_x_arr[signal_length];
  float gyro_y_arr[signal_length];
  float gyro_z_arr[signal_length];

  sd.readSignalValues(CS, "FT3.csv", acc_x_arr, acc_y_arr,
                   acc_z_arr, gyro_x_arr, gyro_y_arr, gyro_z_arr);
  
  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      signal_length);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* falling_features_pointer = falling_features_data_nr3;

  // Check if the calculated feature data is within 5% of the features calculated for training algorithm
  // Reason for not checking if equal, is because of the SD float are saved with only 2 decimals.
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], falling_features_pointer[i], abs(0.05*features[i]));
  }
}

TF_LITE_MICRO_TEST(SDFeatureCalculationDailySignalNr1) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  int signal_length = 299;
  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = daily_acc_x_signal_nr2;
  float* acc_y_ptr = daily_acc_y_signal_nr2;
  float* acc_z_ptr = daily_acc_z_signal_nr2;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = daily_gyro_x_signal_nr2;
  float* gyro_y_ptr = daily_gyro_y_signal_nr2;
  float* gyro_z_ptr = daily_gyro_z_signal_nr2;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
  }

  sd.writeSignalValues(CS, "DT1.csv", acc_x_ptr, acc_y_ptr,
                        acc_z_ptr, gyro_x_ptr, gyro_y_ptr,
                        gyro_z_ptr, signal_length, true);

  float acc_x_arr[signal_length];
  float acc_y_arr[signal_length];
  float acc_z_arr[signal_length];
  float gyro_x_arr[signal_length];
  float gyro_y_arr[signal_length];
  float gyro_z_arr[signal_length];

  sd.readSignalValues(CS, "DT1.csv", acc_x_arr, acc_y_arr,
                   acc_z_arr, gyro_x_arr, gyro_y_arr, gyro_z_arr);
  
  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      signal_length);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* daily_features_pointer = daily_features_data_nr2;

  // Check if the calculated feature data is within 5% of the features calculated for training algorithm
  // Reason for not checking if equal, is because of the SD float are saved with only 2 decimals.
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], daily_features_pointer[i], abs(0.05*features[i]));
  }
}

TF_LITE_MICRO_TEST(SDFeatureCalculationDailySignalNr2) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  int signal_length = 300;
  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = daily_acc_x_signal_nr2;
  float* acc_y_ptr = daily_acc_y_signal_nr2;
  float* acc_z_ptr = daily_acc_z_signal_nr2;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = daily_gyro_x_signal_nr2;
  float* gyro_y_ptr = daily_gyro_y_signal_nr2;
  float* gyro_z_ptr = daily_gyro_z_signal_nr2;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
  }

  sd.writeSignalValues(CS, "DT2.csv", acc_x_ptr, acc_y_ptr,
                        acc_z_ptr, gyro_x_ptr, gyro_y_ptr,
                        gyro_z_ptr, signal_length, true);

  float acc_x_arr[signal_length];
  float acc_y_arr[signal_length];
  float acc_z_arr[signal_length];
  float gyro_x_arr[signal_length];
  float gyro_y_arr[signal_length];
  float gyro_z_arr[signal_length];

  sd.readSignalValues(CS, "DT2.csv", acc_x_arr, acc_y_arr,
                   acc_z_arr, gyro_x_arr, gyro_y_arr, gyro_z_arr);
  
  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      signal_length);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* daily_features_pointer = daily_features_data_nr2;

  // Check if the calculated feature data is within 5% of the features calculated for training algorithm
  // Reason for not checking if equal, is because of the SD float are saved with only 2 decimals.
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], daily_features_pointer[i], abs(0.05*features[i]));
  }
}

TF_LITE_MICRO_TEST(SDFeatureCalculationDailySignalNr3) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  int signal_length = 500;
  // Generate pointers to accelerometer fall signal data
  float* acc_x_ptr = daily_acc_x_signal_nr3;
  float* acc_y_ptr = daily_acc_y_signal_nr3;
  float* acc_z_ptr = daily_acc_z_signal_nr3;
  // Generate pointers to gyroscope fall signal data
  float* gyro_x_ptr = daily_gyro_x_signal_nr3;
  float* gyro_y_ptr = daily_gyro_y_signal_nr3;
  float* gyro_z_ptr = daily_gyro_z_signal_nr3;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);

  // Makes sure the SD card is connected on the CS
  if (!SD.begin(CS)) {
    error_reporter->Report("SD card failed, or is not present");
  }

  sd.writeSignalValues(CS, "DT3.csv", acc_x_ptr, acc_y_ptr,
                        acc_z_ptr, gyro_x_ptr, gyro_y_ptr,
                        gyro_z_ptr, signal_length, true);

  float acc_x_arr[signal_length];
  float acc_y_arr[signal_length];
  float acc_z_arr[signal_length];
  float gyro_x_arr[signal_length];
  float gyro_y_arr[signal_length];
  float gyro_z_arr[signal_length];

  sd.readSignalValues(CS, "DT3.csv", acc_x_arr, acc_y_arr,
                   acc_z_arr, gyro_x_arr, gyro_y_arr, gyro_z_arr);
  
  // Generate feature array
  float features[FEATURE_AMOUNT];

  // Calculate features from signal data
  feature_calc(features, acc_x_arr, acc_y_arr, acc_z_arr,
                      gyro_x_arr, gyro_y_arr, gyro_z_arr,
                      signal_length);

  // Get a pointer to the calculated features from the data used for training the algorithm (Google Colab)
  float* daily_features_pointer = daily_features_data_nr3;

  // Check if the calculated feature data is within 5% of the features calculated for training algorithm
  // Reason for not checking if equal, is because of the SD float are saved with only 2 decimals.
  for (int i = 0; i < FEATURE_AMOUNT; i++) {
    TF_LITE_MICRO_EXPECT_NEAR(features[i], daily_features_pointer[i], abs(0.05*features[i]));
  }
}

TF_LITE_MICRO_TESTS_END
