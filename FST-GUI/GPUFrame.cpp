#include "GPUFrame.hpp"

#include "MainFrame.hpp"
#include "utils.hpp"
#include <wx/valnum.h>
#include <filesystem>

void GPUFrame::OnClickDone(wxCommandEvent& event) {
    fst_gui->saveSave();

    this->Close();
}

void GPUFrame::OnClickReset(wxCommandEvent& event) {
    SaveData resetData;

    fst_gui->saveStruct.gpuModeSelected = MODE_CUDA;

    fst_gui->saveStruct.maxThreads = resetData.maxThreads;

    int maxThreads = INT_MAX;

    if (!fst_gui->deviceList().empty()) {
        fst_gui->saveStruct.gpuDeviceID = resetData.gpuDeviceID;

        maxThreads = fst_gui->deviceList()[fst_gui->saveStruct.gpuDeviceID].maxThreads;
        fst_gui->saveStruct.maxThreads = std::min(fst_gui->saveStruct.maxThreads, maxThreads);
    }
    else {
        fst_gui->saveStruct.gpuDeviceID = -1;
    }

    fst_gui->saveStruct.MAX_SK_PHASE_ONE = resetData.MAX_SK_PHASE_ONE;
    fst_gui->saveStruct.MAX_SK_PHASE_TWO_A = resetData.MAX_SK_PHASE_TWO_A;
    fst_gui->saveStruct.MAX_SK_PHASE_TWO_B = resetData.MAX_SK_PHASE_TWO_B;
    fst_gui->saveStruct.MAX_SK_PHASE_TWO_C = resetData.MAX_SK_PHASE_TWO_C;
    fst_gui->saveStruct.MAX_SK_PHASE_TWO_D = resetData.MAX_SK_PHASE_TWO_D;
    fst_gui->saveStruct.MAX_SK_PHASE_THREE = resetData.MAX_SK_PHASE_THREE;
    fst_gui->saveStruct.MAX_SK_PHASE_FOUR = resetData.MAX_SK_PHASE_FOUR;
    fst_gui->saveStruct.MAX_SK_PHASE_FIVE = resetData.MAX_SK_PHASE_FIVE;
    fst_gui->saveStruct.MAX_SK_PHASE_SIX = resetData.MAX_SK_PHASE_SIX;
    fst_gui->saveStruct.MAX_PLAT_SOLUTIONS = resetData.MAX_PLAT_SOLUTIONS;
    fst_gui->saveStruct.MAX_UPWARP_SOLUTIONS = resetData.MAX_UPWARP_SOLUTIONS;
    fst_gui->saveStruct.MAX_SK_UPWARP_SOLUTIONS = resetData.MAX_SK_UPWARP_SOLUTIONS;
    fst_gui->saveStruct.MAX_SPEED_SOLUTIONS = resetData.MAX_SPEED_SOLUTIONS;
    fst_gui->saveStruct.MAX_10K_SOLUTIONS = resetData.MAX_10K_SOLUTIONS;
    fst_gui->saveStruct.MAX_SLIDE_SOLUTIONS = resetData.MAX_SLIDE_SOLUTIONS;
    fst_gui->saveStruct.MAX_BD_SOLUTIONS = resetData.MAX_BD_SOLUTIONS;
    fst_gui->saveStruct.MAX_DOUBLE_10K_SOLUTIONS = resetData.MAX_DOUBLE_10K_SOLUTIONS;
    fst_gui->saveStruct.MAX_BULLY_PUSH_SOLUTIONS = resetData.MAX_BULLY_PUSH_SOLUTIONS;
    fst_gui->saveStruct.MAX_SQUISH_SPOTS = resetData.MAX_SQUISH_SPOTS;
    fst_gui->saveStruct.MAX_STRAIN_SETUPS = resetData.MAX_STRAIN_SETUPS;

    comboDev->Clear();

    for (DeviceInfo dev : fst_gui->deviceList()) {
        comboDev->Append(std::string(dev.name));
    }

    if (!comboDev->IsListEmpty()) {
        comboDev->SetSelection(fst_gui->saveStruct.gpuDeviceID);

        wxIntegerValidator<int>* threadVal = (wxIntegerValidator<int>*)threadBox->GetValidator();
        threadVal->SetMin(0);
        threadVal->SetMax(maxThreads);
    }

    cudaRadio->SetValue(true);
    
    threadBox->ChangeValue(std::to_string(fst_gui->saveStruct.maxThreads));

    solSK1->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_ONE));
    solSK2A->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_A));
    solSK2B->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_B));
    solSK2C->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_C));
    solSK2D->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_D));
    solSK3->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_THREE));
    solSK4->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_FOUR));
    solSK5->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_FIVE));
    solPlat->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_PLAT_SOLUTIONS));
    solUpwarp->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_UPWARP_SOLUTIONS));
    solSKUpwarp->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SK_UPWARP_SOLUTIONS));
    solSpeed->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SPEED_SOLUTIONS));
    sol10K->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_10K_SOLUTIONS));
    solSlide->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SLIDE_SOLUTIONS));
    solBD->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_BD_SOLUTIONS));
    solDouble10K->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_DOUBLE_10K_SOLUTIONS));
    solBP->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_BULLY_PUSH_SOLUTIONS));
    squishSpot->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_SQUISH_SPOTS));
    strainSetup->ChangeValue(std::to_string(fst_gui->saveStruct.MAX_STRAIN_SETUPS));

    MainFrame* parent = (MainFrame*)this->GetParent();
    parent->UpdateExePath();

    fst_gui->saveSave();
}

void GPUFrame::OnModeChange(wxCommandEvent& event) {
    MainFrame* parent = (MainFrame*)this->GetParent();

    comboDev->Clear();

    switch (event.GetId()) {
        case ID_CUDA_RADIO:
            fst_gui->saveStruct.gpuModeSelected = MODE_CUDA; 
            threadLabel->SetLabelText("Max Threads Per Block:");
            parent->addBlockOnRun = true;
            break;

        case ID_SYCL_RADIO:
            fst_gui->saveStruct.gpuModeSelected = MODE_SYCL;
            threadLabel->SetLabelText("Max Work Group Size:");
            parent->addBlockOnRun = true;
            break;
    }

    for (DeviceInfo dev : fst_gui->deviceList()) {
        comboDev->Append(std::string(dev.name));
    }

    if (fst_gui->deviceList().empty()) {
        fst_gui->saveStruct.gpuDeviceID = -1;
                
        wxIntegerValidator<int>* threadVal = (wxIntegerValidator<int>*)threadBox->GetValidator();
        threadVal->SetMin(0);
        threadVal->SetMax(INT_MAX);
    }
    else {
        fst_gui->saveStruct.gpuDeviceID = 0;
        int maxThreads = fst_gui->deviceList()[0].maxThreads;
        wxIntegerValidator<int>* threadVal = (wxIntegerValidator<int>*)threadBox->GetValidator();
        threadVal->SetMin(0);
        threadVal->SetMax(maxThreads);
        fst_gui->saveStruct.maxThreads = std::min(fst_gui->saveStruct.maxThreads, maxThreads);
        threadBox->ChangeValue(std::to_string(fst_gui->saveStruct.maxThreads));
    }

    parent->UpdateExePath();

    comboDev->SetSelection(0);

    fst_gui->saveSave();
}

void GPUFrame::OnComboChange(wxCommandEvent& event) {
    MainFrame* parent = (MainFrame*)this->GetParent();

    wxComboBox* comboBox = (wxComboBox*)event.GetEventObject();

    switch (event.GetId()) {
    case ID_GPU_DEV:
        if (!comboBox->IsListEmpty()) {
            fst_gui->saveStruct.gpuDeviceID = comboBox->GetSelection();
            int maxThreads = INT_MAX;
            switch (fst_gui->saveStruct.gpuModeSelected) {
                case MODE_CUDA:
                case MODE_SYCL:
                    maxThreads = fst_gui->deviceList()[fst_gui->saveStruct.gpuDeviceID].maxThreads;
            }
            wxIntegerValidator<int>* threadVal = (wxIntegerValidator<int>*)threadBox->GetValidator();
            threadVal->SetMin(0);
            threadVal->SetMax(maxThreads);
            fst_gui->saveStruct.maxThreads = std::min(fst_gui->saveStruct.maxThreads, maxThreads);
            threadBox->ChangeValue(std::to_string(fst_gui->saveStruct.maxThreads));
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        break;
    }
}

void GPUFrame::OnTextChange(wxCommandEvent& event) {
    MainFrame* parent = (MainFrame*)this->GetParent();

    wxTextCtrl* textBox = (wxTextCtrl*)event.GetEventObject();
    std::string s = textBox->GetLineText(0).ToStdString();

    switch (event.GetId()) {
    case ID_GPU_THREAD:
        try {
            fst_gui->saveStruct.maxThreads = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL1:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_ONE = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL2A:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_TWO_A = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL2B:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_TWO_B = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL2C:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_TWO_C = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL2D:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_TWO_D = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL3:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_THREE = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL4:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_FOUR = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL5:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_FIVE = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SK_SOL6:
        try {
            fst_gui->saveStruct.MAX_SK_PHASE_SIX = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_PLAT_SOL:
        try {
            fst_gui->saveStruct.MAX_PLAT_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_UPWARP_SOL:
        try {
            fst_gui->saveStruct.MAX_UPWARP_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SKUW_SOL:
        try {
            fst_gui->saveStruct.MAX_SK_UPWARP_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SPEED_SOL:
        try {
            fst_gui->saveStruct.MAX_SPEED_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_10K_SOL:
        try {
            fst_gui->saveStruct.MAX_10K_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SLIDE_SOL:
        try {
            fst_gui->saveStruct.MAX_SLIDE_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_BD_SOL:
        try {
            fst_gui->saveStruct.MAX_BD_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_DOUBLE_10K_SOL:
        try {
            fst_gui->saveStruct.MAX_DOUBLE_10K_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_BP_SOL:
        try {
            fst_gui->saveStruct.MAX_BULLY_PUSH_SOLUTIONS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_SQUISH_SPOTS:
        try {
            fst_gui->saveStruct.MAX_SQUISH_SPOTS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    case ID_STRAIN_SETUPS:
        try {
            fst_gui->saveStruct.MAX_STRAIN_SETUPS = std::stoi(s);
            parent->addBlockOnRun = true;
            fst_gui->saveSave();
        }
        catch (std::invalid_argument) {}
        break;
    }
}

GPUFrame::GPUFrame(wxWindow* parent, const wxString& title, FST_GUI* f)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(gpufWidth, gpufHeight), wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    double scaleFactor = this->GetDPIScaleFactor();
    this->SetSize(wxSize(scaleFactor * gpufWidth, scaleFactor * gpufHeight));

    fst_gui = f;

    wxPanel* panel = new wxPanel(this, -1);

    wxBoxSizer* mainVBox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* modeHBox = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* modeLabelVBox = new wxBoxSizer(wxVERTICAL);
    modeLabel = new wxStaticText(panel, wxID_ANY, wxT("Compute Library:"));

    modeLabelVBox->Add(modeLabel, 0, wxTOP, 0);
    modeHBox->Add(modeLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 8));

    wxBoxSizer* modeRadioHBox = new wxBoxSizer(wxHORIZONTAL);
    cudaRadio = new wxRadioButton(panel, ID_CUDA_RADIO, "CUDA");
    syclRadio = new wxRadioButton(panel, ID_SYCL_RADIO, "SYCL");

    switch (fst_gui->saveStruct.gpuModeSelected) {
        case (MODE_CUDA):
            cudaRadio->SetValue(true);
            break;
        case (MODE_SYCL):
            syclRadio->SetValue(true);
            break;
    }

    modeRadioHBox->Add(cudaRadio, 0, wxRIGHT, (int)std::round(scaleFactor * 10));
    modeRadioHBox->Add(syclRadio, 0, wxRIGHT, (int)std::round(scaleFactor * 10));
    modeHBox->Add(modeRadioHBox, 0, wxLEFT, (int)std::round(scaleFactor * 12));

    mainVBox->Add(modeHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    Connect(ID_CUDA_RADIO, wxEVT_RADIOBUTTON, wxCommandEventHandler(GPUFrame::OnModeChange));
    Connect(ID_SYCL_RADIO, wxEVT_RADIOBUTTON, wxCommandEventHandler(GPUFrame::OnModeChange));

    wxBoxSizer* devHBox = new wxBoxSizer(wxHORIZONTAL);
    devLabel = new wxStaticText(panel, wxID_ANY, wxT("GPU Device:"));

    wxBoxSizer* devLabelVBox = new wxBoxSizer(wxVERTICAL);
    devLabelVBox->Add(devLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    devHBox->Add(devLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 28));

    wxBoxSizer* devComboVBox = new wxBoxSizer(wxVERTICAL);

    wxArrayString comboDevOptions;

    switch (fst_gui->saveStruct.gpuModeSelected) {
        case (MODE_CUDA):
        case (MODE_SYCL):
            for (DeviceInfo dev : fst_gui->deviceList()) {
                comboDevOptions.Add(std::string(dev.name));
            }
            break;
    }

    fst_gui->saveStruct.gpuDeviceID = std::min(std::max(fst_gui->saveStruct.gpuDeviceID, 0), (int)comboDevOptions.size() - 1);

    comboDev = new wxComboBox(panel, ID_GPU_DEV, fst_gui->saveStruct.gpuDeviceID < 0 ? wxString("") : comboDevOptions[fst_gui->saveStruct.gpuDeviceID], wxDefaultPosition, wxDefaultSize, comboDevOptions, wxCB_READONLY);
    devComboVBox->Add(comboDev, 0, wxALL | wxALIGN_CENTER, 0);
    devHBox->Add(devComboVBox, (int)std::round(scaleFactor * 1));

    wxSize comboDevSize = comboDev->GetSize();
    comboDevSize.SetWidth((int)std::round(scaleFactor * 480));
    comboDev->SetMinSize(comboDevSize);

    mainVBox->Add(devHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    Connect(ID_GPU_DEV, wxEVT_COMBOBOX, wxCommandEventHandler(GPUFrame::OnComboChange));

    wxBoxSizer* threadHBox = new wxBoxSizer(wxHORIZONTAL);
    threadLabel = new wxStaticText(panel, wxID_ANY, fst_gui->saveStruct.gpuModeSelected == MODE_SYCL ? wxT("Max Work Group Size:") : wxT("Max Threads Per Block:"));

    wxBoxSizer* threadLabelVBox = new wxBoxSizer(wxVERTICAL);
    threadLabelVBox->Add(threadLabel, 0, wxTOP, (int)std::round(scaleFactor * 4));
    threadHBox->Add(threadLabelVBox, 0, wxRIGHT, (int)std::round(scaleFactor * 24));

    wxBoxSizer* threadTextVBox = new wxBoxSizer(wxVERTICAL);

    int maxThreads = INT_MAX;

    if (!fst_gui->deviceList().empty()) {
        switch (fst_gui->saveStruct.gpuModeSelected) {
        case MODE_CUDA:
        case MODE_SYCL:
            maxThreads = fst_gui->deviceList()[fst_gui->saveStruct.gpuDeviceID].maxThreads;
            break;
        }
    }

    maxThreads = (fst_gui->saveStruct.gpuDeviceID >= 0) ? maxThreads : 256;
    fst_gui->saveStruct.maxThreads = std::min(std::max(fst_gui->saveStruct.maxThreads, 0), maxThreads);

    wxIntegerValidator<int> threadVal(NULL, wxNUM_VAL_DEFAULT);
    threadVal.SetMin(0);
    threadVal.SetMax(maxThreads);

    threadBox = new wxTextCtrl(panel, ID_GPU_THREAD, std::to_string(fst_gui->saveStruct.maxThreads), wxDefaultPosition, wxDefaultSize, 0, threadVal);
    threadTextVBox->Add(threadBox, 0, wxALL, 0);
    threadHBox->Add(threadTextVBox, (int)std::round(scaleFactor * 1));

    mainVBox->Add(threadHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    Connect(ID_GPU_THREAD, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 10));

    wxIntegerValidator<int> intVal(NULL, wxNUM_VAL_DEFAULT);
    intVal.SetMin(0);

    wxBoxSizer* solutionsLabelHBox = new wxBoxSizer(wxHORIZONTAL);
    solutionsLabel = new wxStaticText(panel, wxID_ANY, wxT("GPU Solution Storage Limits:"));

    solutionsLabelHBox->Add(solutionsLabel, 0, wxRIGHT, (int)std::round(scaleFactor * 26));

    mainVBox->Add(solutionsLabelHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    wxBoxSizer* solutionsHBox = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer* solutionsGrid = new wxGridSizer(10, 4, (int)std::round(scaleFactor * 4), (int)std::round(scaleFactor * 8));

    solSK1Label = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 1:"));
    solutionsGrid->Add(solSK1Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK1 = new wxTextCtrl(panel, ID_SK_SOL1, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_ONE), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK1, 0, wxALL | wxALIGN_CENTER, 0);

    solSK2ALabel = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 2A:"));
    solutionsGrid->Add(solSK2ALabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK2A = new wxTextCtrl(panel, ID_SK_SOL2A, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_A), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK2A, 0, wxALL | wxALIGN_CENTER, 0);

    solSK2BLabel = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 2B:"));
    solutionsGrid->Add(solSK2BLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK2B = new wxTextCtrl(panel, ID_SK_SOL2B, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_B), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK2B, 0, wxALL | wxALIGN_CENTER, 0);

    solSK2CLabel = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 2C:"));
    solutionsGrid->Add(solSK2CLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK2C = new wxTextCtrl(panel, ID_SK_SOL2C, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_C), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK2C, 0, wxALL | wxALIGN_CENTER, 0);

    solSK2DLabel = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 2D:"));
    solutionsGrid->Add(solSK2DLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK2D = new wxTextCtrl(panel, ID_SK_SOL2D, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_TWO_D), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK2D, 0, wxALL | wxALIGN_CENTER, 0);

    solSK3Label = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 3:"));
    solutionsGrid->Add(solSK3Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK3 = new wxTextCtrl(panel, ID_SK_SOL3, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_THREE), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK3, 0, wxALL | wxALIGN_CENTER, 0);

    solSK4Label = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 4:"));
    solutionsGrid->Add(solSK4Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK4 = new wxTextCtrl(panel, ID_SK_SOL4, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_FOUR), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK4, 0, wxALL | wxALIGN_CENTER, 0);

    solSK5Label = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 5:"));
    solutionsGrid->Add(solSK5Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK5 = new wxTextCtrl(panel, ID_SK_SOL5, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_FIVE), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK5, 0, wxALL | wxALIGN_CENTER, 0);

    solSK6Label = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Solutions 6:"));
    solutionsGrid->Add(solSK6Label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSK6 = new wxTextCtrl(panel, ID_SK_SOL6, std::to_string(fst_gui->saveStruct.MAX_SK_PHASE_SIX), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSK6, 0, wxALL | wxALIGN_CENTER, 0);

    solPlatLabel = new wxStaticText(panel, wxID_ANY, wxT("Platform Solutions:"));
    solutionsGrid->Add(solPlatLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solPlat = new wxTextCtrl(panel, ID_PLAT_SOL, std::to_string(fst_gui->saveStruct.MAX_PLAT_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solPlat, 0, wxALL | wxALIGN_CENTER, 0);

    solUpwarpLabel = new wxStaticText(panel, wxID_ANY, wxT("Upwarp Solutions:"));
    solutionsGrid->Add(solUpwarpLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solUpwarp = new wxTextCtrl(panel, ID_UPWARP_SOL, std::to_string(fst_gui->saveStruct.MAX_UPWARP_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solUpwarp, 0, wxALL | wxALIGN_CENTER, 0);

    solSKUpwarpLabel = new wxStaticText(panel, wxID_ANY, wxT("Slide Kick Upwarp Solutions:"));
    solutionsGrid->Add(solSKUpwarpLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSKUpwarp = new wxTextCtrl(panel, ID_SKUW_SOL, std::to_string(fst_gui->saveStruct.MAX_SK_UPWARP_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSKUpwarp, 0, wxALL | wxALIGN_CENTER, 0);

    solSpeedLabel = new wxStaticText(panel, wxID_ANY, wxT("Speed Solutions:"));
    solutionsGrid->Add(solSpeedLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSpeed = new wxTextCtrl(panel, ID_SPEED_SOL, std::to_string(fst_gui->saveStruct.MAX_SPEED_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSpeed, 0, wxALL | wxALIGN_CENTER, 0);

    sol10KLabel = new wxStaticText(panel, wxID_ANY, wxT("10K Solutions:"));
    solutionsGrid->Add(sol10KLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    sol10K = new wxTextCtrl(panel, ID_10K_SOL, std::to_string(fst_gui->saveStruct.MAX_10K_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(sol10K, 0, wxALL | wxALIGN_CENTER, 0);

    solSlideLabel = new wxStaticText(panel, wxID_ANY, wxT("Slide Solutions:"));
    solutionsGrid->Add(solSlideLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solSlide = new wxTextCtrl(panel, ID_SLIDE_SOL, std::to_string(fst_gui->saveStruct.MAX_SLIDE_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solSlide, 0, wxALL | wxALIGN_CENTER, 0);

    solBDLabel = new wxStaticText(panel, wxID_ANY, wxT("Breakdance Solutions:"));
    solutionsGrid->Add(solBDLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solBD = new wxTextCtrl(panel, ID_BD_SOL, std::to_string(fst_gui->saveStruct.MAX_BD_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solBD, 0, wxALL | wxALIGN_CENTER, 0);

    solDouble10KLabel = new wxStaticText(panel, wxID_ANY, wxT("Double 10K Solutions:"));
    solutionsGrid->Add(solDouble10KLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solDouble10K = new wxTextCtrl(panel, ID_DOUBLE_10K_SOL, std::to_string(fst_gui->saveStruct.MAX_DOUBLE_10K_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solDouble10K, 0, wxALL | wxALIGN_CENTER, 0);

    solBPLabel = new wxStaticText(panel, wxID_ANY, wxT("Bully Push Solutions:"));
    solutionsGrid->Add(solBPLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    solBP = new wxTextCtrl(panel, ID_BP_SOL, std::to_string(fst_gui->saveStruct.MAX_BULLY_PUSH_SOLUTIONS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(solBP, 0, wxALL | wxALIGN_CENTER, 0);

    squishSpotLabel = new wxStaticText(panel, wxID_ANY, wxT("Squish Spots:"));
    solutionsGrid->Add(squishSpotLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    squishSpot = new wxTextCtrl(panel, ID_SQUISH_SPOTS, std::to_string(fst_gui->saveStruct.MAX_SQUISH_SPOTS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(squishSpot, 0, wxALL | wxALIGN_CENTER, 0);

    strainSetupLabel = new wxStaticText(panel, wxID_ANY, wxT("Strain Setups:"));
    solutionsGrid->Add(strainSetupLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    strainSetup = new wxTextCtrl(panel, ID_STRAIN_SETUPS, std::to_string(fst_gui->saveStruct.MAX_STRAIN_SETUPS), wxDefaultPosition, wxDefaultSize, 0, intVal);
    solutionsGrid->Add(strainSetup, 0, wxALL | wxALIGN_CENTER, 0);

    Connect(ID_SK_SOL1, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL2A, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL2B, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL2C, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL2D, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL3, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL4, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL5, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SK_SOL6, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_PLAT_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_UPWARP_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SKUW_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SPEED_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_10K_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SLIDE_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_BD_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_DOUBLE_10K_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_BP_SOL, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_SQUISH_SPOTS, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));
    Connect(ID_STRAIN_SETUPS, wxEVT_TEXT, wxCommandEventHandler(GPUFrame::OnTextChange));

    solutionsHBox->Add(solutionsGrid, 1, wxLEFT, (int)std::round(scaleFactor * 20));

    mainVBox->Add(solutionsHBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, (int)std::round(scaleFactor * 10));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 30));

    wxBoxSizer* buttonHBox = new wxBoxSizer(wxHORIZONTAL);
    resetButton = new wxButton(panel, ID_RESET_BUTTON, wxT("Reset to Defaults"));
    buttonHBox->Add(resetButton, 0);
    doneButton = new wxButton(panel, ID_DONE_BUTTON, wxT("Done"));
    buttonHBox->Add(doneButton, 0, wxLEFT | wxBOTTOM, (int)std::round(scaleFactor * 5));
    mainVBox->Add(buttonHBox, 0, wxALIGN_RIGHT | wxRIGHT, (int)std::round(scaleFactor * 10));

    Connect(ID_RESET_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(GPUFrame::OnClickReset));
    Connect(ID_DONE_BUTTON, wxEVT_BUTTON, wxCommandEventHandler(GPUFrame::OnClickDone));

    mainVBox->Add(-1, (int)std::round(scaleFactor * 35));

    panel->SetSizer(mainVBox);

    Centre();

    fst_gui->saveSave();
}
