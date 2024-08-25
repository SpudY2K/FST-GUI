#include <sstream>
#include <iomanip>
#include <fstream>
#include <wx/wx.h>
#include "FST-GUI.hpp"
#include "MainFrame.hpp"
#include "utils.hpp"
#include "SYCLQuery/SYCLQuery.hpp"
#include "CUDAQuery/CUDAQuery.hpp"

FST_GUI::FST_GUI() : blockQueue(this)
{
    loadSave();
}

FST_GUI::~FST_GUI()
{
    saveSave();
}

void FST_GUI::loadSave() {
    if (!std::filesystem::exists(this->saveFile)) {
        saveSave();
    }
    else {
        std::ifstream fs(this->saveFile, std::ios::out | std::ios::binary);

        if (fs) {
            int saveVersion;
            fs.read((char*)(&saveVersion), sizeof saveVersion);

            if (saveVersion == this->saveStruct.version) {
                fs.read((char*)(&this->saveStruct.platformOption), sizeof this->saveStruct.platformOption);
                fs.read((char*)(&this->saveStruct.zModeOption), sizeof this->saveStruct.zModeOption);
                fs.read((char*)(&this->saveStruct.xMin), sizeof this->saveStruct.xMin);
                fs.read((char*)(&this->saveStruct.xMax), sizeof this->saveStruct.xMax);
                fs.read((char*)(&this->saveStruct.yMin), sizeof this->saveStruct.yMin);
                fs.read((char*)(&this->saveStruct.yMax), sizeof this->saveStruct.yMax);
                fs.read((char*)(&this->saveStruct.zMin), sizeof this->saveStruct.zMin);
                fs.read((char*)(&this->saveStruct.zMax), sizeof this->saveStruct.zMax);
                fs.read((char*)(&this->saveStruct.xSamples), sizeof this->saveStruct.xSamples);
                fs.read((char*)(&this->saveStruct.ySamples), sizeof this->saveStruct.ySamples);
                fs.read((char*)(&this->saveStruct.zSamples), sizeof this->saveStruct.zSamples);
                int outStringSize;
                fs.read((char*)(&outStringSize), sizeof outStringSize);
                std::string outputDirectoryStr = "";
                outputDirectoryStr.resize(outStringSize);
                fs.read((char*)(outputDirectoryStr.c_str()), outStringSize);
                this->saveStruct.outputDirectory = std::filesystem::path(outputDirectoryStr);
                fs.read((char*)(&this->saveStruct.gpuDeviceID), sizeof this->saveStruct.gpuDeviceID);
                fs.read((char*)(&this->saveStruct.maxThreads), sizeof this->saveStruct.maxThreads);
                fs.read((char*)(&this->saveStruct.MAX_UPWARP_SOLUTIONS), sizeof this->saveStruct.MAX_UPWARP_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_PLAT_SOLUTIONS), sizeof this->saveStruct.MAX_PLAT_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_ONE), sizeof this->saveStruct.MAX_UPWARP_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_A), sizeof this->saveStruct.MAX_SK_PHASE_TWO_A);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_B), sizeof this->saveStruct.MAX_SK_PHASE_TWO_B);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_C), sizeof this->saveStruct.MAX_SK_PHASE_TWO_C);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_D), sizeof this->saveStruct.MAX_SK_PHASE_TWO_D);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_THREE), sizeof this->saveStruct.MAX_SK_PHASE_THREE);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_FOUR), sizeof this->saveStruct.MAX_SK_PHASE_FOUR);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_FIVE), sizeof this->saveStruct.MAX_SK_PHASE_FIVE);
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_SIX), sizeof this->saveStruct.MAX_SK_PHASE_SIX);
                fs.read((char*)(&this->saveStruct.MAX_SK_UPWARP_SOLUTIONS), sizeof this->saveStruct.MAX_SK_UPWARP_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_SPEED_SOLUTIONS), sizeof this->saveStruct.MAX_SPEED_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_10K_SOLUTIONS), sizeof this->saveStruct.MAX_10K_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_SLIDE_SOLUTIONS), sizeof this->saveStruct.MAX_SLIDE_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_BD_SOLUTIONS), sizeof this->saveStruct.MAX_BD_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_DOUBLE_10K_SOLUTIONS), sizeof this->saveStruct.MAX_DOUBLE_10K_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_BULLY_PUSH_SOLUTIONS), sizeof this->saveStruct.MAX_BULLY_PUSH_SOLUTIONS);
                fs.read((char*)(&this->saveStruct.MAX_SQUISH_SPOTS), sizeof this->saveStruct.MAX_SQUISH_SPOTS);
                fs.read((char*)(&this->saveStruct.MAX_STRAIN_SETUPS), sizeof this->saveStruct.MAX_STRAIN_SETUPS);
                fs.read((char*)(&outStringSize), sizeof outStringSize);
                std::string cudaExecutablePathStr = "";
                cudaExecutablePathStr.resize(outStringSize);
                fs.read((char*)(cudaExecutablePathStr.c_str()), outStringSize);
                this->saveStruct.cudaExecutablePath = std::filesystem::path(cudaExecutablePathStr);
                fs.read((char*)(&outStringSize), sizeof outStringSize);
                std::string syclExecutablePathStr = "";
                syclExecutablePathStr.resize(outStringSize);
                fs.read((char*)(syclExecutablePathStr.c_str()), outStringSize);
                this->saveStruct.syclExecutablePath = std::filesystem::path(syclExecutablePathStr);
                fs.read((char*)(&this->saveStruct.gpuModeSelected), sizeof this->saveStruct.gpuModeSelected);
            }
        }
        fs.close();
    }
}

void FST_GUI::saveSave() {
    std::ofstream fs(this->saveFile, std::ios::out | std::ios::binary);

    if (fs) {
        fs.write((char*)(&this->saveStruct.version), sizeof this->saveStruct.version);
        fs.write((char*)(&this->saveStruct.platformOption), sizeof this->saveStruct.platformOption);
        fs.write((char*)(&this->saveStruct.zModeOption), sizeof this->saveStruct.zModeOption);
        fs.write((char*)(&this->saveStruct.xMin), sizeof this->saveStruct.xMin);
        fs.write((char*)(&this->saveStruct.xMax), sizeof this->saveStruct.xMax);
        fs.write((char*)(&this->saveStruct.yMin), sizeof this->saveStruct.yMin);
        fs.write((char*)(&this->saveStruct.yMax), sizeof this->saveStruct.yMax);
        fs.write((char*)(&this->saveStruct.zMin), sizeof this->saveStruct.zMin);
        fs.write((char*)(&this->saveStruct.zMax), sizeof this->saveStruct.zMax);
        fs.write((char*)(&this->saveStruct.xSamples), sizeof this->saveStruct.xSamples);
        fs.write((char*)(&this->saveStruct.ySamples), sizeof this->saveStruct.ySamples);
        fs.write((char*)(&this->saveStruct.zSamples), sizeof this->saveStruct.zSamples);
        std::string outputDirectoryStr = this->saveStruct.outputDirectory.string();
        int outStringSize = outputDirectoryStr.size();
        fs.write((char*)(&outStringSize), sizeof outStringSize);
        fs.write((char*)(outputDirectoryStr.c_str()), outStringSize);
        fs.write((char*)(&this->saveStruct.gpuDeviceID), sizeof this->saveStruct.gpuDeviceID);
        fs.write((char*)(&this->saveStruct.maxThreads), sizeof this->saveStruct.maxThreads);
        fs.write((char*)(&this->saveStruct.MAX_UPWARP_SOLUTIONS), sizeof this->saveStruct.MAX_UPWARP_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_PLAT_SOLUTIONS), sizeof this->saveStruct.MAX_PLAT_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_ONE), sizeof this->saveStruct.MAX_UPWARP_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_A), sizeof this->saveStruct.MAX_SK_PHASE_TWO_A);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_B), sizeof this->saveStruct.MAX_SK_PHASE_TWO_B);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_C), sizeof this->saveStruct.MAX_SK_PHASE_TWO_C);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_D), sizeof this->saveStruct.MAX_SK_PHASE_TWO_D);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_THREE), sizeof this->saveStruct.MAX_SK_PHASE_THREE);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_FOUR), sizeof this->saveStruct.MAX_SK_PHASE_FOUR);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_FIVE), sizeof this->saveStruct.MAX_SK_PHASE_FIVE);
        fs.write((char*)(&this->saveStruct.MAX_SK_PHASE_SIX), sizeof this->saveStruct.MAX_SK_PHASE_SIX);
        fs.write((char*)(&this->saveStruct.MAX_SK_UPWARP_SOLUTIONS), sizeof this->saveStruct.MAX_SK_UPWARP_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_SPEED_SOLUTIONS), sizeof this->saveStruct.MAX_SPEED_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_10K_SOLUTIONS), sizeof this->saveStruct.MAX_10K_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_SLIDE_SOLUTIONS), sizeof this->saveStruct.MAX_SLIDE_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_BD_SOLUTIONS), sizeof this->saveStruct.MAX_BD_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_DOUBLE_10K_SOLUTIONS), sizeof this->saveStruct.MAX_DOUBLE_10K_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_BULLY_PUSH_SOLUTIONS), sizeof this->saveStruct.MAX_BULLY_PUSH_SOLUTIONS);
        fs.write((char*)(&this->saveStruct.MAX_SQUISH_SPOTS), sizeof this->saveStruct.MAX_SQUISH_SPOTS);
        fs.write((char*)(&this->saveStruct.MAX_STRAIN_SETUPS), sizeof this->saveStruct.MAX_STRAIN_SETUPS);
        std::string cudaExecutablePathStr = this->saveStruct.cudaExecutablePath.string();
        outStringSize = cudaExecutablePathStr.size();
        fs.write((char*)(&outStringSize), sizeof outStringSize);
        fs.write((char*)(cudaExecutablePathStr.c_str()), outStringSize);
        std::string syclExecutablePathStr = this->saveStruct.syclExecutablePath.string();
        outStringSize = syclExecutablePathStr.size();
        fs.write((char*)(&outStringSize), sizeof outStringSize);
        fs.write((char*)(syclExecutablePathStr.c_str()), outStringSize);
        fs.write((char*)(&this->saveStruct.gpuModeSelected), sizeof this->saveStruct.gpuModeSelected);
    }

    fs.close();
}

bool FST_GUI::OnInit()
{
    if (is_cuda_available()) {
        get_cuda_devices(this->cudaDeviceList);
    }

    if (is_sycl_available()) {
        get_sycl_devices(this->syclDeviceList);
    }

    if (!askGPUMode()) {
        return false;
    }

    checkExecutable();

	MainFrame* mainFrame = new MainFrame(L"BitFS Final Speed Transfer Brute Forcer Launch App", this);
	mainFrame->Show(true);
	return true;
}

const std::vector<DeviceInfo>& FST_GUI::deviceList() {
    switch (saveStruct.gpuModeSelected) {
        case MODE_CUDA:
            return cudaDeviceList;
        case MODE_SYCL:
            return syclDeviceList;
        default:
            return cudaDeviceList;
    }
}

std::filesystem::path FST_GUI::executablePath() {
    return executablePath(&saveStruct);
}

std::filesystem::path FST_GUI::executablePath(SaveData* blockData) {
    switch (blockData->gpuModeSelected) {
    case MODE_CUDA:
        return blockData->cudaExecutablePath;
    case MODE_SYCL:
        return blockData->syclExecutablePath;
    default:
        return "";
    }
}

bool FST_GUI::askGPUMode() {
    if (saveStruct.gpuModeSelected == MODE_NONE) {
            wxArrayString libCoices = { wxT("CUDA (nVidia GPUs)"), wxT("SYCL (AMD or Intel GPUs/CPUs") };

            wxSingleChoiceDialog findExeDialog = wxSingleChoiceDialog(NULL, "Select your default compute library.\n\nThis can be changed later by going to \"GPU Settings\".", "Select Compute Library", libCoices, NULL, wxCANCEL | wxOK | wxCENTRE);
            int result = findExeDialog.ShowModal();

            if (result == wxID_CANCEL) {
                return false;
            }

            if (findExeDialog.GetSelection() == 0) {
                saveStruct.gpuModeSelected = MODE_CUDA;
            }
            else if (findExeDialog.GetSelection() == 1) {
                saveStruct.gpuModeSelected = MODE_SYCL;

            }

            if (saveStruct.gpuModeSelected == MODE_NONE) {
                return false;
            }

            saveSave();
            
            return true;
    }
    else {
        return true;
    }
}

bool FST_GUI::checkExecutable(SaveData* blockData) {
    if (!std::filesystem::exists(executablePath(blockData))) {
        wxMessageDialog findExeDialog = wxMessageDialog(NULL, "Cannot find FST Brute Forcer executable.\n\nPlease provide a path to the executable file.", "Error", wxCANCEL | wxOK | wxCENTRE);
        int result = findExeDialog.ShowModal();

        if (result == wxID_CANCEL) {
            return false;
        }

        if (!updateExecutableFile(blockData)) {
            return false;
        }

        saveSave();
    }

    return true;
}

bool FST_GUI::checkExecutable() {
    return checkExecutable(&saveStruct);
}

bool FST_GUI::updateExecutableFile(SaveData* blockData) {
    switch (blockData->gpuModeSelected) {
    case MODE_CUDA:
        return findExecutableFile(blockData->cudaExecutablePath);
    case MODE_SYCL:
        return findExecutableFile(blockData->syclExecutablePath);
    default:
        return false;
    }
}

bool FST_GUI::updateExecutableFile() {
    return updateExecutableFile(&saveStruct);
}

bool findExecutableFile(std::filesystem::path& executablePath) {
#ifdef _WIN32
    std::string wildCardStr = "Executable files (*.exe)|*.exe";
#else
    std::string wildCardStr = "";
#endif

    wxFileDialog openFileDialog(NULL, _("Open Executable file"), executablePath.parent_path().c_str(), "", wildCardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    int result = openFileDialog.ShowModal();

    if (result == wxID_CANCEL) {
        return false;
    }

    executablePath = std::filesystem::path(openFileDialog.GetPath().ToStdString());

    return true;
}

wxIMPLEMENT_APP(FST_GUI);
//wxIMPLEMENT_WXWIN_MAIN_CONSOLE;
