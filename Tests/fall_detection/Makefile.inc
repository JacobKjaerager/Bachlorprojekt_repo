FALL_DETECTION_TEST_SRCS := \
tensorflow/lite/micro/examples/fall_detection/fall_detection_test.cc \
tensorflow/lite/micro/examples/fall_detection/fall_model_f46.cc \
tensorflow/lite/micro/examples/fall_detection/fall_model_f46_2.cc \
tensorflow/lite/micro/examples/fall_detection/fall_model_f46_3.cc \
tensorflow/lite/micro/examples/fall_detection/daily_features_data.cc \
tensorflow/lite/micro/examples/fall_detection/fall_features_data.cc

FALL_DETECTION_TEST_HDRS := \
tensorflow/lite/micro/examples/fall_detection/fall_model_f46.h \
tensorflow/lite/micro/examples/fall_detection/fall_model_f46_2.h \
tensorflow/lite/micro/examples/fall_detection/fall_model_f46_3.h \
tensorflow/lite/micro/examples/fall_detection/daily_features_data.h \
tensorflow/lite/micro/examples/fall_detection/fall_features_data.h 

#Find any platform - specific rules for this example.
include $(wildcard tensorflow/lite/micro/examples/fall_detection/*/Makefile.inc)

# Tests loading and running the fall model.
$(eval $(call microlite_test,fall_detection_test,\
$(FALL_DETECTION_TEST_SRCS),$(FALL_DETECTION_TEST_HDRS)))


