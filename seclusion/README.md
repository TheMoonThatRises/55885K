# Secluded Files

## Use

This space is mainly to be used to test individual pieces of code not dependent on pros api. To add to the CMakeLists.txt, add `add_executable(executablename file.cpp)` after the last executable in the file.

### Compile

To compile, first enter the directory holding the code. Then type `mkdir build && cd build`. This will create the file holding the executables. To compile, run `cmake .. && cmake --build .`. To run the executable, run `./executablename`.
