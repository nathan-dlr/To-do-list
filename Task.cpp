#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "Task.h"

Task::Task() = default;

Task::Task(wxWindow* Frame, int panelY, wxBoxSizer* container) {
    panel = new wxPanel(Frame, wxID_ANY, wxDefaultPosition, wxSize(1000, 50));
    typeTask = new wxTextCtrl(panel, wxID_ANY,"Enter task", wxDefaultPosition, wxSize(900, 50), wxTE_PROCESS_ENTER);
    removeButton = new wxBitmapButton(panel, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxDefaultPosition, wxSize(30, 30));

    taskSizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(taskSizer);
    taskSizer->Add(typeTask, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, 5));
    container->Add(panel, wxSizerFlags().Expand().Border(wxALL, 5));
}

Task::Task(wxWindow* Frame, int panelY, const std::string &taskDescription,  wxBoxSizer* container) {
    panel = new wxPanel(Frame, wxID_ANY, wxDefaultPosition, wxSize(1000, 50));
    checkBox = new wxCheckBox(panel, wxID_ANY, taskDescription, wxDefaultPosition, wxSize(500, 50));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxDefaultPosition, wxSize(30, 30));
    removeButton = new wxBitmapButton(panel, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxDefaultPosition, wxSize(30, 30));

    taskSizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(taskSizer);
    taskSizer->Add(checkBox, wxSizerFlags().Proportion(1).Expand().Border(wxLEFT | wxUP, 5));
    taskSizer->Add(editButton, wxSizerFlags().Border(wxALL, 5));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, 5));
    container->Add(panel, wxSizerFlags().Expand().Border(wxALL, 5));
}

wxTextCtrl* Task::GetTextCtrl() {
    return typeTask;
}

wxCheckBox* Task::GetCheckBox() {
    return checkBox;
}

wxBitmapButton* Task::GetRemoveButton() {
    return removeButton;
}

wxBitmapButton* Task::GetEditButton() {
    return editButton;
}

wxBitmapButton* Task::PublishTask(const wxString &taskDescription) {
    taskSizer->Detach(typeTask);
    taskSizer->Detach(removeButton);
    typeTask->Destroy();
    typeTask = nullptr;
    checkBox  = new wxCheckBox(panel, wxID_ANY, taskDescription, wxDefaultPosition, wxSize(500, 50));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxDefaultPosition, wxSize(30, 30));

    taskSizer->Add(checkBox, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5));
    taskSizer->Add(editButton, wxSizerFlags().Border(wxALL, 5));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, 5));
    return editButton;
}

wxTextCtrl* Task::EditTask(const wxString &label) {
    taskSizer->Detach(checkBox);
    taskSizer->Detach(editButton);
    taskSizer->Detach(removeButton);
    editButton->Destroy();
    checkBox ->Destroy();
    editButton = nullptr;
    checkBox = nullptr;
    typeTask = new wxTextCtrl(panel, wxID_ANY,label.c_str(), wxDefaultPosition, wxSize(900, 50), wxTE_PROCESS_ENTER);

    taskSizer->Add(typeTask, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 5));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL,5));
    return typeTask;
}

void Task::MovePanel() {
    int y = panel->GetPosition().y - 50;
    panel->wxWindow::Move(0, y, wxSIZE_USE_EXISTING);
}

void Task::DestroyPanel() {
    panel->Destroy();
}