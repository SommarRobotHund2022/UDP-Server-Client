cmake_minimum_required(VERSION 3.0.0)
project(UDP_CAM CXX)

add_executable(${PROJECT_NAME} ./udp_streamer.cc)

target_link_directories(${PROJECT_NAME} PRIVATE
    "/usr/lib/arm-linux-gnueabih/"
    "./"
)

target_link_libraries(${PROJECT_NAME}
    opencv_videoio 
    opencv_photo
    opencv_highgui
    opencv_core 
    opencv_imgproc 
    opencv_imgcodecs
    raspicam 
    raspicam_cv 
    stdc++ 
    sockpp 
)

target_include_directories(${PROJECT_NAME} PRIVATE
    /usr/include/opencv4
    ./sockpp/include
    ./raspicam-0.1.9/src/
)