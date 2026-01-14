cmake_minimum_required(VERSION 3.12)
# 获取模块名
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# 收集源码
file(GLOB_RECURSE ALL_COMPILE_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

# 创建可执行文件
add_executable(${MODULE_NAME} ${ALL_COMPILE_SRC})

# 如果需要额外链接选项
target_compile_options(${MODULE_NAME} PRIVATE ${MYAPP_STRICT_COMPILE_FLAGS})

# 指定第三方库路径
target_link_directories(${MODULE_NAME} PRIVATE ${THIRD_PARTY_INSTALL_DIR}/lib)

# 指定头文件搜索路径
target_include_directories(${MODULE_NAME} PRIVATE
    ${THIRD_PARTY_INSTALL_DIR}/include
)

# 链接第三方库（只链接实际存在的库）
target_link_libraries(${MODULE_NAME} PUBLIC
    ${BASE_DEPENDENCIES}
    addressbook_v2
    base
    SQLiteCpp
    sqlite3
    gtest
    gmock
)

