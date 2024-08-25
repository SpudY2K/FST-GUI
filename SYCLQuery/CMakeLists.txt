add_library(SYCLQuery STATIC
	"SYCLQuery.cpp"
	"SYCLQuery.hpp")

target_include_directories(SYCLQuery PRIVATE /opt/intel/oneapi/2024.2/include/)
target_include_directories(SYCLQuery PRIVATE /opt/intel/oneapi/2024.2/include/sycl)
target_include_directories(SYCLQuery PRIVATE ..)

set(CMAKE_C_COMPILER "/opt/intel/oneapi/2024.2/bin/icx")
set(CMAKE_CXX_COMPILER "/opt/intel/oneapi/2024.2/bin/icpx")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -fsycl -fsycl-unnamed-lambda -fsycl-targets=amdgcn-amd-amdhsa,spir64 -Xsycl-target-backend=amdgcn-amd-amdhsa --offload-arch=gfx1030 -Wno-c++11-narrowing")
