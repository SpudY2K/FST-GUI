#pragma once
#include <wx/wx.h>
#include "FST-GUI.hpp"
#include "RunThread.hpp"

#ifdef _WIN32
    const int mfWidthQueue = 1100;
    const int mfWidthNoQueue = 600;
    const int mfHeight = 750;
#else
    const int mfWidthQueue = 1250;
    const int mfWidthNoQueue = 650;
    const int mfHeight = 950;
#endif

wxDECLARE_EVENT(wxEVT_RUNTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_RUNTHREAD_UPDATE, wxThreadEvent);

enum MainFrameIDs {
    ID_NORM_Z_MODE = 1000,
    ID_NORM_MIN_X = 1001,
    ID_NORM_MAX_X = 1002,
    ID_NORM_MIN_Y = 1003,
    ID_NORM_MAX_Y = 1004,
    ID_NORM_MIN_Z = 1005,
    ID_NORM_MAX_Z = 1006,
    ID_NORM_SAMPLES_X = 1007,
    ID_NORM_SAMPLES_Y = 1008,
    ID_NORM_SAMPLES_Z = 1009,
    ID_PLATFORM_X = 1010,
    ID_OUT_PATH = 1011,
    ID_OUT_BROWSE_BUTTON = 1012,
    ID_GPU_BUTTON = 1013,
    ID_RUN_BUTTON = 1014,
    ID_EXE_PATH = 1015,
    ID_EXE_BROWSE_BUTTON = 1016,
    ID_QUEUE_BUTTON = 1017,
    ID_QUEUE_LIST = 1018,
    ID_ADD_QUEUE_BUTTON = 1019,
    ID_REMOVE_QUEUE_BUTTON = 1020,
    ID_CLEAR_QUEUE_BUTTON = 1021,
    ID_IMPORT_QUEUE_BUTTON = 1022,
    ID_CTRL_V = 1023,
    ID_TOP_QUEUE_BUTTON = 1024,
    ID_UP_QUEUE_BUTTON = 1025,
    ID_DOWN_QUEUE_BUTTON = 1026,
    ID_BOTTOM_QUEUE_BUTTON = 1027
};

class FST_GUI;
struct BlockData;

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, FST_GUI* f);
    void OnKeyPress(wxKeyEvent& event);
    void OnTextChange(wxCommandEvent& event);
    void OnComboChange(wxCommandEvent& event);
    void OnClickBrowse(wxCommandEvent& event);
    void OnClickQueue(wxCommandEvent& event);
    void OnClickGPU(wxCommandEvent& event);
    void OnClickRun(wxCommandEvent& event);
    void OnClickAddQueue(wxCommandEvent& event);
    void OnClickRemoveQueue(wxCommandEvent& event);
    void OnClickClearQueue(wxCommandEvent& event);
    void OnClickImportQueue(wxCommandEvent& event);
    void OnClickQueueTop(wxCommandEvent& event);
    void OnClickQueueUp(wxCommandEvent& event);
    void OnClickQueueDown(wxCommandEvent& event);
    void OnClickQueueBottom(wxCommandEvent& event);
    void OnThreadCompletion(wxThreadEvent& event);
    void OnThreadUpdate(wxThreadEvent& event);
    void OnClose(wxCloseEvent& event);
    void UpdateExePath();
    bool CheckOutputDirectory();
    bool AddBlockToQueue();
    void RunNextBlock();
    void UpdateQueueList(int selectedIdx);
    void UpdateQueueList();
    bool PasteFromClipboard();
    void MoveQueue(int source, int target);

    bool addBlockOnRun = true;
    bool removeBlockOnCancel = false;

protected:
    RunThread *runThread = nullptr;
    friend class RunThread;

private:
    void SetupArgs(BlockData* blockParams, std::vector<std::string>& args);

    const int maxOutputLines = 10000;
    const int float_precision = 9;

    bool queueVisible = false;
    bool runCancelled = false;

    FST_GUI* fst_gui;

    wxStaticText* exeFileLabel;
    wxTextCtrl* exeFileText;
    wxButton* exeBrowseButton;

    wxStaticText* normLabel;
    wxStaticText* labelX;
    wxStaticText* labelY;
    wxComboBox* comboZ;

    wxTextCtrl* minBoxX;
    wxTextCtrl* minBoxY;
    wxTextCtrl* minBoxZ;
    wxTextCtrl* maxBoxX;
    wxTextCtrl* maxBoxY;
    wxTextCtrl* maxBoxZ;

    wxStaticText* samplesLabel;

    wxTextCtrl* samplesX;
    wxTextCtrl* samplesY;
    wxTextCtrl* samplesZ;

    wxTextCtrl* gapsX;
    wxTextCtrl* gapsY;
    wxTextCtrl* gapsZ;

    wxStaticText* platformLabel;
    wxComboBox* platformX;
    wxTextCtrl* platformY;
    wxTextCtrl* platformZ;

    wxStaticText* outFileLabel;
    wxTextCtrl* outFileText;
    wxButton* outBrowseButton;

    wxButton* gpuButton;
    wxButton* runButton;

    wxStaticText* outputLabel;
    wxTextCtrl* outputBox;

    wxStatusBar* statusBar;

    wxButton* showQueueButton;

    wxStaticText* queueLabel;
    wxListBox* queueList;
    wxButton* addQueueButton;
    wxButton* removeQueueButton;
    wxButton* clearQueueButton;
    wxButton* importQueueButton;

    wxButton* topQueueButton;
    wxButton* upQueueButton;
    wxButton* downQueueButton;
    wxButton* bottomQueueButton;
};
