cmake_minimum_required(VERSION 3.10)
#set(CMAKE_CXX_FLAGS -DNDEBUG)
set(CMAKE_CXX_STANDARD 17)
project(esel LANGUAGES C CXX)

find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)

set(COMMON_DIR ${CMAKE_SOURCE_DIR}/common)
set(IMGUI_DIR ../external/imgui)


include_directories(
  /usr/local/include/
  ${COMMON_DIR}/include
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${IMGUI_DIR}/misc
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

set( imgui_src  
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp  
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp  
  
)

set( imgui_header
  ${IMGUI_DIR}/imconfig.h
  ${IMGUI_DIR}/imgui.h
  ${IMGUI_DIR}/imgui_internal.h
  ${IMGUI_DIR}/imstb_rectpack.h
  ${IMGUI_DIR}/imstb_textedit.h
  ${IMGUI_DIR}/imstb_truetype.h
  ${IMGUI_DIR}/backends/imgui_impl_glfw.h
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.h  
)

add_library(common ${COMMON_HEADERS} ${COMMON_SRC} )
target_include_directories(common PUBLIC ${COMMON_DIR}/include)

add_library(imgui ${imgui_src} ${imgui_header})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} )
set(LIBS common glfw imgui ${Vulkan_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})