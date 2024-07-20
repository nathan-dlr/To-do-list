#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>

#ifndef TO_DO_MAINFRAME_H
#define TO_DO_MAINFRAME_H

#endif //TO_DO_MAINFRAME_H

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    bool CAN_EDIT = true;
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* taskContainer = new wxBoxSizer(wxVERTICAL);
    wxButton* addTask;
    wxTextCtrl* textBox;
    wxBitmapButton* editTaskButton;
    wxBitmapButton* removeTaskButton;
    Task* task;
    std::vector<Task*> tasks;
    Database* DB = new Database;


    void OnAppClose(wxCommandEvent const& event);
    void OnAddTask(wxCommandEvent const& event);
    void OnKeyboardEnterNew(wxCommandEvent const& event);
    void OnKeyboardEnterEdit(wxCommandEvent const& event);
    void OnEditButtonClick(wxCommandEvent const& event);
    void OnRemoveButtonClick(wxCommandEvent const& event);
    void CreateTask();
    void LoadTasks();
    void SetupSizer();
};