// Start of wxWidgets "Hello World" Program
#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include <string.h>
#include "database.h"
#include "Task.h"
#include <cstddef>

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
    wxTextCtrl* typeTask;
    wxBitmapButton* editTaskButton;
    wxBitmapButton* removeTaskButton;
    Task* task;
    vector<Task*> tasks;

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
        task = new Task(this, TASK_Y, descriptions[i]);

        tasks.push_back(task);
        editTaskButton = task->getEditButton();
        editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
        removeTaskButton = task->getRemoveButton();
        removeTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);
    }
}

void MyFrame::CreateTask() {
    task = new Task(this, TASK_Y);
    tasks.push_back(task);

    typeTask = task->getTextCtrl();
    typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterNew, this);
    removeTaskButton = task->getRemoveButton();
    removeTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);
}

void MyFrame::OnEnterNew(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    insertTask(taskDescription);

    CAN_EDIT = true;
    task->publishTask(taskDescription);

    editTaskButton = task->getEditButton();
    editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
}

void MyFrame::OnEnterEdit(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    int index = (typeTask->GetParent()->GetPosition().y) / 50;

    editTaskButton = tasks[index]->publishEditedTask(taskDescription);
    editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);

    editTask(index, taskDescription);
    CAN_EDIT = true;
}

void MyFrame::OnExit(wxCommandEvent const& event) {
    for (int i = 0; i < tasks.size(); i++) {
        delete tasks[i];
    }
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
        wxObject const* obj = event.GetEventObject();
        auto const* button = wxDynamicCast(obj, wxBitmapButton);
        int index = (button->GetParent()->GetPosition().y) / 50;
        wxString label = tasks[index]->getCheckBox()->GetLabel();

        typeTask = tasks[index]->editTask(label);
        typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterEdit, this);
    }
}

void MyFrame::OnRemoveTask(wxCommandEvent const& event) {
    const wxObject* obj = event.GetEventObject();
    auto const* button = wxDynamicCast(obj, wxBitmapButton);
    int index = (button->GetParent()->GetPosition().y) / 50;
    //delete pre-established taskCheckBoxes
    tasks[index]->destroyPanel();
    delete tasks[index];
    tasks.erase(tasks.begin() + index);
    removeTask(index);

    for (int i = index ; i < tasks.size(); i++) {
        tasks[i]->movePanel();
    }

    TASK_Y -= 50;
    CAN_EDIT = true;
}
