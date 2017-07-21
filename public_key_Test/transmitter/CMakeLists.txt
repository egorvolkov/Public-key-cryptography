cmake_minimum_required(VERSION 3.6)
project(transmitter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "C:/clion_projects/NewMatrix/public_key_Test")
set(SOURCE_FILES
        bitsOperations.c
        coding.c
        createMessage.c
        getPublicKey.c
        longArithmetic.c
        main.c
        myTime.c
        myTime.h
        returnEncodedMessage.c
        transmitter.h
        utilities.c)

add_executable(transmitter ${SOURCE_FILES})