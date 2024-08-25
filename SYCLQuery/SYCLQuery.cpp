#include "pch.h"
#include "SYCLQuery.hpp"
#include <dpct/dpct.hpp>
#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h> //dlopen
#endif

#if _WIN32
const std::string lName = "sycl7";
#else
const std::string lName = "libsycl";
#endif

bool test_library_sycl(const std::string libName) {
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

bool is_sycl_available() {
    return test_library_sycl(lName);
}

bool get_sycl_devices(std::vector<DeviceInfo>& deviceList) {
    if (!is_sycl_available()) {
        return false;
    }

    int deviceCount = dpct::dev_mgr::instance().device_count();

    int err = DPCT_CHECK_ERROR(deviceCount =
        dpct::dev_mgr::instance().device_count());

    if (err != 0) {
        return false;
    }

    for (int i = 0; i < deviceCount; i++) {
        dpct::device_info devProp;
        dpct::get_device_info(devProp, dpct::dev_mgr::instance().get_device(i));
        DeviceInfo devInfo;
        devInfo.name = devProp.get_name();
        devInfo.maxThreads = devProp.get_max_work_group_size();
        deviceList.emplace_back(devInfo);
    }

    return true;
}
