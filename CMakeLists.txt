cmake_minimum_required(VERSION 3.26)
project(crypt LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    main.cpp
    crypt.cpp
    crypt.h
    macros.h
    atbash/atbash.cpp
    atbash/atbash.h
    caesar/caesar.cpp
    caesar/caesar.h
    richelieu/richelieu.cpp
    richelieu/richelieu.h
    gronsfeld/gronsfeld.cpp
    gronsfeld/gronsfeld.h
    visionary/visionary.cpp
    visionary/visionary.h
    playfair/playfair.cpp
    playfair/playfair.h
    analysis/analysis.cpp
    analysis/analysis.h
    analysis/barcharts.cpp
    analysis/barcharts.h
    scaling/scaling.cpp
    scaling/scaling.h
    rsa/rsa.cpp
    rsa/rsa.h
    hellman/hellman.cpp
    hellman/hellman.h
    )

set(INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}
    atbash 
    caesar
    richelieu
    gronsfeld 
    visionary
    playfair
    analysis
    scaling
    rsa
    hellman
    )

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
qt_standard_project_setup()
qt_add_executable(crypt ${PROJECT_SOURCES})
target_link_libraries(crypt PRIVATE Qt6::Widgets Qt6::Charts gmpxx gmp)
target_include_directories(crypt PRIVATE ${INCLUDE_DIRECTORIES})
