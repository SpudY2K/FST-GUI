#pragma once
#include <vector>
#include "Common/DeviceInfo.hpp"

bool is_sycl_available();
bool get_sycl_devices(std::vector<struct DeviceInfo>& deviceList);