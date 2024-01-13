cppcheck --force --enable=all --suppressions-list=suppressions.txt --inconclusive --std=c++20 --platform=unix64 --max-ctu-depth=5 --check-level=exhaustive --error-exitcode=1 -IKRONOS/ src/
