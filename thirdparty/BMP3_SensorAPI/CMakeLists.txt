cmake_minimum_required(VERSION 3.28)
project(BMP3_SensorAPI)

# If the third-party library has its own source files
add_library(BMP3_SensorAPI STATIC 
    BMP3_SensorAPI/bmp3_defs.h
    BMP3_SensorAPI/bmp3.h
    BMP3_SensorAPI/bmp3.c
)
add_library(BMP3_SensorAPI::BMP3_SensorAPI ALIAS BMP3_SensorAPI)

target_include_directories(BMP3_SensorAPI PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)