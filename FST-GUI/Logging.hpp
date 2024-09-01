#pragma once
#include <string>
#include "FST-GUI.hpp"

bool ReadRange(std::string& line, float* range);
bool ReadTriplet(std::string& line, float* triplet);
bool ReadInteger(std::string& line, int& value);
bool ReadFloat(std::string& line, float& value);
bool CheckLogLine(std::string& line, std::string& time, char& logType);
bool GetNormalFromLogLine(std::string line, float* normal, float* position);
bool GetOptionFromLine(std::string line, BlockData& blockData, unsigned int& blockStatus);
bool CheckLineForSearchInfo(std::string line, int& samplesSearched, float& lastSampleSearched);
bool CheckLineForCompletion(std::string line);