// Start of wxWidgets "Hello World" Program
#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include <string.h>
#include "database.h"
#include "Task.h"

using namespace std;


class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

//"wxIMPLEMENT_APP provides appropriate entry function depending on the platform being used"
wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    int TASK_Y = -50;
    bool CAN_EDIT = true;
    wxButton* addTask;
    wxBoxSizer* sizer;
    wxCheckBox* taskCheckBox;
    wxTextCtrl* typeTask;
    vector<wxCheckBox*> taskCheckBoxes;
    wxBitmapButton* editTaskButton;
    vector<wxBitmapButton*> editButtons;
    wxBitmapButton* removeTaskButton;
    vector<wxBitmapButton*> removeButtons;
    void OnExit(wxCommandEvent const& event);
    void OnAddTask(wxCommandEvent const& event);
    void OnEnterNew(wxCommandEvent const& event);
    void OnEnterEdit(wxCommandEvent const& event);
    void OnEditTask(wxCommandEvent const& event);
    void OnRemoveTask(wxCommandEvent const& event);
    void CreateTask();
    void PopulateTasks();
};


//"this function is called  upon application startup and creates the main window"
bool MyApp::OnInit()
{
    //"there is no memory leak in this: wxWidgets takes ownership of all the window objects and destroys them automatically when the corresponding on-screen window is destroyed."
    auto *frame = new MyFrame();
    frame->SetClientSize(1000, 600);
     //"frames are hidden by default to allow filling them to be filled with contents before showing everything at once."
    //"Show() must be called for the application to appear"
    frame->Show(true);

    //"returning true allows the application to start running"
    return true;
}

 MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "To Do") {
    //"menu pointers don't need to (and, in fact, must not) be destroyed because they are owned by the menu bar, which is itself owned by the frame, which is owned, i.e. will be destroyed, by wxWidgets."
    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    auto *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    PopulateTasks();

    addTask = new wxButton(this, wxID_ADD, "Add Task", wxPoint(50,500), wxSize(900,50));
    addTask->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_BUTTON));
    addTask->Bind(wxEVT_BUTTON, &MyFrame::OnAddTask, this, wxID_ADD);

    //"We have to connect our event handlers to the events we want to handle in them. We do this by calling Bind() to send all the menu events (identified by wxEVT_MENU event type) with the specified ID to the given function."
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

}

void MyFrame::PopulateTasks() {
    int count = countEntries();
    vector<string> descriptions = getDescription();
    for (int i = 0; i < count; i++) {
        TASK_Y += 50;
        taskCheckBox = new wxCheckBox(this, wxID_ANY, descriptions[i], wxPoint(50, TASK_Y), wxSize(500, 50));
        editTaskButton = new wxBitmapButton(this, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, TASK_Y + 10), wxSize(30, 30));
        removeTaskButton = new wxBitmapButton(this, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, TASK_Y + 10), wxSize(30, 30));

        editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
        removeTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);

        editButtons.push_back(editTaskButton);
        removeButtons.push_back(removeTaskButton);
        taskCheckBoxes.push_back(taskCheckBox);
    }
}

void MyFrame::CreateTask() {
    typeTask = new wxTextCtrl(this, wxID_ANY,"Enter Task", wxPoint(0, TASK_Y), wxSize(500, 50), wxTE_PROCESS_ENTER);
    removeTaskButton = new wxBitmapButton(this, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, TASK_Y + 10), wxSize(30, 30));
    removeButtons.push_back(removeTaskButton);

    typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterNew, this);
    removeTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);
}

void MyFrame::OnEnterNew(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    insertTask(taskDescription);

    typeTask->Destroy();
    CAN_EDIT = true;

    taskCheckBox = new wxCheckBox(this, wxID_ANY, taskDescription, wxPoint(50, TASK_Y), wxSize(500, 50));
    taskCheckBoxes.push_back(taskCheckBox);
    editTaskButton = new wxBitmapButton(this, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, TASK_Y + 10), wxSize(30, 30));
    editButtons.push_back(editTaskButton);
    editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
}

void MyFrame::OnEnterEdit(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    wxPoint position = typeTask->GetPosition();
    int index = position.y / 50;
    editTask(index, taskDescription);

    typeTask->Destroy();
    CAN_EDIT = true;

    taskCheckBox = new wxCheckBox(this, wxID_ANY, taskDescription, wxPoint(50, position.y), wxSize(500, 50));
    taskCheckBoxes[index] = taskCheckBox;
    editTaskButton = new wxBitmapButton(this, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, position.y + 10), wxSize(30, 30));
    editButtons[index] = editTaskButton;
    editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
}

void MyFrame::OnExit(wxCommandEvent const& event) {
    Close(true);
}

void MyFrame::OnAddTask(wxCommandEvent const& event) {
    if (CAN_EDIT && addTask->GetPosition().y >= TASK_Y + 50) {
        CAN_EDIT = false;
        TASK_Y += 50;
        CreateTask();
    }
}

void MyFrame::OnEditTask(wxCommandEvent const& event) {
    if (CAN_EDIT) {
        CAN_EDIT = false;
        const wxObject* obj = event.GetEventObject();
        auto const* button = wxDynamicCast(obj, wxBitmapButton);
        wxPoint position = button->GetPosition();
        int index = (position.y - 10) / 50;
        wxString label = taskCheckBoxes[index]->GetLabel();

        taskCheckBoxes[index]->Destroy();
        editButtons[index]->Destroy();
        //making the indexes null helps us keep track of what to delete if someone presses a remove button
        taskCheckBoxes[index] = nullptr;
        editButtons[index] = nullptr;

        typeTask = new wxTextCtrl(this, wxID_ANY,label.c_str(), wxPoint(0, 50 * index), wxSize(500, 50), wxTE_PROCESS_ENTER);
        typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterEdit, this);
    }
}

void MyFrame::OnRemoveTask(wxCommandEvent const& event) {
    const wxObject* obj = event.GetEventObject();
    auto const* button = wxDynamicCast(obj, wxBitmapButton);
    wxPoint position = button->GetPosition();
    int index = (position.y - 10) / 50;
    //delete pre-established taskCheckBoxes
    if (taskCheckBoxes[index] != nullptr && taskCheckBoxes.size() > index) {
        taskCheckBoxes[index]->Destroy();
        editButtons[index]->Destroy();
    }
    //this branch could be taken when someone is deleting while editing a Task or deleting while adding a Task
    else {
        typeTask->Destroy();
    }
    //there will always be remove buttons to be destroyed thus the vector must always delete something
    removeButtons[index]->Destroy();
    removeButtons.erase(removeButtons.begin() + index);
    if (index != taskCheckBoxes.size()) {
        //taskCheckBoxes and edit buttons only need to be erased when the Task being destroyed has already been established
        //additionally that means their vectors/DB entries only need to be cleaned up on this condition as well
        taskCheckBoxes.erase(taskCheckBoxes.begin() + index);
        editButtons.erase(editButtons.begin() + index);
        removeTask(index);
        for (int i = index; i < taskCheckBoxes.size(); i++) {
            int y_pos = removeButtons[i]->GetPosition().y - 50;
            editButtons[i]->wxWindow::Move(900, y_pos, wxSIZE_USE_EXISTING);
            taskCheckBoxes[i]->wxWindow::Move(50, taskCheckBoxes[i]->GetPosition().y - 50, wxSIZE_USE_EXISTING);
            removeButtons[i]->wxWindow::Move(950, y_pos, wxSIZE_USE_EXISTING);
        }
    }
    TASK_Y -= 50;
    CAN_EDIT = true;
}