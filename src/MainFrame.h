#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "Task.h"
#include "Database.h"

#ifndef TO_DO_MAINFRAME_H
#define TO_DO_MAINFRAME_H


class MainFrame : public wxFrame
{
public:
    MainFrame();


    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* taskContainer = new wxBoxSizer(wxVERTICAL);
    wxButton* addTask;
    wxTextCtrl* textBox;
    wxBitmapButton* editTaskButton;
    wxBitmapButton* removeTaskButton;
    Task* task;
    std::vector<Task*> tasks;

    void OnAddTask(wxCommandEvent const& event);
    void OnAppClose(wxCommandEvent const& event);
    void OnKeyboardEnterNew(wxCommandEvent const& event);
    void OnKeyboardEnterEdit(wxCommandEvent const& event);
    void OnEditButtonClick(wxCommandEvent const& event);
    void OnRemoveButtonClick(wxCommandEvent const& event);
    void CreateTask();
    void LoadTasks();
    void SetupSizer();
    Database* DB = new Database;
private:

    bool CAN_EDIT = true;

};


#endif //TO_DO_MAINFRAME_H
