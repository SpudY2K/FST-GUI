#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <vector>
#include "subprocess.h"

// the ID we'll use to identify our event
const int NUMBER_UPDATE_ID = 100000;

// a thread class that will periodically send events to the GUI thread
class RunThread : public wxThread
{
    wxFrame* m_parent;
    std::string process;
    std::vector<std::string> args;
    const char* command_c[100];
    struct subprocess_s subprocess;

public:
    RunThread(wxFrame* parent, std::string proc, std::vector<std::string>& a)
    {
        m_parent = parent;
        process = proc;
        args = a;

        command_c[0] = process.c_str();
        int i = 1;
        while (i <= args.size()) {
            command_c[i] = args[i-1].c_str();
            i++;
        }

        command_c[i] = NULL;
    }

    virtual ExitCode Entry();

    int terminate_subprocess();
};