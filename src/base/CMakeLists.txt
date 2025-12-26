cmake_minimum_required(VERSION 3.12)
# 获取模块名
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# 收集源码
file(GLOB_RECURSE ALL_COMPILE_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

# 创建 OBJECT 库（只编译源码，不链接）
add_library(${MODULE_NAME} STATIC ${ALL_COMPILE_SRC})

# 如果需要额外链接选项
target_compile_options(${MODULE_NAME} PRIVATE ${MYAPP_STRICT_COMPILE_FLAGS})

# 链接第三方库（用 target_link_libraries 不要手动 link_directories）
target_link_libraries(${MODULE_NAME} PUBLIC
    ${BASE_DEPENDENCIES}
    spdlog
    fmt
)