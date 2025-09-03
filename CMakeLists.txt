cmake_minimum_required(VERSION 3.12)
project(MyBTCClone VERSION 0.1 LANGUAGES CXX)

# Use C++17 (or higher)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# Optionally link to a local external secp256k1
# include_directories(${PROJECT_SOURCE_DIR}/external/secp256k1/include)

# Define your source files
set(SOURCES
    src/main.cpp
    src/blockchain.cpp
    src/block.cpp
    src/transaction.cpp
    src/wallet.cpp
    src/utils.cpp
)

# Define the executable
add_executable(mybtc ${SOURCES})

# Link against system-installed secp256k1
find_library(SECP256K1_LIB secp256k1 REQUIRED)

target_link_libraries(mybtc
    ${SECP256K1_LIB}
)

# Optionally link more libraries
# target_link_libraries(mybtc crypto)

# Add warnings for good practice
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mybtc PRIVATE -Wall -Wextra -Wpedantic)
endif()
