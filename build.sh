#!/bin/bash

set -e   # 出错即退出
set -o pipefail

# ========================
# 基本变量
# ========================
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")


LOG_FILE="run_${TIMESTAMP}.log"
# ========================
# 日志重定向（关键）
# ========================
exec > >(tee -a "${LOG_FILE}") 2>&1
# ========================
# 任务函数
# ========================
do_build() {
    echo "===== BUILD ====="
    if [ ! -d "build" ]; then
        # 若不存在则创建
        mkdir build
        echo "已创建build目录"
    else
        echo "build目录已存在，无需创建"
    fi

    cd build
    cmake ..
    make -j$(nproc)
    ./bin/ut
    cd -
}

do_run() {
    echo "===== RUN ====="
    cd build
    ./bin/ut
    cd -
}

do_cppcheck() {
    echo "===== CPPCHECK ====="
    cppcheck \
        --enable=all \
        --inconclusive \
        --std=c++11 \
        --suppress=missingIncludeSystem \
        --suppress=unusedFunction \
        --suppress=unreadVariable \
        ./src/addressbook_v2
}

do_coverage() {
    echo "===== COVERAGE ====="
    lcov --capture --directory build/ --output-file coverage.info
    # 将多次 remove 合并为一次，这是安全的优化
    lcov --remove coverage.info \
        '/usr/*' \
        '*/SQLiteCpp/*' \
        '*/third_party/*' \
        '*/ut/*' \
        '*/log/*' \
        --output-file coverage.info

    echo "Generating HTML..."
    genhtml coverage.info --output-directory coverage_report
}

do_doxygen() {
    echo "===== DOXYGEN ====="
    #doxygen Doxyfile
}

do_clean()
{
    echo "===== CLEAN ====="
    rm -rf *.log
    rm -rf *.info
    rm -rf coverage_report
    rm -rf build
}
# ========================
# 默认执行全部
# ========================
do_all() {
    do_clean
    do_build
    do_run
    do_cppcheck
    do_coverage
    do_doxygen
}

# ========================
# 参数解析
# ========================
if [ $# -eq 0 ]; then
    echo "No arguments, run ALL tasks"
    do_all
else
    for arg in "$@"; do
        case $arg in
            build)
                do_build
                ;;
            run)
                do_run
                ;;
            cppcheck)
                do_cppcheck
                ;;
            coverage)
                do_coverage
                ;;
            clean)
                do_clean
                ;;
            doxygen)
                do_doxygen
                ;;
            all)
                do_all
                ;;
            *)
                echo "Unknown argument: $arg"
                echo "Supported: build run cppcheck coverage doxygen all"
                exit 1
                ;;
        esac
    done
fi
