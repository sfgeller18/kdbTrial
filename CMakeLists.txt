cmake_minimum_required(VERSION 3.10)
project(kbase)

execute_process(
    COMMAND bash -c "echo $QHOME"
    OUTPUT_VARIABLE QHOME_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-D__KDB__)
add_definitions(-D__YAHOO__)
#add_definitions(-D__JSON__)
add_definitions(-D__EXPERMIENTAL__)


find_package(CURL REQUIRED)



# Add the path to kdb+'s object file (e.o)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${QHOME_PATH}/l64/e.o)

# Set your source files
file(GLOB_RECURSE SRCS "src/*.cpp")

# Create the executable for C++ code
add_executable(kbase ${SRCS})

# Link against kdb+'s object file
target_link_libraries(kbase ${CURL_LIBRARIES} ${QHOME_PATH}/l64/e.o)

# Specify C++ standard for the C++ executable (optional)
set_target_properties(kbase PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

