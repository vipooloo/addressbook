#!/bin/bash
set -e

# 如果这里报错，说明你忘了在运行脚本前先运行测试程序！
echo "Capturing coverage..."
lcov --capture --directory build/ --output-file coverage.info

echo "Filtering coverage..."
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

echo "Done."