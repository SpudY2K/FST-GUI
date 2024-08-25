#pragma once
#include <vector>
#include "Common/DeviceInfo.hpp"

bool get_cuda_devices(std::vector<struct DeviceInfo>& deviceList);
bool is_cuda_available();