cmake_minimum_required(VERSION 3.6)
project(receiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "C:/clion_projects/public_key_Test")
set(SOURCE_FILES
        bitsOperations.c
        computePublicKey.c
        decoding.c
        generateSecretKey.c
        getEncodedMessage.c
        main.c
        modularArithmetic.c
        modules.txt
        myTime.c
        myTime.h
        receiver.h
        returnPublicKey.c
        transmitterConnection.c
        utilities.c)

add_executable(receiver ${SOURCE_FILES})