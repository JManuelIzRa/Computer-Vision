CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(trafic_sign_classifier)
ENABLE_LANGUAGE(CXX)
SET (CMAKE_CXX_STANDARD 11) 

FIND_PACKAGE(OpenCV 4.2	REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(train_val train_val.cpp common_code.cpp common_code.hpp metrics.cpp metrics.hpp)
add_executable(test_image test_image.cpp common_code.cpp common_code.hpp )
