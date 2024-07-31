#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "MainFrame.h"

using namespace std;

const int Y_POS_OFFSET = 10;
const int ADD_X = 900;
const int ADD_Y = 50;
const int ADD_TASK_BORDER = 25;

 MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "To Do") {
    LoadTasks();

    addTask = new wxButton(this, wxID_ADD, "Add Task", wxDefaultPosition, wxSize(ADD_X, ADD_Y));
    addTask->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_BUTTON));
    addTask->Bind(wxEVT_BUTTON, &MainFrame::OnAddTask, this, wxID_ADD);

    SetupSizer();
    //"We have to connect our event handlers to the events we want to handle in them. We do this by calling Bind() to send all the menu events (identified by wxEVT_MENU event type) with the specified ID to the given function."
    Bind(wxEVT_MENU, &MainFrame::OnAppClose, this, wxID_EXIT);

}

void MainFrame::LoadTasks() {
    int count = DB->CountEntries();
    vector<string> descriptions = DB->GetDescription();
    for (int i = 0; i < count; i++) {
        task = new Task(this, descriptions[i], taskContainer);

        tasks.push_back(task);
        editTaskButton = task->GetEditButton();
        removeTaskButton = task->GetRemoveButton();
        editTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditButtonClick, this, wxID_EDIT);
        removeTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnRemoveButtonClick, this, wxID_REMOVE);
    }
}

void MainFrame::SetupSizer() {
    mainSizer->Add(taskContainer, wxSizerFlags().Expand());
    mainSizer->AddStretchSpacer();
    mainSizer->Add(addTask, wxSizerFlags().Expand().Border(wxALL, ADD_TASK_BORDER));
    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
}

void MainFrame::OnAddTask(wxCommandEvent const& event) {
    if ((CAN_EDIT && tasks.empty()) || (CAN_EDIT && (taskContainer->GetSize().y + tasks[0]->GetPanelHeight() < addTask->GetPosition().y) )) {
        CAN_EDIT = false;
        CreateTask();
    }
}

void MainFrame::CreateTask() {
    task = new Task(this, taskContainer);
    tasks.push_back(task);

    textBox = task->GetTextCtrl();
    removeTaskButton = task->GetRemoveButton();
    textBox->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnKeyboardEnterNew, this);
    removeTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnRemoveButtonClick, this, wxID_REMOVE);

    Layout();
}

void MainFrame::OnKeyboardEnterNew(wxCommandEvent const &event) {
    wxString taskDescription = textBox->GetValue();
    DB->InsertData(taskDescription);

    editTaskButton = task->PublishTask(taskDescription);
    editTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditButtonClick, this, wxID_EDIT);

    CAN_EDIT = true;
    Layout();
}

void MainFrame::OnEditButtonClick(wxCommandEvent const& event) {
    if (CAN_EDIT) {
        CAN_EDIT = false;
        wxObject const* obj = event.GetEventObject();
        auto const* button = wxDynamicCast(obj, wxBitmapButton);
        auto const* currentPanel = button->GetParent();
        int index = (currentPanel->GetPosition().y) / (currentPanel->GetSize().y + Y_POS_OFFSET);
        wxString label = tasks[index]->GetCheckBox()->GetLabel();

        textBox = tasks[index]->EditTask(label);
        textBox->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnKeyboardEnterEdit, this);
        Layout();
    }
}

void MainFrame::OnKeyboardEnterEdit(wxCommandEvent const &event) {
    wxString taskDescription = textBox->GetValue();
    auto const* currentPanel = textBox->GetParent();
    int index = (currentPanel->GetPosition().y) / (currentPanel->GetSize().y + Y_POS_OFFSET);
    DB->EditData(index, taskDescription);

    editTaskButton = tasks[index]->PublishTask(taskDescription);
    editTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditButtonClick, this, wxID_EDIT);

    CAN_EDIT = true;
    Layout();
}

void MainFrame::OnRemoveButtonClick(wxCommandEvent const& event) {
    const wxObject* obj = event.GetEventObject();
    auto const* button = wxDynamicCast(obj, wxBitmapButton);
    auto const* currentPanel = button->GetParent();
    if (CAN_EDIT || textBox->GetParent() == currentPanel) {
        int index = (currentPanel->GetPosition().y) / (currentPanel->GetSize().y + Y_POS_OFFSET);
        //delete pre-established taskCheckBoxes
        tasks[index]->DestroyPanel();
        delete tasks[index];
        tasks.erase(tasks.begin() + index);
        DB->RemoveData(index);

        for (int i = index ; i < tasks.size(); i++) {
            tasks[i]->MovePanel();
        }

        CAN_EDIT = true;
        Layout();
    }
}

void MainFrame::OnAppClose(wxCommandEvent const& event) {
    for (Task* current : tasks) {
        delete current;
    }
    delete DB;
    Close(true);
}

