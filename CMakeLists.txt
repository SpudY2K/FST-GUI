cmake_minimum_required(VERSION 3.15)

project(FST-GUI-APP
	DESCRIPTION "A GUI for the FST brute forcer"
	LANGUAGES CXX CUDA
)

# Library components
add_subdirectory(FST-GUI)
add_subdirectory(CUDAQuery)
add_subdirectory(SYCLQuery)


if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type." FORCE)
endif()
