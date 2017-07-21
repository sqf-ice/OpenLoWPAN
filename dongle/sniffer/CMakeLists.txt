SET(STM32_CHIP STM32F072x8)
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../stm32-cmake/cmake/gcc_stm32.cmake)
SET(CHIBIOS_ROOT ${CMAKE_SOURCE_DIR}/../../ChibiOS)

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)

PROJECT(dongle-sniffer)

ENABLE_LANGUAGE(ASM)

FIND_PACKAGE(ChibiOS 17 COMPONENTS rt hal gpt ext pal serial serial_usb spi st usb queues buffers REQUIRED)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${ChibiOS_INCLUDE_DIRS}
    ../board
    ../../6LoWPAN
    ../../PHY
)

ADD_DEFINITIONS(-DCORTEX_USE_FPU=FALSE)

SET(STM32_LINKER_SCRIPT ${ChibiOS_LINKER_SCRIPT})

SET(HEADER_FILES
    ../../PHY/cc2520_reg.h
    ../../PHY/cc2520.h
    ../../6LoWPAN/mac802.14.5.h
    chconf.h
    halconf.h
    mcuconf.h
    usb.h
    sniffer.h
)

SET(SOURCE_FILES 
    ../../PHY/cc2520.c
    ../../6LoWPAN/mac802.14.5.c
    ../board/board.c
    main.c 
    usb.c
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${SOURCE_FILES} ${HEADER_FILES} ${ChibiOS_SOURCES})

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}.elf)
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME}.elf)
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME}.elf)
