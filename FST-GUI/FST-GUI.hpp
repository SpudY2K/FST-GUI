#pragma once
#include <wx/wx.h>
#include <filesystem>
#include <list>
#include "Common/DeviceInfo.hpp"

enum LogType {
    LOG_INFO = 'I',
    LOG_WARNING = 'W',
    LOG_ERROR = 'E'
};

enum GPU_MODE {
    MODE_CUDA = 0,
    MODE_SYCL = 1,
    MODE_NONE = 2
};

struct SaveData {
    int version = 1000;

    int platformOption = 0;
    int zModeOption = 0;

    float xMin = 0.20;
    float xMax = 0.21;
    float yMin = 0.85;
    float yMax = 0.86;
    float zMin = 0.57;
    float zMax = 0.58;

    int xSamples = 41;
    int ySamples = 41;
    int zSamples = 41;

    std::filesystem::path outputDirectory = std::filesystem::current_path().append("results");

    int gpuDeviceID = 0;
    int maxThreads = 256;

    int MAX_UPWARP_SOLUTIONS = 10000000;
    int MAX_PLAT_SOLUTIONS = 200000;

    int MAX_SK_PHASE_ONE = 50000;
    int MAX_SK_PHASE_TWO_A = 50000;
    int MAX_SK_PHASE_TWO_B = 50000;
    int MAX_SK_PHASE_TWO_C = 5000000;
    int MAX_SK_PHASE_TWO_D = 5000000;
    int MAX_SK_PHASE_THREE = 4000000;
    int MAX_SK_PHASE_FOUR = 5000000;
    int MAX_SK_PHASE_FIVE = 5000000;
    int MAX_SK_PHASE_SIX = 200000;

    int MAX_SK_UPWARP_SOLUTIONS = 100000;
    int MAX_SPEED_SOLUTIONS = 100000000;
    int MAX_10K_SOLUTIONS = 500000;
    int MAX_SLIDE_SOLUTIONS = 2000000;
    int MAX_BD_SOLUTIONS = 50000;

    int MAX_DOUBLE_10K_SOLUTIONS = 300000;
    int MAX_BULLY_PUSH_SOLUTIONS = 900000;

    int MAX_SQUISH_SPOTS = 5000;
    int MAX_STRAIN_SETUPS = 500000;

#ifdef _WIN32
    std::filesystem::path cudaExecutablePath = std::filesystem::current_path().append("fst-brute-forcer.exe");
    std::filesystem::path syclExecutablePath = std::filesystem::current_path().append("fst-brute-forcer-sycl.exe");
#else
    std::filesystem::path cudaExecutablePath = std::filesystem::current_path().append("fst-brute-forcer");
    std::filesystem::path syclExecutablePath = std::filesystem::current_path().append("fst-brute-forcer-sycl");
#endif // _WIN32

    GPU_MODE gpuModeSelected = MODE_NONE;
};

class FST_GUI;

class BlockQueue
{
public:
    BlockQueue(FST_GUI* f);

    std::list<SaveData>::iterator queueBegin();
    bool queueEmpty();
    int queueLength();
    bool addBlockToQueue(SaveData newBlock);
    bool addBlockToQueue();
    SaveData removeBlockFromQueue(int index);
    SaveData getNextBlockInQueue();
    void getQueueStrings(wxArrayString& queueStrings, bool running);
    void clearQueue(bool removeFirst);

private:
    std::list<SaveData> queue;
    FST_GUI* fst_gui;
};

bool compareBlocks(SaveData* s1, SaveData* s2);

class FST_GUI : public wxApp
{
public:
    SaveData saveStruct;
    BlockQueue blockQueue;

    bool syclEnabled = false;
    bool cudaEnabled = false;

    const std::vector<DeviceInfo>& deviceList();
    std::filesystem::path executablePath();
    std::filesystem::path executablePath(SaveData* blockData);

    FST_GUI();
    virtual ~FST_GUI();
    virtual bool OnInit() override;

    void loadToSaveStruct(std::ifstream& fs, SaveData* save);
    void loadSave();
    void saveFromSaveStruct(std::ofstream& fs, SaveData* save);
    void saveSave();
    bool updateExecutableFile();
    bool updateExecutableFile(SaveData* blockData);
    bool checkExecutable();
    bool checkExecutable(SaveData* blockData);
    bool askGPUMode();

private:
    const std::filesystem::path saveFile = std::filesystem::current_path().append("fstguiSave.bin");
    std::vector<DeviceInfo> cudaDeviceList;
    std::vector<DeviceInfo> syclDeviceList;
};

bool findExecutableFile(std::filesystem::path& executablePath);