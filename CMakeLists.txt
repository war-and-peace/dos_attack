cmake_minimum_required(VERSION 3.0.0)
project(dos_attack VERSION 0.1.0)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)
add_executable(dos_attack dos_attack.cpp)
target_link_libraries(dos_attack PRIVATE Threads::Threads)
