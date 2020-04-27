
#include "fall_predictor.h"

#include "Arduino.h"

// Return the result of the the last prediction
// 0: Not fall, 1: Fall
int predict_fall(float* output) {
  int this_predict = -1;
  // If output[1] is bigger than output[0] this means chance of fall is bigger
  // than the chance of a not fall. The equal is there, because if the model,
  // is fifty / fifty whether it's a fall or not a fall, we chose it to be a
  // fall for safety
  if(output[1] >= output[0]) {
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
}
