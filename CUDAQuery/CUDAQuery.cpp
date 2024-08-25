#include "pch.h"
#include "cuda_runtime.h"
#include "CUDAQuery.hpp"
#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h> //dlopen
#endif

#if _WIN32
const std::string lName = "cudart64_110";
#else
const std::string lName = "libcuda";
#endif

bool test_library_cuda(const std::string libName) {
#if _WIN32
    auto lib = LoadLibraryA((libName + ".dll").c_str());
#else
    auto lib = dlopen((libName + ".so").c_str(), RTLD_LAZY);
#endif
    bool loaded = (lib != nullptr);

    if (loaded) {
#if _WIN32
        FreeLibrary(lib);
#else
        dlclose(lib);
#endif
    }
    return loaded;
}

bool is_cuda_available() {
    return test_library_cuda(lName);
}

bool get_cuda_devices(std::vector<DeviceInfo>& deviceList) {
    if (!is_cuda_available()) {
        return false;
    }

    deviceList.clear();

    int deviceCount;

    int err = cudaGetDeviceCount(&deviceCount);

    if (err != 0) {
        return false;
    }

    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);
        DeviceInfo devInfo;
        devInfo.name = devProp.name;
        devInfo.maxThreads = devProp.maxThreadsPerBlock;
        deviceList.emplace_back(devInfo);
    }

    return true;
}
