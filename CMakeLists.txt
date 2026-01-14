cmake_minimum_required(VERSION 3.12)
project(myapp LANGUAGES CXX C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

add_compile_options(-O0 -g)

option(ENABLE_COVERAGE "Enable coverage" ON)
option(ENABLE_ASAN "Enable AddressSanitizer" ON)
option(ENABLE_GTEST "Enable GTest" ON)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

if(ENABLE_ASAN)
    message(STATUS "Building with AddressSanitizer")
    set(ASAN_FLAGS ${ASAN_FLAGS} "-fsanitize=address,undefined")
    set(ASAN_FLAGS ${ASAN_FLAGS} "-fno-sanitize=signed-integer-overflow")
    set(ASAN_FLAGS ${ASAN_FLAGS} "-fno-sanitize-recover=all")
    set(ASAN_FLAGS ${ASAN_FLAGS} "-fno-omit-frame-pointer")
    set(ASAN_FLAGS ${ASAN_FLAGS} "-fsanitize=address,undefined")
    add_compile_options(${ASAN_FLAGS})
    add_link_options(${ASAN_FLAGS})
endif()

# 依赖通用库
set(BASE_DEPENDENCIES ${CMAKE_DL_LIBS} pthread ssl crypto)

# 定义严格编译选项（统一管理）
set(MYAPP_STRICT_COMPILE_FLAGS
    -Wall                        # 启用大部分常见警告
    -Wextra                      # 启用额外警告（-Wall 没覆盖的）
    -Werror                      # 将所有警告视为错误
    -Wpedantic                   # 严格遵循 C/C++ 标准
    #-Wconversion                 # 警告隐式类型转换可能导致的数据丢失
    #-Wsign-conversion            # 警告有符号/无符号类型之间的转换
    -Wshadow                     # 警告局部变量遮蔽同名变量
    -Wnon-virtual-dtor           # 警告基类有虚函数但析构函数非虚
    -Woverloaded-virtual         # 警告函数重载可能隐藏基类虚函数
    -Wno-error=unused-parameter  # 忽略未使用函数参数的错误，即使 -Werror 生效
    -Werror=old-style-cast      # 警告 C 风格的强制转换
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

#--------------------------------------------------------------
# 自动 include 源码里的 include 目录
execute_process(
    COMMAND find ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/third_party -type d -name include -print
    OUTPUT_VARIABLE MY_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\n" ";" MY_INCLUDE_DIRS "${MY_INCLUDE_DIRS}")
foreach(dir ${MY_INCLUDE_DIRS})
    message(STATUS "Adding own include directory: ${dir}")
    # 自己代码的 include（严格检查）
    include_directories(${dir})
endforeach()
#--------------------------------------------------------------

# 添加源码子目录
add_subdirectory(src)
add_subdirectory(third_party/fmt)
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/SQLiteCpp)

# 单元测试
if(ENABLE_GTEST)
    add_subdirectory(third_party/googletest)
    add_subdirectory(ut)
endif()
