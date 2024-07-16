#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "Task.h"
#include <cstddef>

Task::Task() = default;

Task::Task(wxWindow* Frame, int panelY) {
    panel = new wxPanel(Frame, wxID_ANY, wxPoint(0, panelY), wxSize(1000, 50));
    typeTask = new wxTextCtrl(panel, wxID_ANY,"Enter Task", wxPoint(0, 0), wxSize(900, 50), wxTE_PROCESS_ENTER);
    removeButton = new wxBitmapButton(panel, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, 10), wxSize(30, 30));
}

Task::Task(wxWindow* Frame, int panelY, const std::string &taskDescription) {
    panel = new wxPanel(Frame, wxID_ANY, wxPoint(0, panelY), wxSize(1000, 50));
    checkBox = new wxCheckBox(panel, wxID_ANY, taskDescription, wxPoint(50, 0), wxSize(500, 50));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, 10), wxSize(30, 30));
    removeButton = new wxBitmapButton(panel, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, 10), wxSize(30, 30));
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
    typeTask->Destroy();
    typeTask = nullptr;
    checkBox  = new wxCheckBox(panel, wxID_ANY, taskDescription, wxPoint(50, 0), wxSize(500, 50));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, 10), wxSize(30, 30));
    return editButton;
}

wxTextCtrl* Task::EditTask(const wxString &label) {
    editButton->Destroy();
    checkBox ->Destroy();
    editButton = nullptr;
    checkBox = nullptr;
    typeTask = new wxTextCtrl(panel, wxID_ANY,label.c_str(), wxPoint(0, 0), wxSize(900, 50), wxTE_PROCESS_ENTER);
    return typeTask;
}

wxBitmapButton* Task::PublishEditedTask(const wxString &taskDescription) {
    typeTask->Destroy();
    typeTask = nullptr;
    checkBox = new wxCheckBox(panel, wxID_ANY, taskDescription, wxPoint(50, 0), wxSize(500, 50));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, 10), wxSize(30, 30));
    return editButton;
}

void Task::MovePanel() {
    int y = panel->GetPosition().y - 50;
    panel->wxWindow::Move(0, y, wxSIZE_USE_EXISTING);
}

void Task::DestroyPanel() {
    panel->Destroy();
}