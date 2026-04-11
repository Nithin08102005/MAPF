# project(lifelong)

# cmake_minimum_required (VERSION 3.5)
# set(CMAKE_CXX_STANDARD 17)

# IF(NOT CMAKE_BUILD_TYPE)
#     #SET(CMAKE_BUILD_TYPE "DEBUG")
#     SET(CMAKE_BUILD_TYPE "RELEASE")
#     #SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
#     #SET(CMAKE_BUILD_TYPE "MINSIZEREL")
# ENDIF()

# include_directories("inc")
# file(GLOB SOURCES "src/*.cpp")

# # Find Boost
# find_package( Boost 1.49.0 REQUIRED COMPONENTS program_options system filesystem)
# include_directories( ${Boost_INCLUDE_DIRS} )


# add_executable(lifelong ${SOURCES} )
# target_link_libraries(lifelong ${Boost_LIBRARIES})
# 1. ALWAYS call this first
cmake_minimum_required(VERSION 3.15) 

# 2. Then name the project
project(lifelong)

# Export compile commands for IDE IntelliSense (VS Code / Cursor C++ extension)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 3. Suppress the FindBoost deprecation warning (Policy CMP0167)
cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 14)
if(MSVC)
    add_compile_options(/permissive-) # Fixes 'or', 'and', etc.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    # These 3 lines fix the "boost_protected_member" errors
    add_definitions(-DBOOST_DESCRIBE_CXX14)
    add_definitions(-DBOOST_ALLOW_DEPRECATED_HEADERS)
    add_definitions(-D_HAS_AUTO_PTR_ETC=1)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# 4. Find Boost using modern targets
find_package(Boost REQUIRED COMPONENTS program_options system filesystem heap graph)

# 5. Define sources (Note: GLOB is generally discouraged for large projects, 
# but fine for small ones. Just remember to re-run CMake if you add a new .cpp file!)
file(GLOB SOURCES "src/*.cpp")

add_executable(lifelong ${SOURCES})

# 6. Modern Linking: This automatically handles include directories and dependencies
target_include_directories(lifelong PRIVATE "inc")
target_link_libraries(lifelong PRIVATE 
    Boost::program_options 
    Boost::system 
    Boost::filesystem
)