#include "RunThread.hpp"
#include "MainFrame.hpp"

int RunThread::terminate_subprocess() {
    return subprocess_terminate(&subprocess);
}

wxThread::ExitCode RunThread::Entry()
{
    char buffer[128];

    int result = subprocess_create(command_c, subprocess_option_no_window | subprocess_option_combined_stdout_stderr | subprocess_option_search_user_path | subprocess_option_inherit_environment | subprocess_option_enable_async, &subprocess);

    FILE* p_stdout = subprocess_stdout(&subprocess);

    if (result != 0) {
        wxThreadEvent completedEvent(wxEVT_RUNTHREAD_COMPLETED);
        wxPostEvent(this->m_parent, completedEvent);

        return (wxThread::ExitCode)1;
    }

    fflush(p_stdout);

    // read till end of process:
    while (!TestDestroy() && !feof(p_stdout)) {
        // use buffer to read and add to result
        if (fgets(buffer, 128, p_stdout) != NULL) {

            wxThreadEvent updateEvent(wxEVT_RUNTHREAD_UPDATE);
            updateEvent.SetString(buffer);
            wxPostEvent(this->m_parent, updateEvent);
        }

        fflush(p_stdout);
    }

    result = subprocess_terminate(&subprocess);

    int exitCode = 0;

    subprocess_join(&subprocess, &exitCode);

    wxThreadEvent completedEvent(wxEVT_RUNTHREAD_COMPLETED);
    wxPostEvent(this->m_parent, completedEvent);

    return (wxThread::ExitCode)exitCode;
}