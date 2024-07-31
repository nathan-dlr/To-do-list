#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "Task.h"
#include "catch.hpp"

const int PANEL_X = 1000;
const int TEXTBOX_X = 900;
const int CHECKBOX_X = 500;
const int Y_SIZE = 50;
const int BUTTONS_SIZE = 30;
const int ADD_TASK_BORDER = 5;

Task::Task() = default;

Task::Task(wxWindow* Frame, wxBoxSizer* container) {
    panel = new wxPanel(Frame, wxID_ANY, wxDefaultPosition, wxSize(PANEL_X, Y_SIZE));
    textBox = new wxTextCtrl(panel, wxID_ANY, "Enter task", wxDefaultPosition, wxSize(TEXTBOX_X,  Y_SIZE), wxTE_PROCESS_ENTER);
    textBox->SetFocus();
    removeButton = new wxBitmapButton(panel, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxDefaultPosition, wxSize(BUTTONS_SIZE, BUTTONS_SIZE));

    taskSizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(taskSizer);
    taskSizer->Add(textBox, wxSizerFlags().Proportion(1).Expand().Border(wxALL, ADD_TASK_BORDER));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, ADD_TASK_BORDER));
    container->Add(panel, wxSizerFlags().Expand().Border(wxALL, ADD_TASK_BORDER));
}

Task::Task(wxWindow* Frame, const std::string &taskDescription,  wxBoxSizer* container) {
    panel = new wxPanel(Frame, wxID_ANY, wxDefaultPosition, wxSize(PANEL_X, Y_SIZE));
    checkBox = new wxCheckBox(panel, wxID_ANY, taskDescription, wxDefaultPosition, wxSize(CHECKBOX_X, Y_SIZE));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxDefaultPosition, wxSize(BUTTONS_SIZE, BUTTONS_SIZE));
    removeButton = new wxBitmapButton(panel, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxDefaultPosition, wxSize(BUTTONS_SIZE, BUTTONS_SIZE));

    taskSizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(taskSizer);
    taskSizer->Add(checkBox, wxSizerFlags().Proportion(1).Expand().Border(wxLEFT | wxUP, ADD_TASK_BORDER));
    taskSizer->Add(editButton, wxSizerFlags().Border(wxALL, ADD_TASK_BORDER));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, ADD_TASK_BORDER));
    container->Add(panel, wxSizerFlags().Expand().Border(wxALL, ADD_TASK_BORDER));
}

wxTextCtrl* Task::GetTextCtrl() {
    return textBox;
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

int Task::GetPanelHeight() {
    return panel->GetSize().y;
}

wxBitmapButton* Task::PublishTask(const wxString &taskDescription) {
    taskSizer->Detach(textBox);
    taskSizer->Detach(removeButton);
    textBox->Destroy();
    textBox = nullptr;
    checkBox  = new wxCheckBox(panel, wxID_ANY, taskDescription, wxDefaultPosition, wxSize(CHECKBOX_X, Y_SIZE));
    editButton = new wxBitmapButton(panel, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxDefaultPosition, wxSize(BUTTONS_SIZE, BUTTONS_SIZE));

    taskSizer->Add(checkBox, wxSizerFlags().Proportion(1).Expand().Border(wxALL, ADD_TASK_BORDER));
    taskSizer->Add(editButton, wxSizerFlags().Border(wxALL, ADD_TASK_BORDER));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, ADD_TASK_BORDER));
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
    textBox = new wxTextCtrl(panel, wxID_ANY, label.c_str(), wxDefaultPosition, wxSize(TEXTBOX_X, Y_SIZE), wxTE_PROCESS_ENTER);
    textBox->SetFocus();

    taskSizer->Add(textBox, wxSizerFlags().Proportion(1).Expand().Border(wxALL, ADD_TASK_BORDER));
    taskSizer->Add(removeButton, wxSizerFlags().Border(wxALL, ADD_TASK_BORDER));
    return textBox;
}

void Task::MovePanel() {
    int y = panel->GetPosition().y - Y_SIZE;
    panel->wxWindow::Move(0, y, wxSIZE_USE_EXISTING);
}

void Task::DestroyPanel() {
    panel->Destroy();
}