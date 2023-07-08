cmake_minimum_required(VERSION 3.26)
project(crypt CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC
    main.cpp
    crypt.cpp
    alphabet.cpp
    atbash/atbash.cpp
    caesar/caesar.cpp
    richelieu/richelieu.cpp
    gronsfeld/gronsfeld.cpp
    visionary/visionary.cpp
    playfair/playfair.cpp
    scaling/scaling.cpp
    analysis/analysis.cpp
    analysis/barcharts.cpp
    rsa/rsa.cpp
    hellman/hellman.cpp
    )

set(INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}
    atbash
    caesar
    richelieu
    gronsfeld
    visionary
    playfair
    scaling
    analysis
    rsa
    hellman
    )

set(LIB
    Qt6::Widgets
    Qt6::Charts
    gmpxx 
    gmp
    )

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
qt_standard_project_setup()
qt_add_executable(crypt ${SRC})
target_link_libraries(crypt PRIVATE ${LIB})
target_include_directories(crypt PRIVATE ${INCLUDE})
