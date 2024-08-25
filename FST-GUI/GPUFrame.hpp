#pragma once
#include "FST-GUI.hpp"
#include <wx/wx.h>

#ifdef _WIN32
    const int gpufWidth = 650;
    const int gpufHeight = 510;
#else
    const int gpufWidth = 650;
    const int gpufHeight = 660;
#endif

enum GPUFrameIDs {
    ID_GPU_DEV = 2000,
    ID_GPU_THREAD = 2001,
    ID_SK_SOL1 = 2002,
    ID_SK_SOL2A = 2003,
    ID_SK_SOL2B = 2004,
    ID_SK_SOL2C = 2005,
    ID_SK_SOL2D = 2006,
    ID_SK_SOL3 = 2007,
    ID_SK_SOL4 = 2008,
    ID_SK_SOL5 = 2009,
    ID_SK_SOL6 = 2010,
    ID_PLAT_SOL = 2011,
    ID_UPWARP_SOL = 2012,
    ID_SKUW_SOL = 2013,
    ID_SPEED_SOL = 2014,
    ID_10K_SOL = 2015,
    ID_SLIDE_SOL = 2016,
    ID_BD_SOL = 2017,
    ID_DOUBLE_10K_SOL = 2018,
    ID_BP_SOL = 2019,
    ID_SQUISH_SPOTS = 2020,
    ID_STRAIN_SETUPS = 2021,
    ID_RESET_BUTTON = 2022,
    ID_DONE_BUTTON = 2023,
    ID_CUDA_RADIO = 2024,
    ID_SYCL_RADIO = 2025
};

class GPUFrame : public wxDialog
{
public:
    GPUFrame(wxWindow* parent, const wxString& title, FST_GUI* f);
    void OnTextChange(wxCommandEvent& event);
    void OnModeChange(wxCommandEvent& event);
    void OnComboChange(wxCommandEvent& event);
    void OnClickDone(wxCommandEvent& event);
    void OnClickReset(wxCommandEvent& event);

private:
    FST_GUI* fst_gui;

    wxStaticText* modeLabel;
    wxRadioButton* cudaRadio;
    wxRadioButton* syclRadio;

    wxStaticText* devLabel;
    wxComboBox* comboDev;

    wxStaticText* threadLabel;
    wxTextCtrl* threadBox;

    wxStaticText* solutionsLabel;

    wxStaticText* solSK1Label;
    wxTextCtrl* solSK1;
    wxStaticText* solSK2ALabel;
    wxTextCtrl* solSK2A;
    wxStaticText* solSK2BLabel;
    wxTextCtrl* solSK2B;
    wxStaticText* solSK2CLabel;
    wxTextCtrl* solSK2C;
    wxStaticText* solSK2DLabel;
    wxTextCtrl* solSK2D;
    wxStaticText* solSK3Label;
    wxTextCtrl* solSK3;
    wxStaticText* solSK4Label;
    wxTextCtrl* solSK4;
    wxStaticText* solSK5Label;
    wxTextCtrl* solSK5;
    wxStaticText* solSK6Label;
    wxTextCtrl* solSK6;
    wxStaticText* solPlatLabel;
    wxTextCtrl* solPlat;
    wxStaticText* solUpwarpLabel;
    wxTextCtrl* solUpwarp;
    wxStaticText* solSKUpwarpLabel;
    wxTextCtrl* solSKUpwarp;
    wxStaticText* solSpeedLabel;
    wxTextCtrl* solSpeed;
    wxStaticText* sol10KLabel;
    wxTextCtrl* sol10K;
    wxStaticText* solSlideLabel;
    wxTextCtrl* solSlide;
    wxStaticText* solBDLabel;
    wxTextCtrl* solBD;
    wxStaticText* solDouble10KLabel;
    wxTextCtrl* solDouble10K;
    wxStaticText* solBPLabel;
    wxTextCtrl* solBP;
    wxStaticText* squishSpotLabel;
    wxTextCtrl* squishSpot;
    wxStaticText* strainSetupLabel;
    wxTextCtrl* strainSetup;

    wxButton* doneButton;
    wxButton* resetButton;
};
