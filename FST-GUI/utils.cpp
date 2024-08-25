#include "utils.hpp"
#include <sstream>
#include <iomanip>

std::string float2string(float num, int precision) {
    std::ostringstream oss;
    oss << std::setprecision(precision) << std::noshowpoint << num;
    std::string str = oss.str();
    return str;
}