#include "Logging.hpp"

bool ReadFraction(std::string& line, int* frac) {
    if (!line.starts_with("(")) {
        return false;
    }

    int spaceIdx = line.find(")");

    if (spaceIdx == -1) {
        return false;
    }

    std::string fracStr = line.substr(1, spaceIdx - 1);

    int commaIdx = fracStr.find("/");

    if (commaIdx == -1) {
        return false;
    }

    try {
        frac[0] = std::stoi(fracStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    fracStr = fracStr.substr(commaIdx + 1);

    try {
        frac[1] = std::stoi(fracStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    line = line.substr(spaceIdx + 1);

    return true;
}

bool ReadRange(std::string& line, float* range) {
    if (!line.starts_with("[")) {
        return false;
    }

    int spaceIdx = line.find("]");

    if (spaceIdx == -1) {
        return false;
    }

    std::string rangeStr = line.substr(1, spaceIdx - 1);

    int commaIdx = rangeStr.find(",");

    if (commaIdx == -1) {
        return false;
    }

    try {
        range[0] = std::stof(rangeStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    rangeStr = rangeStr.substr(commaIdx + 1);

    try {
        range[1] = std::stof(rangeStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    line = line.substr(spaceIdx + 1);

    return true;
}

bool ReadTriplet(std::string& line, float* triplet) {
    if (!line.starts_with("(")) {
        return false;
    }

    int spaceIdx = line.find(")");

    if (spaceIdx == -1) {
        return false;
    }

    std::string tripletStr = line.substr(1, spaceIdx - 1);

    int commaIdx = tripletStr.find(",");

    if (commaIdx == -1) {
        return false;
    }

    try {
        triplet[0] = std::stof(tripletStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    tripletStr = tripletStr.substr(commaIdx + 1);

    commaIdx = tripletStr.find(",");

    if (commaIdx == -1) {
        return false;
    }

    try {
        triplet[1] = std::stof(tripletStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    tripletStr = tripletStr.substr(commaIdx + 1);

    try {
        triplet[2] = std::stof(tripletStr.substr(0, commaIdx));
    }
    catch (...) {
        return false;
    }

    line = line.substr(spaceIdx + 1);

    return true;
}

bool ReadInteger(std::string& line, int& value) {
    int spaceIdx = line.find(" ");

    if (spaceIdx == -1) {
        spaceIdx = line.length();
    }

    std::string intStr = line.substr(0, spaceIdx);

    try {
        value = std::stoi(intStr);
    }
    catch (...) {
        return false;
    }

    line = line.substr(spaceIdx);

    return true;
}

bool ReadFloat(std::string& line, float& value) {
    int spaceIdx = line.find(" ");

    if (spaceIdx == -1) {
        spaceIdx = line.length();
    }

    std::string intStr = line.substr(0, spaceIdx);

    try {
        value = std::stof(intStr);
    }
    catch (...) {
        return false;
    }

    line = line.substr(spaceIdx);

    return true;
}

bool CheckLineForSearchInfo(std::string line, int& samplesSearched, float& lastSampleSearched) {
    if (!line.starts_with("Searching - ")) {
        return false;
    }

    line = line.substr(12);

    int spaceIdx = line.find("Z = ");

    if (spaceIdx == -1) {
        return false;
    }

    line = line.substr(spaceIdx + 4);

    float currentSample;

    if (!ReadFloat(line, currentSample)) {
        return false;
    }

    if (!line.starts_with(" ")) {
        return false;
    }

    line = line.substr(1);

    int frac[2];

    if (!ReadFraction(line, frac)) {
        return false;
    }

    lastSampleSearched = currentSample;
    samplesSearched = frac[0];

    return true;
}

bool CheckLineForCompletion(std::string line) {
    return line.starts_with("Search Completed");
}

bool GetOptionFromLine(std::string line, SaveData& blockData, unsigned int& blockStatus) {
    if (!line.starts_with("Option - ")) {
        return false;
    }

    line = line.substr(9);

    int spaceIdx = line.find(" = ");

    if (spaceIdx == -1) {
        return false;
    }

    std::string optionName = line.substr(0, spaceIdx);
    line = line.substr(spaceIdx + 3);

    if (optionName == "X_Normal_Range") {
        float range[2];
        if (ReadRange(line, range)) {
            blockData.xMin = range[0];
            blockData.xMax = range[1];
            blockStatus |= 0x01;
        }
    } 
    else if (optionName == "Y_Normal_Range") {
        float range[2];
        if (ReadRange(line, range)) {
            blockData.yMin = range[0];
            blockData.yMax = range[1];
            blockStatus |= 0x02;
        }
    } 
    else if (optionName == "Z_Normal_Range") {
        float range[2];
        if (ReadRange(line, range)) {
            blockData.zMin = range[0];
            blockData.zMax = range[1];
            blockData.zModeOption = 1;
            blockStatus |= 0x04;
            blockStatus |= 0x08;
        }
    } 
    else if (optionName == "XZ_Sum_Range") {
        float range[2];
        if (ReadRange(line, range)) {
            blockData.zMin = range[0];
            blockData.zMax = range[1];
            blockData.zModeOption = 0;
            blockStatus |= 0x04;
            blockStatus &= ~0x08;
        }
    } 
    else if (optionName == "X_Normal_Samples") {
        int value;
        if (ReadInteger(line, value)) {
            blockData.xSamples = value;
            blockStatus |= 0x10;
        }
    }
    else if (optionName == "Y_Normal_Samples") {
        int value;
        if (ReadInteger(line, value)) {
            blockData.ySamples = value;
            blockStatus |= 0x20;
        }
    }
    else if (optionName == "Z_Normal_Samples") {
        int value;
        if (ReadInteger(line, value)) {
            blockData.zSamples = value;
            blockData.zModeOption = 1;
            blockStatus |= 0x40;
            blockStatus |= 0x80;
        }
    }
    else if (optionName == "XZ_Sum_Samples") {
        int value;
        if (ReadInteger(line, value)) {
            blockData.zSamples = value;
            blockData.zModeOption = 0;
            blockStatus |= 0x40;
            blockStatus &= ~0x80;
        }
    }
    else if (optionName == "Platform_Position") {
        float position[3];
        if (ReadTriplet(line, position)) {
            if (position[0] == -1945) {
                blockData.platformOption = 0;
                blockStatus |= 0x100;
            }
            else if (position[0] == -2866) {
                blockData.platformOption = 1;
                blockStatus |= 0x100;
            }
        }
    }

    return true;
}

bool CheckLogLine(std::string& line, std::string& time, char& logType) {
    logType = line[0];

    if (logType != LOG_INFO && logType != LOG_WARNING && logType != LOG_ERROR) {
        return false;
    }

    line = line.substr(1);

    if (!line.starts_with(" ")) {
        return false;
    }

    line = line.substr(1);

    if (!line.starts_with("[")) {
        return false;
    }

    int spaceIdx = line.find("] ");

    if (spaceIdx == -1) {
        return false;
    }

    time = line.substr(1, spaceIdx - 1);
    line = line.substr(spaceIdx + 1);

    if (!line.starts_with(" - ")) {
        return false;
    }

    line = line.substr(3);

    return true;

}

bool GetNormalFromLogLine(std::string line, float* normal, float* position) {
    if (!ReadTriplet(line, normal)) {
        return false;
    }

    if (!line.starts_with(" ")) {
        return false;
    }

    line = line.substr(1);

    if (!ReadTriplet(line, position)) {
        return false;
    }

    return true;
}