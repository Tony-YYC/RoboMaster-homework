cmake_minimum_required(VERSION 3.15)
project(KalmanFilter)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED )

include_directories(${Eigen3_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(.)

add_executable(${PROJECT_NAME} main.cpp kalman.cpp)
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS}  )