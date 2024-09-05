#include <sstream>
#include <iomanip>
#include <fstream>
#include <wx/wx.h>
#include "FST-GUI.hpp"
#include "utils.hpp"
#include "SYCLQuery/SYCLQuery.hpp"
#include "CUDAQuery/CUDAQuery.hpp"

#include "FSTLogo.xpm"

FST_GUI::FST_GUI() : blockQueue(this)
{
    loadSave();
}

FST_GUI::~FST_GUI()
{
    saveSave();
}

bool FST_GUI::readCheckpoint(BlockData* checkpointBlock, std::filesystem::path checkpointPath) {
    const int skipBytes = 28;

    std::ifstream fs(checkpointPath, std::ios::out | std::ios::binary);

    if (!fs.is_open()) return false;
    fs.seekg(skipBytes);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->xMin), sizeof checkpointBlock->xMin);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->xMax), sizeof checkpointBlock->xMax);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->yMin), sizeof checkpointBlock->yMin);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->yMax), sizeof checkpointBlock->yMax);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->zMin), sizeof checkpointBlock->zMin);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->zMax), sizeof checkpointBlock->zMax);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->xSamples), sizeof checkpointBlock->xSamples);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->ySamples), sizeof checkpointBlock->ySamples);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->zSamples), sizeof checkpointBlock->zSamples);
    if (!fs.good()) return false;
    fs.read((char*)(&checkpointBlock->zModeOption), sizeof(bool));
    if (!fs.good()) return false;

    bool quadMode;
    fs.read((char*)(&quadMode), sizeof quadMode);
    if (!fs.good()) return false;
    if (quadMode) return false;

    float platformX;
    fs.read((char*)(&platformX), sizeof platformX);
    if (!fs.good()) return false;
    if (platformX == -1945.0f) {
        checkpointBlock->platformOption = 0;
    } else if (platformX == -2866.0f) {
        checkpointBlock->platformOption = 1;
    } else {
        return false;
    }

    float platformY;
    fs.read((char*)(&platformY), sizeof platformY);
    if (!fs.good()) return false;
    if (platformY != -3225.0f) return false;

    float platformZ;
    fs.read((char*)(&platformZ), sizeof platformZ);
    if (!fs.good()) return false;
    if (platformZ != -715.0f) return false;

    return true;
}

bool FST_GUI::loadToBlockStruct(std::ifstream& fs, BlockData* save) {
    fs.read((char*)(&save->platformOption), sizeof save->platformOption);
    if (!fs.good()) return false;
    fs.read((char*)(&save->zModeOption), sizeof save->zModeOption);
    if (!fs.good()) return false;
    fs.read((char*)(&save->xMin), sizeof save->xMin);
    if (!fs.good()) return false;
    fs.read((char*)(&save->xMax), sizeof save->xMax);
    if (!fs.good()) return false;
    fs.read((char*)(&save->yMin), sizeof save->yMin);
    if (!fs.good()) return false;
    fs.read((char*)(&save->yMax), sizeof save->yMax);
    if (!fs.good()) return false;
    fs.read((char*)(&save->zMin), sizeof save->zMin);
    if (!fs.good()) return false;
    fs.read((char*)(&save->zMax), sizeof save->zMax);
    if (!fs.good()) return false;
    fs.read((char*)(&save->xSamples), sizeof save->xSamples);
    if (!fs.good()) return false;
    fs.read((char*)(&save->ySamples), sizeof save->ySamples);
    if (!fs.good()) return false;
    fs.read((char*)(&save->zSamples), sizeof save->zSamples);
    if (!fs.good()) return false;

    return true;
}

void FST_GUI::loadSave() {
    if (!std::filesystem::exists(this->saveFile)) {
        saveSave();
    }
    else {
        std::ifstream fs(this->saveFile, std::ios::in | std::ios::binary);

        if (fs.is_open()) {
            int saveVersion;
            fs.read((char*)(&saveVersion), sizeof saveVersion);

            bool readOK = saveVersion == this->saveStruct.version;

            if (readOK) {
                readOK &= loadToBlockStruct(fs, &this->saveStruct.blockData);

                int outStringSize;
                fs.read((char*)(&outStringSize), sizeof outStringSize);
                readOK &= fs.good();
                std::string outputDirectoryStr = "";
                outputDirectoryStr.resize(outStringSize);
                fs.read((char*)(outputDirectoryStr.c_str()), outStringSize);
                readOK &= fs.good();
                this->saveStruct.outputDirectory = std::filesystem::path(outputDirectoryStr);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.gpuDeviceID), sizeof this->saveStruct.gpuDeviceID);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.maxThreads), sizeof this->saveStruct.maxThreads);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_UPWARP_SOLUTIONS), sizeof this->saveStruct.MAX_UPWARP_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_PLAT_SOLUTIONS), sizeof this->saveStruct.MAX_PLAT_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_ONE), sizeof this->saveStruct.MAX_UPWARP_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_A), sizeof this->saveStruct.MAX_SK_PHASE_TWO_A);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_B), sizeof this->saveStruct.MAX_SK_PHASE_TWO_B);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_C), sizeof this->saveStruct.MAX_SK_PHASE_TWO_C);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_TWO_D), sizeof this->saveStruct.MAX_SK_PHASE_TWO_D);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_THREE), sizeof this->saveStruct.MAX_SK_PHASE_THREE);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_FOUR), sizeof this->saveStruct.MAX_SK_PHASE_FOUR);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_FIVE), sizeof this->saveStruct.MAX_SK_PHASE_FIVE);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_PHASE_SIX), sizeof this->saveStruct.MAX_SK_PHASE_SIX);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SK_UPWARP_SOLUTIONS), sizeof this->saveStruct.MAX_SK_UPWARP_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SPEED_SOLUTIONS), sizeof this->saveStruct.MAX_SPEED_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_10K_SOLUTIONS), sizeof this->saveStruct.MAX_10K_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SLIDE_SOLUTIONS), sizeof this->saveStruct.MAX_SLIDE_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_BD_SOLUTIONS), sizeof this->saveStruct.MAX_BD_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_DOUBLE_10K_SOLUTIONS), sizeof this->saveStruct.MAX_DOUBLE_10K_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_BULLY_PUSH_SOLUTIONS), sizeof this->saveStruct.MAX_BULLY_PUSH_SOLUTIONS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_SQUISH_SPOTS), sizeof this->saveStruct.MAX_SQUISH_SPOTS);
                readOK &= fs.good();
                fs.read((char*)(&this->saveStruct.MAX_STRAIN_SETUPS), sizeof this->saveStruct.MAX_STRAIN_SETUPS);
                readOK &= fs.good();
                fs.read((char*)(&outStringSize), sizeof outStringSize);
                readOK &= fs.good();
                std::string cudaExecutablePathStr = "";
                cudaExecutablePathStr.resize(outStringSize);
                fs.read((char*)(cudaExecutablePathStr.c_str()), outStringSize);
                readOK &= fs.good();
                this->saveStruct.cudaExecutablePath = std::filesystem::path(cudaExecutablePathStr);
                fs.read((char*)(&outStringSize), sizeof outStringSize);
                readOK &= fs.good();
                std::string syclExecutablePathStr = "";
                syclExecutablePathStr.resize(outStringSize);
                fs.read((char*)(syclExecutablePathStr.c_str()), outStringSize);
                readOK &= fs.good();
                this->saveStruct.syclExecutablePath = std::filesystem::path(syclExecutablePathStr);
                fs.read((char*)(&this->saveStruct.gpuModeSelected), sizeof this->saveStruct.gpuModeSelected);
                readOK &= fs.good();

                int queueLength;
                fs.read((char*)(&queueLength), sizeof queueLength);
                readOK &= fs.good();

                for (int i = 0; i < queueLength && readOK; i++) {
                    BlockData queueBlock;
                    readOK &= loadToBlockStruct(fs, &queueBlock);
                    this->blockQueue.addBlockToQueue(queueBlock);
                }
            }

            fs.close();

            if (!readOK) {
                this->saveStruct = SaveData();
                this->blockQueue.clearQueue(true);
                saveSave();
            }
        }
    }
}

void FST_GUI::saveFromBlockStruct(std::ofstream& fs, BlockData* save) {
    fs.write((char*)(&save->platformOption), sizeof save->platformOption);
    fs.write((char*)(&save->zModeOption), sizeof save->zModeOption);
    fs.write((char*)(&save->xMin), sizeof save->xMin);
    fs.write((char*)(&save->xMax), sizeof save->xMax);
    fs.write((char*)(&save->yMin), sizeof save->yMin);
    fs.write((char*)(&save->yMax), sizeof save->yMax);
    fs.write((char*)(&save->zMin), sizeof save->zMin);
    fs.write((char*)(&save->zMax), sizeof save->zMax);
    fs.write((char*)(&save->xSamples), sizeof save->xSamples);
    fs.write((char*)(&save->ySamples), sizeof save->ySamples);
    fs.write((char*)(&save->zSamples), sizeof save->zSamples);
}

void FST_GUI::saveSave() {
    std::ofstream fs(this->saveFile, std::ios::out | std::ios::binary);

    if (fs.is_open()) {
        fs.write((char*)(&this->saveStruct.version), sizeof this->saveStruct.version);

        saveFromBlockStruct(fs, &this->saveStruct.blockData);

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

        int queueLength = blockQueue.queueLength();
        fs.write((char*)(&queueLength), sizeof queueLength);

        std::list<BlockData>::iterator iter = blockQueue.queueBegin();

        for (int i = 0; i < queueLength; i++) {
            saveFromBlockStruct(fs, &(*iter));

            iter++;
        }

        fs.close();
    }
}

bool FST_GUI::OnInit()
{
    icon = wxIcon(FSTLogo_xpm);

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

	mainFrame = new MainFrame(L"BitFS Final Speed Transfer Brute Forcer Launch App", this);
    mainFrame->SetIcon(icon);
	mainFrame->Show(true);
	return true;
}

int FST_GUI::FilterEvent(wxEvent& event)
{
    if ((event.GetEventType() == wxEVT_KEY_DOWN) && (((wxKeyEvent&)event).ControlDown()) && (((wxKeyEvent&)event).GetKeyCode() == 'V') && mainFrame && mainFrame->IsActive())
    {
        if (mainFrame->PasteFromClipboard()) {
            return true;
        }
        else {
            return -1;
        }
    }

    return -1;
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
    switch (saveStruct.gpuModeSelected) {
    case MODE_CUDA:
        return saveStruct.cudaExecutablePath;
    case MODE_SYCL:
        return saveStruct.syclExecutablePath;
    default:
        return "";
    }
}

bool FST_GUI::askGPUMode() {
    if (saveStruct.gpuModeSelected == MODE_NONE) {
            wxArrayString libCoices = { wxT("CUDA (nVidia GPUs)"), wxT("SYCL (AMD or Intel GPUs/CPUs") };

            wxSingleChoiceDialog gpuChoiceDialog = wxSingleChoiceDialog(NULL, "Select your default compute library.\n\nThis can be changed later by going to \"GPU Settings\".", "Select Compute Library", libCoices, NULL, wxCANCEL | wxOK | wxCENTRE);

            gpuChoiceDialog.SetIcon(icon);
            int result = gpuChoiceDialog.ShowModal();

            if (result == wxID_CANCEL) {
                return false;
            }

            if (gpuChoiceDialog.GetSelection() == 0) {
                saveStruct.gpuModeSelected = MODE_CUDA;
            }
            else if (gpuChoiceDialog.GetSelection() == 1) {
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

bool FST_GUI::checkExecutable() {
    if (!std::filesystem::exists(executablePath())) {
        wxMessageDialog findExeDialog = wxMessageDialog(NULL, "Cannot find FST Brute Forcer executable.\n\nPlease provide a path to the executable file.", "Error", wxCANCEL | wxOK | wxCENTRE);
        findExeDialog.SetIcon(icon);
        int result = findExeDialog.ShowModal();

        if (result == wxID_CANCEL) {
            return false;
        }

        if (!updateExecutableFile()) {
            return false;
        }

        saveSave();
    }

    return true;
}

bool FST_GUI::updateExecutableFile() {
    switch (saveStruct.gpuModeSelected) {
    case MODE_CUDA:
        return findExecutableFile(saveStruct.cudaExecutablePath);
    case MODE_SYCL:
        return findExecutableFile(saveStruct.syclExecutablePath);
    default:
        return false;
    }
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
