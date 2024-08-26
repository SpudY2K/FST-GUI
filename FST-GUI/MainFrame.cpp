#include "MainFrame.hpp"
#include "GPUFrame.hpp"
#include "Logging.hpp"
#include "utils.hpp"
#include <wx/valnum.h>
#include <wx/statline.h>
#include <filesystem>
#include <sstream>
#include <fstream>

wxDEFINE_EVENT(wxEVT_RUNTHREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_RUNTHREAD_UPDATE, wxThreadEvent);

void MainFrame::UpdateExePath() {
    exeFileText->ChangeValue(fst_gui->executablePath().string());
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    if (runThread) {
        runThread->terminate_subprocess();
    }
    
    Destroy();
}

void MainFrame::OnThreadCompletion(wxThreadEvent& event)
{
    runThread = nullptr;

    if (runCancelled) {
        if (fst_gui->blockQueue.queueLength() == 1 && removeBlockOnCancel) {
            fst_gui->blockQueue.removeBlockFromQueue(0);
            UpdateQueueList(this->queueList->GetSelection() - 1);
            fst_gui->saveSave();
        }

        statusBar->SetStatusText("");
        runButton->SetLabelText("Run Brute Forcer");

        runCancelled = false;
    }
    else {
        fst_gui->blockQueue.removeBlockFromQueue(0);
        UpdateQueueList(this->queueList->GetSelection() - 1);
        fst_gui->saveSave();

        if (fst_gui->blockQueue.queueEmpty()) {
            statusBar->SetStatusText("");
            runButton->SetLabelText("Run Brute Forcer");
        }
        else {
            this->RunNextBlock();
        }
    }
}

void MainFrame::OnThreadUpdate(wxThreadEvent& event)
{
    outputBox->AppendText(event.GetString());

    if (outputBox->GetNumberOfLines() > maxOutputLines) {
        long start = outputBox->XYToPosition(0, outputBox->GetNumberOfLines() - maxOutputLines);
        outputBox->Remove(0, start);
    }
}

void MainFrame::SetupArgs(SaveData* blockParams, std::vector<std::string>& args) {
    const int filename_float_precision = 6;

    std::string outFileName = "fstSearch_" + float2string(blockParams->xMin, filename_float_precision) + "_" + float2string(blockParams->xMax, filename_float_precision) + "_"
        + float2string(blockParams->yMin, filename_float_precision) + "_" + float2string(blockParams->yMax, filename_float_precision) + "_" + float2string(blockParams->zMin, filename_float_precision)
        + "_" + float2string(blockParams->zMax, filename_float_precision) + "_" + std::to_string(blockParams->xSamples) + "_" + std::to_string(blockParams->ySamples) + "_" + std::to_string(blockParams->zSamples) + "_" + platformX->GetString(blockParams->platformOption).ToStdString() + ".csv";

    std::filesystem::path outFilePath = blockParams->outputDirectory;
    outFilePath.append(outFileName);

    int i = 0;

    while (std::filesystem::exists(outFilePath)) {
        std::string newOutFileName = outFileName.substr(0, outFileName.size() - 4) + "_" + std::to_string(i) + ".csv";
        outFilePath = blockParams->outputDirectory;
        outFilePath.append(newOutFileName);
        i++;
    }

    std::filesystem::path logFilePath = outFilePath;
    logFilePath.replace_extension(".log");

    args.emplace_back("-nx");
    args.emplace_back(float2string(blockParams->xMin, float_precision));
    args.emplace_back(float2string(blockParams->xMax, float_precision));
    args.emplace_back(std::to_string(blockParams->xSamples));
    args.emplace_back("-ny");
    args.emplace_back(float2string(blockParams->yMin, float_precision));
    args.emplace_back(float2string(blockParams->yMax, float_precision));
    args.emplace_back(std::to_string(blockParams->ySamples));
    args.emplace_back("-nxz");
    args.emplace_back(float2string(blockParams->zMin, float_precision));
    args.emplace_back(float2string(blockParams->zMax, float_precision));
    args.emplace_back(std::to_string(blockParams->zSamples));
    if (blockParams->zModeOption == 1) {
        args.emplace_back("-nz");
    }
    args.emplace_back("-p");
    args.emplace_back(platformX->GetString(blockParams->platformOption).ToStdString());
    args.emplace_back(platformY->GetLineText(0));
    args.emplace_back(platformZ->GetLineText(0));
    args.emplace_back("-l");
    args.emplace_back(logFilePath.string());
    args.emplace_back("-o");
    args.emplace_back(outFilePath.string());
    args.emplace_back("-m");
    args.emplace_back("1");
    args.emplace_back("-d");
    args.emplace_back(std::to_string(blockParams->gpuDeviceID));
    args.emplace_back("-t");
    args.emplace_back(std::to_string(blockParams->maxThreads));
    args.emplace_back("-lsk1");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_ONE));
    args.emplace_back("-lsk2a");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_TWO_A));
    args.emplace_back("-lsk2b");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_TWO_B));
    args.emplace_back("-lsk2c");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_TWO_C));
    args.emplace_back("-lsk2d");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_TWO_D));
    args.emplace_back("-lsk3");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_THREE));
    args.emplace_back("-lsk4");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_FOUR));
    args.emplace_back("-lsk5");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_FIVE));
    args.emplace_back("-lsk6");
    args.emplace_back(std::to_string(blockParams->MAX_SK_PHASE_SIX));
    args.emplace_back("-lp");
    args.emplace_back(std::to_string(blockParams->MAX_PLAT_SOLUTIONS));
    args.emplace_back("-lu");
    args.emplace_back(std::to_string(blockParams->MAX_UPWARP_SOLUTIONS));
    args.emplace_back("-lsku");
    args.emplace_back(std::to_string(blockParams->MAX_SK_UPWARP_SOLUTIONS));
    args.emplace_back("-ls");
    args.emplace_back(std::to_string(blockParams->MAX_SPEED_SOLUTIONS));
    args.emplace_back("-l10k");
    args.emplace_back(std::to_string(blockParams->MAX_10K_SOLUTIONS));
    args.emplace_back("-lbd");
    args.emplace_back(std::to_string(blockParams->MAX_BD_SOLUTIONS));
    args.emplace_back("-ld10k");
    args.emplace_back(std::to_string(blockParams->MAX_DOUBLE_10K_SOLUTIONS));
    args.emplace_back("-lbp");
    args.emplace_back(std::to_string(blockParams->MAX_BULLY_PUSH_SOLUTIONS));
    args.emplace_back("-lsq");
    args.emplace_back(std::to_string(blockParams->MAX_SQUISH_SPOTS));
    args.emplace_back("-lst");
    args.emplace_back(std::to_string(blockParams->MAX_STRAIN_SETUPS));
    args.emplace_back("-v");
    args.emplace_back("-b");
}

void MainFrame::RunNextBlock() {
    SaveData blockData = fst_gui->blockQueue.getNextBlockInQueue();

    if (!fst_gui->checkExecutable(&blockData)) {
        outputBox->ChangeValue("Error: Could not find executable file.");
        return;
    }

    outputBox->ChangeValue("Starting Brute Forcer...");
    outputBox->AppendText("\n\n");

    std::vector<std::string> args;
    SetupArgs(&blockData, args);

    switch (fst_gui->saveStruct.gpuModeSelected) {
    case MODE_CUDA:
    case MODE_SYCL:
        runThread = new RunThread(this, fst_gui->executablePath(&blockData).string(), args);
        break;
    default:
        return;
    }

    if (runThread->Run() != wxTHREAD_NO_ERROR)
    {
        outputBox->ChangeValue("Error: Failed to start main process thread.");
        delete runThread;
        runThread = nullptr;
        statusBar->SetStatusText("");
        runButton->SetLabelText("Run Brute Forcer");
        return;
    }

    statusBar->SetStatusText(" Brute Forcer Running");
    runButton->SetLabelText("Stop Brute Forcer");
    UpdateQueueList();
 }

void MainFrame::UpdateQueueList() {
    int selectedIdx = queueList->GetSelection();

    UpdateQueueList(selectedIdx);
}

void MainFrame::UpdateQueueList(int selectedIdx) {
    wxArrayString queueEntries;
    fst_gui->blockQueue.getQueueStrings(queueEntries, runThread != nullptr);
    queueList->Clear();

    if (!queueEntries.IsEmpty()) {
        queueList->InsertItems(queueEntries, 0);
    }

    selectedIdx = std::min(selectedIdx, (int)queueList->GetCount() - 1);
    selectedIdx = selectedIdx < 0 ? wxNOT_FOUND : selectedIdx;

    queueList->SetSelection(selectedIdx);
}

void MainFrame::OnClickQueue(wxCommandEvent& event) {
    double scaleFactor = this->GetDPIScaleFactor();
    wxSize size = this->GetSize();

    if (queueVisible) {
        size.SetWidth(scaleFactor * mfWidthNoQueue);
        showQueueButton->SetLabelText("Show Queue");
    }
    else {
        size.SetWidth(scaleFactor * mfWidthQueue);
        showQueueButton->SetLabelText("Hide Queue");
    }

    queueVisible = !queueVisible;
    this->SetSize(size);
}

void MainFrame::OnClickAddQueue(wxCommandEvent& event) {
    if (AddBlockToQueue()) {
        addBlockOnRun = false;
        removeBlockOnCancel = false;
    }
}

void MainFrame::OnClickRemoveQueue(wxCommandEvent& event) {
    if (queueList->GetSelection() != -1) {
        if (!runThread || queueList->GetSelection() != 0) {
            if (queueList->GetSelection() == fst_gui->blockQueue.queueLength() - 1) {
                removeBlockOnCancel = false;
            }

            fst_gui->blockQueue.removeBlockFromQueue(queueList->GetSelection());
            UpdateQueueList(queueList->GetSelection() - 1);
            fst_gui->saveSave();
        }
    }
}

void MainFrame::OnClickClearQueue(wxCommandEvent& event) {
    fst_gui->blockQueue.clearQueue(!runThread);
    UpdateQueueList();
    fst_gui->saveSave();
}



void MainFrame::OnClickImportQueue(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open log file", fst_gui->saveStruct.outputDirectory.string(), "", "Log files (*.log)|*.log", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    int result = openFileDialog.ShowModal();

    if (result == wxID_CANCEL) {
        return;
    }

    std::filesystem::path logFilePath = std::filesystem::path(openFileDialog.GetPath().ToStdString());

    std::ifstream logFile(logFilePath);

    if (!logFile.is_open()) {
        wxMessageDialog createFolderDialog = wxMessageDialog(this, "Could not open the chosen log file.", "Error", wxOK | wxCENTRE);
        createFolderDialog.ShowModal();
    }
    else {
        int addedBlocks = 0;
        std::string line;
        float normal[3];
        float position[3];

        SaveData blockData = fst_gui->saveStruct;
        unsigned int blockStatus = 0;
        int samplesSearched = 0;
        float lastSampleSearched = 0.0f;
        bool searchComplete = false;

        while (std::getline(logFile, line)) {
            char l;
            std::string time;

            if (CheckLogLine(line, time, l)) {
                if (l == LOG_WARNING) {
                    if (GetNormalFromLogLine(line, normal, position)) {
                        if ((position[0] == -1945 || position[0] == -2866) && position[1] == -3225 && position[2] == -715) {
                            SaveData newBlock = fst_gui->saveStruct;

                            newBlock.xMin = normal[0];
                            newBlock.xMax = normal[0];
                            newBlock.yMin = normal[1];
                            newBlock.yMax = normal[1];
                            newBlock.zMin = normal[2];
                            newBlock.zMax = normal[2];
                            newBlock.xSamples = 1;
                            newBlock.ySamples = 1;
                            newBlock.zSamples = 1;

                            newBlock.zModeOption = 1;
                            newBlock.platformOption = (position[0] == -1945 ? 0 : 1);

                            if (fst_gui->blockQueue.addBlockToQueue(newBlock)) {
                                addedBlocks++;
                            }
                        }
                    }
                }
                else if (l == LOG_INFO) {
                    if (!GetOptionFromLine(line, blockData, blockStatus) && (blockStatus == 0x1FF || blockStatus == 0x177)) {
                        CheckLineForSearchInfo(line, samplesSearched, lastSampleSearched);
                        searchComplete = CheckLineForCompletion(line);
                    }
                }
            }
        }

        if (blockStatus == 0x1FF || blockStatus == 0x177 && samplesSearched > 0 && !searchComplete) {
            blockData.zMin = lastSampleSearched;
            blockData.zSamples = blockData.zSamples - samplesSearched + 1;

            if (fst_gui->blockQueue.addBlockToQueue(blockData)) {
                addedBlocks++;
            }
        }

        if (addedBlocks == 0) {
            wxMessageDialog createFolderDialog = wxMessageDialog(this, "Could not find any new normal blocks in log file.", "Error", wxOK | wxCENTRE);
            createFolderDialog.ShowModal();
        }
        else {
            UpdateQueueList(fst_gui->blockQueue.queueLength() - 1);
            fst_gui->saveSave();
            addBlockOnRun = false;
            removeBlockOnCancel = false;

            std::string outText = "Imported " + std::to_string(addedBlocks) + " normal block" + (addedBlocks > 1 ? "s" : "") + " from log file.";
            wxMessageDialog createFolderDialog = wxMessageDialog(this, outText, "Import Successful", wxOK | wxCENTRE);
            createFolderDialog.ShowModal();
        }
    }
}

bool MainFrame::AddBlockToQueue() {
    std::filesystem::path outputPath = fst_gui->saveStruct.outputDirectory;

    if (!std::filesystem::exists(outputPath)) {
        wxMessageDialog createFolderDialog = wxMessageDialog(this, "The provided output directory does not exist.\n\nWould you like to create it?", "Create Directory?", wxCANCEL | wxYES_NO | wxCENTRE);
        int result = createFolderDialog.ShowModal();

        if (result == wxID_CANCEL) {
            return false;
        }
        else if (result == wxID_YES) {
            bool success = false;

            try {
                success = create_directories(outputPath);
            }
            catch (std::filesystem::filesystem_error) {}

            if (!success) {
                wxMessageDialog createFolderFailDialog = wxMessageDialog(this, "Error: Could not create directory at destination.\n\nNo output files will be saved. Continue anyway?", "Error", wxCANCEL | wxOK | wxCENTRE);
                result = createFolderFailDialog.ShowModal();

                if (result == wxID_CANCEL) {
                    return false;
                }
            }
        }
    }

    bool success = fst_gui->blockQueue.addBlockToQueue();

    if (success) {
        UpdateQueueList(fst_gui->blockQueue.queueLength() - 1);
        fst_gui->saveSave();
    }

    return success;
}

void MainFrame::OnClickRun(wxCommandEvent& event) {
    if (!runThread) {
        fst_gui->saveSave();

        if (addBlockOnRun) {
            this->AddBlockToQueue();
            removeBlockOnCancel = true;
        }

        this->RunNextBlock();
    }
    else {
        runCancelled = true;
        runThread->terminate_subprocess();
        outputBox->AppendText("\nBrute Forcer execution terminated by user.");
    }
}

void MainFrame::OnClickGPU(wxCommandEvent& event) {
    GPUFrame* gpuFrame = new GPUFrame(this, "GPU Settings", fst_gui);
    gpuFrame->ShowModal();
}

void MainFrame::OnClickBrowse(wxCommandEvent& event) {
    switch (event.GetId()) {
        case ID_EXE_BROWSE_BUTTON:
            if (fst_gui->updateExecutableFile()) {
                exeFileText->ChangeValue(fst_gui->executablePath().string());
                addBlockOnRun = true;
            }

            break;
        case ID_OUT_BROWSE_BUTTON:
            wxDirDialog dirDialog(this, "Choose output directory", outFileText->GetLineText(0), wxDD_DEFAULT_STYLE);

            if (dirDialog.ShowModal() == wxID_CANCEL) {
                return;
            }

            fst_gui->saveStruct.outputDirectory = std::filesystem::path(dirDialog.GetPath().ToStdString());
            outFileText->ChangeValue(dirDialog.GetPath());
            addBlockOnRun = true;
            break;
    }

    fst_gui->saveSave();
}

void MainFrame::OnComboChange(wxCommandEvent& event) {
    wxComboBox* comboBox = (wxComboBox*)event.GetEventObject();

    switch (event.GetId()) {
    case ID_NORM_Z_MODE:
        fst_gui->saveStruct.zModeOption = comboBox->GetSelection();
        fst_gui->saveSave();
        addBlockOnRun = true;
        break;
    case ID_PLATFORM_X:
        fst_gui->saveStruct.platformOption = comboBox->GetSelection();
        fst_gui->saveSave();
        addBlockOnRun = true;
        break;
    }
}

void MainFrame::OnTextChange(wxCommandEvent& event) {
    wxTextCtrl* textBox = (wxTextCtrl*)event.GetEventObject();
    std::string s = textBox->GetLineText(0).ToStdString();

    switch (event.GetId()) {
    case ID_NORM_MIN_X:
        try {
            fst_gui->saveStruct.xMin = std::stof(s);
            if (fst_gui->saveStruct.xMin == fst_gui->saveStruct.xMax && fst_gui->saveStruct.xSamples == 1) {
                gapsX->ChangeValue("0");
            } else {
                float xGap = (fst_gui->saveStruct.xMax == fst_gui->saveStruct.xMin) ? 0.0 : fabsf(fst_gui->saveStruct.xMax - fst_gui->saveStruct.xMin) / (float)(fst_gui->saveStruct.xSamples - 1);
                gapsX->ChangeValue(float2string(xGap, float_precision));
            }

            addBlockOnRun = true;
	        fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_MAX_X:
        try {
            fst_gui->saveStruct.xMax = std::stof(s);
            if (fst_gui->saveStruct.xMin == fst_gui->saveStruct.xMax || fst_gui->saveStruct.xSamples == 1) {
                gapsX->ChangeValue("0");
            } else {
                float xGap = (fst_gui->saveStruct.xMax == fst_gui->saveStruct.xMin) ? 0.0 : fabsf(fst_gui->saveStruct.xMax - fst_gui->saveStruct.xMin) / (float)(fst_gui->saveStruct.xSamples - 1);
                gapsX->ChangeValue(float2string(xGap, float_precision));
            }

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_MIN_Y:
        try {
            fst_gui->saveStruct.yMin = std::stof(s);
            if (fst_gui->saveStruct.yMin == fst_gui->saveStruct.yMax || fst_gui->saveStruct.ySamples == 1) {
                gapsY->ChangeValue("0");
            } else {
                float yGap = (fst_gui->saveStruct.yMax == fst_gui->saveStruct.yMin) ? 0.0 : fabsf(fst_gui->saveStruct.yMax - fst_gui->saveStruct.yMin) / (float)(fst_gui->saveStruct.ySamples - 1);
                gapsY->ChangeValue(float2string(yGap, float_precision));
            }

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_MAX_Y:
        try {
            fst_gui->saveStruct.yMax = std::stof(s);
            if (fst_gui->saveStruct.yMin == fst_gui->saveStruct.yMax || fst_gui->saveStruct.ySamples == 1) {
                gapsY->ChangeValue("0");
            } else {
                float yGap = (fst_gui->saveStruct.yMax == fst_gui->saveStruct.yMin) ? 0.0 : fabsf(fst_gui->saveStruct.yMax - fst_gui->saveStruct.yMin) / (float)(fst_gui->saveStruct.ySamples - 1);
                gapsY->ChangeValue(float2string(yGap, float_precision));
            }

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_MIN_Z:
        try {
            fst_gui->saveStruct.zMin = std::stof(s);
            if (fst_gui->saveStruct.zMin == fst_gui->saveStruct.zMax || fst_gui->saveStruct.zSamples == 1) {
                gapsZ->ChangeValue("0");
            } else {
                float zGap = (fst_gui->saveStruct.zMax == fst_gui->saveStruct.zMin) ? 0.0 : fabsf(fst_gui->saveStruct.zMax - fst_gui->saveStruct.zMin) / (float)(fst_gui->saveStruct.zSamples - 1);
                gapsZ->ChangeValue(float2string(zGap, float_precision));
            }

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_MAX_Z:
        try {
            fst_gui->saveStruct.zMax = std::stof(s);
            if (fst_gui->saveStruct.zMin == fst_gui->saveStruct.zMax && fst_gui->saveStruct.zSamples == 1) {
                gapsZ->ChangeValue("0");
            } else {
                float zGap = (fst_gui->saveStruct.zMax == fst_gui->saveStruct.zMin) ? 0.0 : fabsf(fst_gui->saveStruct.zMax - fst_gui->saveStruct.zMin) / (float)(fst_gui->saveStruct.zSamples - 1);
                gapsZ->ChangeValue(float2string(zGap, float_precision));
            }

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_SAMPLES_X:
        try {
            fst_gui->saveStruct.xSamples = std::stoi(s);
            if (fst_gui->saveStruct.xSamples == 1 && fst_gui->saveStruct.xMax != fst_gui->saveStruct.xMin) {
                fst_gui->saveStruct.xMax = fst_gui->saveStruct.xMin;
                maxBoxX->ChangeValue(float2string(fst_gui->saveStruct.xMax, float_precision));
            }

            float xGap = (fst_gui->saveStruct.xMax == fst_gui->saveStruct.xMin) ? 0.0 : fabsf(fst_gui->saveStruct.xMax - fst_gui->saveStruct.xMin) / (float)(fst_gui->saveStruct.xSamples - 1);
            gapsX->ChangeValue(float2string(xGap, float_precision));

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_SAMPLES_Y:
        try {
            fst_gui->saveStruct.ySamples = std::stoi(s);
            if (fst_gui->saveStruct.ySamples == 1 && fst_gui->saveStruct.yMax != fst_gui->saveStruct.yMin) {
                fst_gui->saveStruct.yMax = fst_gui->saveStruct.yMin;
                maxBoxY->ChangeValue(float2string(fst_gui->saveStruct.yMax, float_precision));
            }

            float yGap = (fst_gui->saveStruct.yMax == fst_gui->saveStruct.yMin) ? 0.0 : fabsf(fst_gui->saveStruct.yMax - fst_gui->saveStruct.yMin) / (float)(fst_gui->saveStruct.ySamples - 1);
            gapsY->ChangeValue(float2string(yGap, float_precision));

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_NORM_SAMPLES_Z:
        try {
            fst_gui->saveStruct.zSamples = std::stoi(s);
            if (fst_gui->saveStruct.zSamples == 1 && fst_gui->saveStruct.zMax != fst_gui->saveStruct.zMin) {
                fst_gui->saveStruct.zMax = fst_gui->saveStruct.zMin;
                maxBoxZ->ChangeValue(float2string(fst_gui->saveStruct.zMax, float_precision));
            }

            float zGap = (fst_gui->saveStruct.zMax == fst_gui->saveStruct.zMin) ? 0.0 : fabsf(fst_gui->saveStruct.zMax - fst_gui->saveStruct.zMin) / (float)(fst_gui->saveStruct.zSamples - 1);
            gapsZ->ChangeValue(float2string(zGap, float_precision));

            addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_OUT_PATH:
        fst_gui->saveStruct.outputDirectory = s;
        addBlockOnRun = true;
        fst_gui->saveSave();
        break;
    }
}

MainFrame::MainFrame(const wxString& title, FST_GUI* f)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(mfWidthNoQueue, mfHeight), wxMINIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    double scaleFactor = this->GetDPIScaleFactor();
    this->SetSize(wxSize(scaleFactor * mfWidthNoQueue, scaleFactor * mfHeight));

    fst_gui = f;

    wxPoint panelPos = { 0, 0 };
    wxSize panelSize = { (int)std::round(scaleFactor * (mfWidthNoQueue - 15)), (int)std::round(scaleFactor * (mfHeight - 60)) };
    wxPanel* panel = new wxPanel(this, -1, panelPos, panelSize);

    wxBoxSizer* mainVBox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* exeFileHBox = new wxBoxSizer(wxHORIZONTAL);
    exeFileLabel = new wxStaticText(panel, wxID_ANY, wxT("Brute Forcer Executable:"));

    wxBoxSizer* exeFileLabelVBox = new wxBoxSizer(wxVERTICAL);
    exeFileLabelVBox->Add(exeFileLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    exeFileHBox->Add(exeFileLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 100));

    wxGridSizer* exeFileGrid = new wxGridSizer(1, 2, (int)std::round(scaleFactor * 4), (int)std::round(scaleFactor * 8));
    exeFileText = new wxTextCtrl(panel, ID_EXE_PATH, fst_gui->executablePath().string(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    exeFileGrid->Add(exeFileText, 0, wxALL | wxALIGN_CENTER, 0);
    exeBrowseButton = new wxButton(panel, ID_EXE_BROWSE_BUTTON, wxT("Browse..."));
    exeFileGrid->Add(exeBrowseButton, 0, wxLEFT | wxALIGN_CENTER, (int)std::round(scaleFactor * 100));

    Connect(ID_EXE_BROWSE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickBrowse));

    wxSize fileTextSize = exeFileText->GetSize();
    fileTextSize.SetWidth((int)std::round(scaleFactor * 340));
    exeFileText->SetMinSize(fileTextSize);

    exeFileHBox->Add(exeFileGrid, 0, wxRIGHT, (int)std::round(scaleFactor * 8));

    mainVBox->Add(exeFileHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 20));

    wxBoxSizer* normHBox = new wxBoxSizer(wxHORIZONTAL);
    normLabel = new wxStaticText(panel, wxID_ANY, wxT("Platform Normal Search Range:"));

    wxBoxSizer* normLabelVBox = new wxBoxSizer(wxVERTICAL);
    normLabelVBox->Add(normLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    normHBox->Add(normLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 8));

    wxGridSizer* normGrid = new wxGridSizer(3, 3, (int)std::round(scaleFactor * 4), (int)std::round(scaleFactor * 8));

    labelX = new wxStaticText(panel, wxID_ANY, wxT("X"));
    normGrid->Add(labelX, 0, wxALL | wxALIGN_CENTER, 0);
    labelY = new wxStaticText(panel, wxID_ANY, wxT("Y"));
    normGrid->Add(labelY, 0, wxALL | wxALIGN_CENTER, 0);

    wxArrayString comboZOptions = { wxT("XZ Sum"), wxT("Z") };
    fst_gui->saveStruct.zModeOption = std::max(std::min(fst_gui->saveStruct.zModeOption, 1), 0);

    comboZ = new wxComboBox(panel, ID_NORM_Z_MODE, comboZOptions[fst_gui->saveStruct.zModeOption], wxDefaultPosition, wxDefaultSize, comboZOptions, wxCB_READONLY);
    normGrid->Add(comboZ, 0, wxALL | wxALIGN_CENTER, 0);

    Connect(ID_NORM_Z_MODE, wxEVT_COMBOBOX, wxCommandEventHandler(MainFrame::OnComboChange));

    wxFloatingPointValidator<float> floatVal(float_precision, NULL, wxNUM_VAL_NO_TRAILING_ZEROES);
    wxIntegerValidator<int> intVal(NULL, wxNUM_VAL_DEFAULT);

    floatVal.SetRange(-1.0f, 1.0f);
    intVal.SetMin(1);

    minBoxX = new wxTextCtrl(panel, ID_NORM_MIN_X, float2string(fst_gui->saveStruct.xMin, float_precision), wxDefaultPosition, wxDefaultSize, 0, floatVal);
    normGrid->Add(minBoxX, 0, wxALL | wxALIGN_CENTER, 0);
    minBoxY = new wxTextCtrl(panel, ID_NORM_MIN_Y, float2string(fst_gui->saveStruct.yMin, float_precision), wxDefaultPosition, wxDefaultSize, 0, floatVal);
    normGrid->Add(minBoxY, 0, wxALL | wxALIGN_CENTER, 0);
    minBoxZ = new wxTextCtrl(panel, ID_NORM_MIN_Z, float2string(fst_gui->saveStruct.zMin, float_precision), wxDefaultPosition, wxDefaultSize, 0, floatVal);
    normGrid->Add(minBoxZ, 0, wxALL | wxALIGN_CENTER, 0);

    maxBoxX = new wxTextCtrl(panel, ID_NORM_MAX_X, float2string(fst_gui->saveStruct.xMax, float_precision), wxDefaultPosition, wxDefaultSize, 0, floatVal);
    normGrid->Add(maxBoxX, 0, wxALL | wxALIGN_CENTER, 0);
    maxBoxY = new wxTextCtrl(panel, ID_NORM_MAX_Y, float2string(fst_gui->saveStruct.yMax, float_precision), wxDefaultPosition, wxDefaultSize, 0, floatVal);
    normGrid->Add(maxBoxY, 0, wxALL | wxALIGN_CENTER, 0);
    maxBoxZ = new wxTextCtrl(panel, ID_NORM_MAX_Z, float2string(fst_gui->saveStruct.zMax, float_precision), wxDefaultPosition, wxDefaultSize, 0, floatVal);
    normGrid->Add(maxBoxZ, 0, wxALL | wxALIGN_CENTER, 0);

    Connect(ID_NORM_MIN_X, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_MAX_X, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_MIN_Y, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_MAX_Y, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_MIN_Z, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_MAX_Z, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));

    normHBox->Add(normGrid, (int)std::round(scaleFactor * 1));

    mainVBox->Add(normHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 10));

    wxBoxSizer* samplesHBox = new wxBoxSizer(wxHORIZONTAL);
    samplesLabel = new wxStaticText(panel, wxID_ANY, wxT("Number of Normals to Test:"));

    wxBoxSizer* samplesLabelVBox = new wxBoxSizer(wxVERTICAL);
    samplesLabelVBox->Add(samplesLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    samplesHBox->Add(samplesLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 26));

    wxGridSizer* samplesGrid = new wxGridSizer(2, 3, (int)std::round(scaleFactor * 4), (int)std::round(scaleFactor * 8));

    samplesX = new wxTextCtrl(panel, ID_NORM_SAMPLES_X, std::to_string(fst_gui->saveStruct.xSamples), wxDefaultPosition, wxDefaultSize, 0, intVal);
    samplesGrid->Add(samplesX, 0, wxALL | wxALIGN_CENTER, 0);
    samplesY = new wxTextCtrl(panel, ID_NORM_SAMPLES_Y, std::to_string(fst_gui->saveStruct.ySamples), wxDefaultPosition, wxDefaultSize, 0, intVal);
    samplesGrid->Add(samplesY, 0, wxALL | wxALIGN_CENTER, 0);
    samplesZ = new wxTextCtrl(panel, ID_NORM_SAMPLES_Z, std::to_string(fst_gui->saveStruct.zSamples), wxDefaultPosition, wxDefaultSize, 0, intVal);
    samplesGrid->Add(samplesZ, 0, wxALL | wxALIGN_CENTER, 0);

    float xGap = (fst_gui->saveStruct.xMax == fst_gui->saveStruct.xMin) ? 0.0 : fabsf(fst_gui->saveStruct.xMax - fst_gui->saveStruct.xMin) / (float)(fst_gui->saveStruct.xSamples - 1);
    float yGap = (fst_gui->saveStruct.yMax == fst_gui->saveStruct.yMin) ? 0.0 : fabsf(fst_gui->saveStruct.yMax - fst_gui->saveStruct.yMin) / (float)(fst_gui->saveStruct.ySamples - 1);
    float zGap = (fst_gui->saveStruct.zMax == fst_gui->saveStruct.zMin) ? 0.0 : fabsf(fst_gui->saveStruct.zMax - fst_gui->saveStruct.zMin) / (float)(fst_gui->saveStruct.zSamples - 1);

    gapsX = new wxTextCtrl(panel, wxID_ANY, float2string(xGap, float_precision), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    samplesGrid->Add(gapsX, 0, wxALL | wxALIGN_CENTER, 0);
    gapsY = new wxTextCtrl(panel, wxID_ANY, float2string(yGap, float_precision), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    samplesGrid->Add(gapsY, 0, wxALL | wxALIGN_CENTER, 0);
    gapsZ = new wxTextCtrl(panel, wxID_ANY, float2string(zGap, float_precision), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    samplesGrid->Add(gapsZ, 0, wxALL | wxALIGN_CENTER, 0);

    Connect(ID_NORM_SAMPLES_X, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_SAMPLES_Y, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_NORM_SAMPLES_Z, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));

    samplesHBox->Add(samplesGrid, (int)std::round(scaleFactor * 1));

    mainVBox->Add(samplesHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 10));

    wxBoxSizer* platformHBox = new wxBoxSizer(wxHORIZONTAL);
    platformLabel = new wxStaticText(panel, wxID_ANY,
        wxT("Pyramid Platform Position:"));

    wxBoxSizer* platformLabelVBox = new wxBoxSizer(wxVERTICAL);
    platformLabelVBox->Add(platformLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    platformHBox->Add(platformLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 36));

    wxGridSizer* platformGrid = new wxGridSizer(1, 3, (int)std::round(scaleFactor * 4), (int)std::round(scaleFactor * 24));

    wxArrayString platformXOptions = { wxT("-1945"), wxT("-2866") };
    fst_gui->saveStruct.platformOption = std::max(std::min(fst_gui->saveStruct.platformOption, 1), 0);

    platformX = new wxComboBox(panel, ID_PLATFORM_X, platformXOptions[fst_gui->saveStruct.platformOption], wxDefaultPosition, wxDefaultSize, platformXOptions, wxCB_READONLY);
    platformGrid->Add(platformX, 0, wxALL | wxALIGN_CENTER, 0);
    platformY = new wxTextCtrl(panel, wxID_ANY, "-3225", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    platformGrid->Add(platformY, 0, wxALL | wxALIGN_CENTER, 0);
    platformZ = new wxTextCtrl(panel, wxID_ANY, "-715", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    platformGrid->Add(platformZ, 0, wxALL | wxALIGN_CENTER, 0);
    platformX->SetMinSize(platformY->GetSize());
    platformHBox->Add(platformGrid, 0, wxRIGHT, (int)std::round(scaleFactor * 8));

    Connect(ID_PLATFORM_X, wxEVT_COMBOBOX, wxCommandEventHandler(MainFrame::OnComboChange));

    mainVBox->Add(platformHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 10));

    wxBoxSizer* outFileHBox = new wxBoxSizer(wxHORIZONTAL);
    outFileLabel = new wxStaticText(panel, wxID_ANY, wxT("Output Directory:"));

    wxBoxSizer* outFileLabelVBox = new wxBoxSizer(wxVERTICAL);
    outFileLabelVBox->Add(outFileLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    outFileHBox->Add(outFileLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 110));

    wxGridSizer* outFileGrid = new wxGridSizer(1, 2, (int)std::round(scaleFactor * 4), (int)std::round(scaleFactor * 8));
    outFileText = new wxTextCtrl(panel, ID_OUT_PATH, fst_gui->saveStruct.outputDirectory.string());
    outFileGrid->Add(outFileText, 0, wxALL | wxALIGN_CENTER, 0);
    outBrowseButton = new wxButton(panel, ID_OUT_BROWSE_BUTTON, wxT("Browse..."));
    outFileGrid->Add(outBrowseButton, 0, wxLEFT | wxALIGN_CENTER, (int)std::round(scaleFactor * 110));

    Connect(ID_OUT_PATH, wxEVT_TEXT, wxCommandEventHandler(MainFrame::OnTextChange));
    Connect(ID_OUT_BROWSE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickBrowse));

    fileTextSize = outFileText->GetSize();
    fileTextSize.SetWidth((int)std::round(scaleFactor * 380));
    outFileText->SetMinSize(fileTextSize);

    outFileHBox->Add(outFileGrid, 0, wxRIGHT, (int)std::round(scaleFactor * 8));

    mainVBox->Add(outFileHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 20));

    wxBoxSizer* buttonHBox = new wxBoxSizer(wxHORIZONTAL);
    showQueueButton = new wxButton(panel, ID_QUEUE_BUTTON, wxT("Show Queue"));
    buttonHBox->Add(showQueueButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 5));
    gpuButton = new wxButton(panel, ID_GPU_BUTTON, wxT("GPU Settings"));
    buttonHBox->Add(gpuButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 5));
    runButton = new wxButton(panel, ID_RUN_BUTTON, wxT("Run Brute Forcer"));
    buttonHBox->Add(runButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 5));
    mainVBox->Add(buttonHBox, 0, wxALIGN_RIGHT | wxRIGHT, (int)std::round(scaleFactor * 10));

    Connect(ID_QUEUE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickQueue));
    Connect(ID_GPU_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickGPU));
    Connect(ID_RUN_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickRun));

    Connect(wxID_ANY, wxEVT_RUNTHREAD_UPDATE, wxThreadEventHandler(MainFrame::OnThreadUpdate));
    Connect(wxID_ANY, wxEVT_RUNTHREAD_COMPLETED, wxThreadEventHandler(MainFrame::OnThreadCompletion));

    Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrame::OnClose));

    wxBoxSizer* outputLabelHBox = new wxBoxSizer(wxHORIZONTAL);
    outputLabel = new wxStaticText(panel, wxID_ANY, wxT("Brute Forcer Output:"));

    outputLabelHBox->Add(outputLabel, 0);
    mainVBox->Add(outputLabelHBox, 0, wxLEFT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 10));

    wxBoxSizer* outputHBox = new wxBoxSizer(wxHORIZONTAL);
    outputBox = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE | wxTE_READONLY);

    outputHBox->Add(outputBox, 1, wxEXPAND);
    mainVBox->Add(outputHBox, 1, wxLEFT | wxRIGHT | wxEXPAND, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 25));

    panel->SetSizerAndFit(mainVBox);
    panel->SetSize(panelSize);

    wxPoint dummyPanelPos = { (int)std::round(scaleFactor * (mfWidthNoQueue - 15)), 0 };
    wxSize dummyPanelSize = { (int)std::round(scaleFactor * 15), (int)std::round(scaleFactor * (mfHeight - 60)) };
    wxPanel* dummyPanel = new wxPanel(this, -1, dummyPanelPos, dummyPanelSize);

    wxPoint linePanelPos = { (int)std::round(scaleFactor * mfWidthNoQueue), 0 };
    wxSize linePanelSize = { (int)std::round(scaleFactor * 2), (int)std::round(scaleFactor * (mfHeight - 60)) };
    wxPanel* linePanel = new wxPanel(this, -1, linePanelPos, linePanelSize);
    linePanel->SetBackgroundColour(wxColor(128, 128, 128));

    wxPoint queuePanelPos = { (int)std::round(scaleFactor * (mfWidthNoQueue + 2)), 0 };
    wxSize queuePanelSize = { (int)std::round(scaleFactor * (mfWidthQueue - mfWidthNoQueue - 2)), (int)std::round(scaleFactor * (mfHeight - 60)) };
    wxPanel* queuePanel = new wxPanel(this, -1, queuePanelPos, queuePanelSize);

    wxBoxSizer* queueHBox = new wxBoxSizer(wxHORIZONTAL);
    queueHBox->AddSpacer((int)std::round(scaleFactor * 5));
    
    wxBoxSizer* queueVBox = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* queueLabelHBox = new wxBoxSizer(wxHORIZONTAL);
    queueLabel = new wxStaticText(queuePanel, wxID_ANY, wxT("Block Queue:"));

    queueLabelHBox->Add(queueLabel, 0);
    queueVBox->Add(queueLabelHBox, 0, wxLEFT | wxTOP | wxBOTTOM, (int)std::round(scaleFactor * 10));
    
    wxPoint queueListPos = { (int)std::round(scaleFactor * 100), 0 };
    wxSize queueListSize = { (int)std::round(scaleFactor * (mfWidthQueue - mfWidthNoQueue - 100)), (int)std::round(scaleFactor * (mfHeight - 150)) };
    wxArrayString queueEntries;
    fst_gui->blockQueue.getQueueStrings(queueEntries, runThread != nullptr);
    queueList = new wxListBox(queuePanel, ID_QUEUE_LIST, queueListPos, queueListSize, queueEntries, wxLB_SINGLE | wxLB_HSCROLL | wxLB_NEEDED_SB);

    queueVBox->Add(queueList, 0, wxLEFT | wxALIGN_CENTER, (int)std::round(scaleFactor * 40));

    queueVBox->Add(-1, (int)std::round(scaleFactor * 10));

    wxBoxSizer* queueButtonHBox = new wxBoxSizer(wxHORIZONTAL);
    addQueueButton = new wxButton(queuePanel, ID_ADD_QUEUE_BUTTON, wxT("Add Block"));
    queueButtonHBox->Add(addQueueButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 10));
    importQueueButton = new wxButton(queuePanel, ID_IMPORT_QUEUE_BUTTON, wxT("Import From Log"));
    queueButtonHBox->Add(importQueueButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 10));
    removeQueueButton = new wxButton(queuePanel, ID_REMOVE_QUEUE_BUTTON, wxT("Remove Block"));
    queueButtonHBox->Add(removeQueueButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 10));
    clearQueueButton = new wxButton(queuePanel, ID_CLEAR_QUEUE_BUTTON, wxT("Clear Queue"));
    queueButtonHBox->Add(clearQueueButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 10));
    queueVBox->Add(queueButtonHBox, 0, wxLEFT | wxALIGN_CENTER, (int)std::round(scaleFactor * 30));

    Connect(ID_ADD_QUEUE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickAddQueue));
    Connect(ID_IMPORT_QUEUE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickImportQueue));
    Connect(ID_REMOVE_QUEUE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickRemoveQueue));
    Connect(ID_CLEAR_QUEUE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(MainFrame::OnClickClearQueue));

    queueHBox->Add(queueVBox, 0);
    
    queuePanel->SetSizerAndFit(queueHBox);
    queuePanel->SetSize(queuePanelSize);

    CreateStatusBar(1, wxSTB_DEFAULT_STYLE);
    statusBar = GetStatusBar();

    Centre();

    fst_gui->saveSave();
}
