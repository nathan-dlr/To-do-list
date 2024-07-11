// Start of wxWidgets "Hello World" Program
#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include <string.h>
#include "database.h"

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
    wxCheckBox* task;
    wxTextCtrl* typeTask;
    vector<wxCheckBox*> tasks;
    wxBitmapButton* editTaskButton;
    vector<wxBitmapButton*> editButtons;
    wxBitmapButton* removeTask;
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

//    sizer = new wxBoxSizer(wxHORIZONTAL);
//    sizer->Add(addTask, 1);
//    SetSizerAndFit(sizer);

    //"We have to connect our event handlers to the events we want to handle in them. We do this by calling Bind() to send all the menu events (identified by wxEVT_MENU event type) with the specified ID to the given function."
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    addTask->Bind(wxEVT_BUTTON, &MyFrame::OnAddTask, this, wxID_ADD);
}

void MyFrame::PopulateTasks() {
    int count = countEntries();
    vector<string> descriptions = getDescription();
    for (int i = 0; i < count; i++) {
        TASK_Y += 50;
        task = new wxCheckBox(this, wxID_ANY, descriptions[i], wxPoint(50, TASK_Y), wxSize(500,50));
        editTaskButton = new wxBitmapButton(this, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, TASK_Y + 10), wxSize(30, 30));
        removeTask = new wxBitmapButton(this, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, TASK_Y + 10), wxSize(30,30));

        editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
        removeTask->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);

        editButtons.push_back(editTaskButton);
        removeButtons.push_back(removeTask);
        tasks.push_back(task);
    }
}

void MyFrame::CreateTask() {
    typeTask = new wxTextCtrl(this, wxID_ANY,"Enter Task", wxPoint(0, TASK_Y), wxSize(500, 50), wxTE_PROCESS_ENTER);
    removeTask = new wxBitmapButton(this, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, TASK_Y + 10), wxSize(30,30));
    removeButtons.push_back(removeTask);

    typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterNew, this);
    removeTask->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);
}

void MyFrame::OnEnterNew(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    //create INSERT INTO statement
    insertTask(taskDescription);
    typeTask->Destroy();
    task = new wxCheckBox(this, wxID_ANY, taskDescription, wxPoint(50, TASK_Y), wxSize(500, 50));
    CAN_EDIT = true;
}

void MyFrame::OnEnterEdit(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    wxPoint position = typeTask->GetPosition();
    int index = position.y / 50;
    editTask(index + 1, taskDescription);
    typeTask->Destroy();
    task = new wxCheckBox(this, wxID_ANY, taskDescription, wxPoint(50, position.y), wxSize(500, 50));
    editTaskButton = new wxBitmapButton(this, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, position.y + 10), wxSize(30, 30));
    tasks[index] = task;
    editButtons[index] = editTaskButton;
    editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
    CAN_EDIT = true;
}

void MyFrame::OnExit(wxCommandEvent const& event)
{
    Close(true);
}

void MyFrame::OnAddTask(wxCommandEvent const& event) {
    //add an if-else statement that'll stop the button from moving when it's at the bottom of the screen
    if (CAN_EDIT) {
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
        wxString label = tasks[index]->GetLabel();
        tasks[index]->Destroy();
        editButtons[index]->Destroy();
        //making the indexes null helps us keep track of what to delete if someone presses a remove button
        tasks[index] = nullptr;
        editButtons[index] = nullptr;
        typeTask = new wxTextCtrl(this, wxID_ANY,label.c_str(), wxPoint(0, 50 * index), wxSize(500, 50), wxTE_PROCESS_ENTER);
        typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterEdit, this);
    }

    //turn checkbox into textbox and remove edit button

}

void MyFrame::OnRemoveTask(wxCommandEvent const& event) {
    const wxObject* obj = event.GetEventObject();
    auto const* button = wxDynamicCast(obj, wxBitmapButton);
    wxPoint position = button->GetPosition();
    int index = (position.y - 10) / 50;
    //delete pre-established tasks
    if (tasks[index] != nullptr && tasks.size() > index) {
        tasks[index]->Destroy();
        editButtons[index]->Destroy();
    }
    //this branch could be taken when someone is deleting while editing a task or deleting while adding a task
    else {
        typeTask->Destroy();
    }
    //there will always be remove buttons to be destroyed thus the vector must always delete something
    removeButtons[index]->Destroy();
    removeButtons.erase(removeButtons.begin() + index);
    if (index != tasks.size()) {
        //tasks and edit buttons only need to be erased when the task being destroyed has already been established
        //additionally that means their vectors only need to be cleaned up on this condition as well
        tasks.erase(tasks.begin() + index);
        editButtons.erase(editButtons.begin() + index);
        for (int i = index; i < tasks.size(); i++) {
            int y_pos = removeButtons[i]->GetPosition().y - 50;
            editButtons[i]->wxWindow::Move(900, y_pos, wxSIZE_USE_EXISTING);
            tasks[i]->wxWindow::Move(50, tasks[i]->GetPosition().y - 50, wxSIZE_USE_EXISTING);
            removeButtons[i]->wxWindow::Move(950, y_pos, wxSIZE_USE_EXISTING);
        }
    }
    TASK_Y -= 50;
    CAN_EDIT = true;
}