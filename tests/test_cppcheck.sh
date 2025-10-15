#!/bin/bash
echo "Running cppcheck..."
cppcheck --std=c++17 --language=c++ \
         --inline-suppr --suppress=missingIncludeSystem \
         --suppress=unusedFunction \
         --enable=all --force -q --error-exitcode=1 ../src/ || exit 1
exit 0