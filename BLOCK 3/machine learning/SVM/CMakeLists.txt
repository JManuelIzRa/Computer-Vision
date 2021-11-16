cmake_minimum_required (VERSION 3.0)
project (backg_moodle_v01)
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(OpenCV  REQUIRED)

LINK_LIBRARIES(${OpenCV_LIBS})

add_executable(svm svm.cpp)
add_executable(svm2 svm2.cpp)