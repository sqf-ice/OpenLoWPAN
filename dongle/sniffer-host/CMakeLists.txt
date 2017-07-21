PROJECT(sniffer)

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)

SET(SOURCES
    sniffer.cpp
)

ADD_EXECUTABLE(openlowpan-sniffer ${SOURCES})
