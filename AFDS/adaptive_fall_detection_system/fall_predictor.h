

/**
  *
  *
  * @file:    fall_predictor.h
  * @date:    26-03-2020 09:32:41
  * @author:  Morten Sahlertz
  *
  * @brief    Predictor for whether or not a fall has occured
  *
  *
  *
  *
**/

#ifndef FALL_PREDICTOR_H_
#define FALL_PREDICTOR_H_

/**
* @brief  predict_fall(): Predict whether or not a fall has occured
*
*   Uses a pointer to the output tensors of the model to predict whether 
*   or not a fall has occured.
*
* @param float* output: Pointer to the output tensor of the TensorFlow Lite model
* @return int: 1 = fall detected, 0 = no fall detected.
*
**/
int predict_fall(float* output);

#endif  // FALL_PREDICTOR_H_
