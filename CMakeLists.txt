cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(esel LANGUAGES C CXX)

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

set(COMMON_DIR ${CMAKE_SOURCE_DIR}/common)

include_directories(
  /usr/local/include/
  ${COMMON_DIR}/include
)

file(GLOB_RECURSE SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.c
)
file(GLOB_RECURSE HEADER_FILES
  ${CMAKE_SOURCE_DIR}/include/*.h 
  ${CMAKE_SOURCE_DIR}/include/*.hpp
)

file(GLOB_RECURSE COMMON_HEADERS
  ${COMMON_DIR}/include/*.h 
  ${COMMON_DIR}/include/*.hpp 
)

file(GLOB_RECURSE COMMON_SRC
  ${COMMON_DIR}/src/*.c 
  ${COMMON_DIR}/src/*.cpp 
)

add_library(common ${COMMON_HEADERS} ${COMMON_SRC})
target_include_directories(common PUBLIC ${COMMON_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} )
set(LIBS common glfw ${Vulkan_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})