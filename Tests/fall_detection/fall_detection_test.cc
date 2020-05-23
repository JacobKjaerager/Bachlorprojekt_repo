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

// #include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/examples/fall_detection/fall_model_f46.h" 
#include "tensorflow/lite/micro/examples/fall_detection/daily_features_data.h"
#include "tensorflow/lite/micro/examples/fall_detection/fall_features_data.h"
#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(LoadModelAndPerformInferenceDailySignal1) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(fall_model_data); 
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // This pulls in all the operation implementations we need
  tflite::ops::micro::AllOpsResolver resolver;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  const int tensor_arena_size = 90 * 1024; // Changed to a 90 from 2, since it is our model
  uint8_t tensor_arena[tensor_arena_size];

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                       tensor_arena_size, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_MICRO_EXPECT_EQ(interpreter.AllocateTensors(), kTfLiteOk);

  // Obtain a pointer to the model's input tensor
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  // The property "dims" tells us the tensor's shape. It has one element for
  // each dimension. Our input is a 2D tensor containing 46 element, so "dims"
  // should have size 2.
  TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
  // The value of each element gives the length of the corresponding tensor.
  // We should expect one single element tensor and one with the size of 46
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(46, input->dims->data[1]);
  // // The input is a 32 bit floating point value
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

  // Insert feature values on the input of the model
  const float* walk_features_pointer = daily_features_data_nr1;
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = walk_features_pointer[i];
  }

  // Run the model on this input and check that it succeeds
  TfLiteStatus invoke_status = interpreter.Invoke();
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Obtain a pointer to the model's output tensor
  TfLiteTensor* output = interpreter.output(0);

  // Make sure the output has the properties we expect
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);

  // kNotFallIndex is the index for the output tensor, that gives the chance of a not fall
  // kFallIndex is the index for the output tensor, that gives the chance of a fall
  const int kNotFallIndex = 0;
  const int kFallIndex = 1;

  // Read the output data
  float not_fall_score = output->data.f[kNotFallIndex];
  float fall_score = output->data.f[kFallIndex];

  // Not expecting a fall, therefore not_fall_score must be higher than fall_score
  TF_LITE_MICRO_EXPECT_GT(not_fall_score, fall_score);
  error_reporter->Report("Fall Chance: %f", fall_score);
}

TF_LITE_MICRO_TEST(LoadModelAndPerformInferenceDailySignal2) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(fall_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // This pulls in all the operation implementations we need
  tflite::ops::micro::AllOpsResolver resolver;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  const int tensor_arena_size = 90 * 1024; // Changed to a 90 from 2, since it is our model
  uint8_t tensor_arena[tensor_arena_size];

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                       tensor_arena_size, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_MICRO_EXPECT_EQ(interpreter.AllocateTensors(), kTfLiteOk);

  // Obtain a pointer to the model's input tensor
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  // The property "dims" tells us the tensor's shape. It has one element for
  // each dimension. Our input is a 2D tensor containing 46 element, so "dims"
  // should have size 2.
  TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
  // The value of each element gives the length of the corresponding tensor.
  // We should expect one single element tensor and one with the size of 46
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(46, input->dims->data[1]);
  // // The input is a 32 bit floating point value
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

  // Insert feature values on the input of the model
  const float* walk_features_pointer = daily_features_data_nr2;
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = walk_features_pointer[i];
  }

  // Run the model on this input and check that it succeeds
  TfLiteStatus invoke_status = interpreter.Invoke();
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Obtain a pointer to the model's output tensor
  TfLiteTensor* output = interpreter.output(0);

  // Make sure the output has the properties we expect
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);

  // kNotFallIndex is the index for the output tensor, that gives the chance of a not fall
  // kFallIndex is the index for the output tensor, that gives the chance of a fall
  const int kNotFallIndex = 0;
  const int kFallIndex = 1;

  // Read the output data
  float not_fall_score = output->data.f[kNotFallIndex];
  float fall_score = output->data.f[kFallIndex];

  // Not expecting a fall, therefore not_fall_score must be higher than fall_score
  TF_LITE_MICRO_EXPECT_GT(not_fall_score, fall_score);
  error_reporter->Report("Fall Chance: %f", fall_score);
}

TF_LITE_MICRO_TEST(LoadModelAndPerformInferenceDailySignal3) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(fall_model_data); 
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // This pulls in all the operation implementations we need
  tflite::ops::micro::AllOpsResolver resolver;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  const int tensor_arena_size = 90 * 1024; // Changed to a 90 from 2, since it is our model
  uint8_t tensor_arena[tensor_arena_size];

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                       tensor_arena_size, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_MICRO_EXPECT_EQ(interpreter.AllocateTensors(), kTfLiteOk);

  // Obtain a pointer to the model's input tensor
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  // The property "dims" tells us the tensor's shape. It has one element for
  // each dimension. Our input is a 2D tensor containing 46 element, so "dims"
  // should have size 2.
  TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
  // The value of each element gives the length of the corresponding tensor.
  // We should expect one single element tensor and one with the size of 46
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(46, input->dims->data[1]);
  // // The input is a 32 bit floating point value
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

  // Insert feature values on the input of the model
  const float* walk_features_pointer = daily_features_data_nr3;
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = walk_features_pointer[i];
  }

  // Run the model on this input and check that it succeeds
  TfLiteStatus invoke_status = interpreter.Invoke();
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Obtain a pointer to the model's output tensor
  TfLiteTensor* output = interpreter.output(0);

  // Make sure the output has the properties we expect
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);

  // kNotFallIndex is the index for the output tensor, that gives the chance of a not fall
  // kFallIndex is the index for the output tensor, that gives the chance of a fall
  const int kNotFallIndex = 0;
  const int kFallIndex = 1;

  // Read the output data
  float not_fall_score = output->data.f[kNotFallIndex];
  float fall_score = output->data.f[kFallIndex];

  // Not expecting a fall, therefore not_fall_score must be higher than fall_score
  TF_LITE_MICRO_EXPECT_GT(not_fall_score, fall_score);
  error_reporter->Report("Fall Chance: %f", fall_score);
}

TF_LITE_MICRO_TEST(LoadModelAndPerformInferenceFallSignal1) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(fall_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // This pulls in all the operation implementations we need
  tflite::ops::micro::AllOpsResolver resolver;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  const int tensor_arena_size = 90 * 1024; // Changed to a 90 from 2, since it is our model
  uint8_t tensor_arena[tensor_arena_size];

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                       tensor_arena_size, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_MICRO_EXPECT_EQ(interpreter.AllocateTensors(), kTfLiteOk);

  // Obtain a pointer to the model's input tensor
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  // The property "dims" tells us the tensor's shape. It has one element for
  // each dimension. Our input is a 2D tensor containing 46 element, so "dims"
  // should have size 2.
  TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
  // The value of each element gives the length of the corresponding tensor.
  // We should expect one single element tensor and one with the size of 46
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(46, input->dims->data[1]);
  // // The input is a 32 bit floating point value
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

  // Insert feature values on the input of the model
  const float* walk_features_pointer = falling_features_data_nr1;
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = walk_features_pointer[i];
  }

  // Run the model on this input and check that it succeeds
  TfLiteStatus invoke_status = interpreter.Invoke();
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Obtain a pointer to the model's output tensor
  TfLiteTensor* output = interpreter.output(0);

  // Make sure the output has the properties we expect
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);

  // kNotFallIndex is the index for the output tensor, that gives the chance of a not fall
  // kFallIndex is the index for the output tensor, that gives the chance of a fall
  const int kNotFallIndex = 0;
  const int kFallIndex = 1;

  // Read the output data
  float not_fall_score = output->data.f[kNotFallIndex];
  float fall_score = output->data.f[kFallIndex];

  // Expecting a fall, therefore fall_score must be higher than not_fall_score
  TF_LITE_MICRO_EXPECT_GT(fall_score, not_fall_score);
  error_reporter->Report("Fall Chance: %f", fall_score);
}

TF_LITE_MICRO_TEST(LoadModelAndPerformInferenceFallSignal2) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(fall_model_data); 
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // This pulls in all the operation implementations we need
  tflite::ops::micro::AllOpsResolver resolver;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  const int tensor_arena_size = 90 * 1024; // Changed to a 90 from 2, since it is our model
  uint8_t tensor_arena[tensor_arena_size];

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                       tensor_arena_size, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_MICRO_EXPECT_EQ(interpreter.AllocateTensors(), kTfLiteOk);

  // Obtain a pointer to the model's input tensor
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  // The property "dims" tells us the tensor's shape. It has one element for
  // each dimension. Our input is a 2D tensor containing 46 element, so "dims"
  // should have size 2.
  TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
  // The value of each element gives the length of the corresponding tensor.
  // We should expect one single element tensor and one with the size of 46
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(46, input->dims->data[1]);
  // // The input is a 32 bit floating point value
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

  // Insert feature values on the input of the model
  const float* walk_features_pointer = falling_features_data_nr2;
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = walk_features_pointer[i];
  }

  // Run the model on this input and check that it succeeds
  TfLiteStatus invoke_status = interpreter.Invoke();
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Obtain a pointer to the model's output tensor
  TfLiteTensor* output = interpreter.output(0);

  // Make sure the output has the properties we expect
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);

  // kNotFallIndex is the index for the output tensor, that gives the chance of a not fall
  // kFallIndex is the index for the output tensor, that gives the chance of a fall
  const int kNotFallIndex = 0;
  const int kFallIndex = 1;

  // Read the output data
  float not_fall_score = output->data.f[kNotFallIndex];
  float fall_score = output->data.f[kFallIndex];

  // Expecting a fall, therefore fall_score must be higher than not_fall_score
  TF_LITE_MICRO_EXPECT_GT(fall_score, not_fall_score);
  error_reporter->Report("Fall Chance: %f", fall_score);
}

TF_LITE_MICRO_TEST(LoadModelAndPerformInferenceFallSignal3) {
  // Set up logging
  tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = ::tflite::GetModel(fall_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.\n",
                         model->version(), TFLITE_SCHEMA_VERSION);
  }

  // This pulls in all the operation implementations we need
  tflite::ops::micro::AllOpsResolver resolver;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  const int tensor_arena_size = 90 * 1024; 
  uint8_t tensor_arena[tensor_arena_size];

  // Build an interpreter to run the model with
  tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                       tensor_arena_size, error_reporter);

  // Allocate memory from the tensor_arena for the model's tensors
  TF_LITE_MICRO_EXPECT_EQ(interpreter.AllocateTensors(), kTfLiteOk);

  // Obtain a pointer to the model's input tensor
  TfLiteTensor* input = interpreter.input(0);

  // Make sure the input has the properties we expect
  TF_LITE_MICRO_EXPECT_NE(nullptr, input);
  // The property "dims" tells us the tensor's shape. It has one element for
  // each dimension. Our input is a 2D tensor containing 46 element, so "dims"
  // should have size 2.
  TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
  // The value of each element gives the length of the corresponding tensor.
  // We should expect one single element tensor and one with the size of 46
  TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(46, input->dims->data[1]);
  // // The input is a 32 bit floating point value
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

  // Insert feature values on the input of the model
  const float* walk_features_pointer = falling_features_data_nr3;
  for (int i = 0; i < (input->bytes / sizeof(float)); ++i) {
    input->data.f[i] = walk_features_pointer[i];
  }

  // Run the model on this input and check that it succeeds
  TfLiteStatus invoke_status = interpreter.Invoke();
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

  // Obtain a pointer to the model's output tensor
  TfLiteTensor* output = interpreter.output(0);

  // Make sure the output has the properties we expect
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
  TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
  TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);

  // kNotFallIndex is the index for the output tensor, that gives the chance of a not fall
  // kFallIndex is the index for the output tensor, that gives the chance of a fall
  const int kNotFallIndex = 0;
  const int kFallIndex = 1;

  // Read the output data
  float not_fall_score = output->data.f[kNotFallIndex];
  float fall_score = output->data.f[kFallIndex];

  // Expecting a fall, therefore fall_score must be higher than not_fall_score
  TF_LITE_MICRO_EXPECT_GT(fall_score, not_fall_score);
  error_reporter->Report("Fall Chance: %f", fall_score);
}


TF_LITE_MICRO_TESTS_END



