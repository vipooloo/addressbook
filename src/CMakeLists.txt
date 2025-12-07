# cmake 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 工程名称
project(myapp LANGUAGES CXX C) 

# 添加子库
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt)
        # 如果是目录且存在 CMakeLists.txt，则添加子目录
        message(STATUS "Adding subdirectory: ${child}")
        add_subdirectory(${child})
    endif()
endforeach()

