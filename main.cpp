// Start of wxWidgets "Hello World" Program
#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include <string.h>
#include "database.h"

using namespace std;

int ADD_TASK_Y;
int TASK_Y = -50;
bool CAN_EDIT = true;

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
    wxButton* addTask;
    wxBoxSizer* sizer;
    wxCheckBox* task;
    wxTextCtrl* typeTask;
    vector<wxCheckBox*> tasks;
    wxBitmapButton* editTaskButton;
    vector<wxBitmapButton*> editButtons;
    wxBitmapButton* removeTask;
    vector<wxBitmapButton*> removeButtons;
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAddTask(wxCommandEvent& event);
    void OnEnterNew(wxCommandEvent& event);
    void OnEnterEdit(wxCommandEvent& event);
    void OnEditTask(wxCommandEvent& event);
    void OnRemoveTask(wxCommandEvent& event);
    void CreateTask();
    void populateTasks();
};


//"this function is called  upon application startup and creates the main window"
bool MyApp::OnInit()
{
    //"there is no memory leak in this: wxWidgets takes ownership of all the window objects and destroys them automatically when the corresponding on-screen window is destroyed."
    MyFrame *frame = new MyFrame();
    frame->SetClientSize(1000, 600);
     //"frames are hidden by default to allow filling them to be filled with contents before showing everything at once."
    //"Show() must be called for the application to appear"
    frame->Show(true);

    //"returning true allows the application to start running"
    return true;
}

 MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "To Do") {
    //"menu pointers don't need to (and, in fact, must not) be destroyed because they are owned by the menu bar, which is itself owned by the frame, which is owned, i.e. will be destroyed, by wxWidgets."
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    populateTasks();

    addTask = new wxButton(this, wxID_ADD, "Add Task", wxPoint(0,ADD_TASK_Y), wxSize(500,50));
    addTask->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_BUTTON));

//    sizer = new wxBoxSizer(wxHORIZONTAL);
//    sizer->Add(addTask, 1);
//    SetSizerAndFit(sizer);

    //"We have to connect our event handlers to the events we want to handle in them. We do this by calling Bind() to send all the menu events (identified by wxEVT_MENU event type) with the specified ID to the given function."
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    addTask->Bind(wxEVT_BUTTON, &MyFrame::OnAddTask, this, wxID_ADD);
}

void MyFrame::populateTasks() {
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
    ADD_TASK_Y = TASK_Y + 50;
}

void MyFrame::CreateTask() {
    typeTask = new wxTextCtrl(this, wxID_ANY,"Enter Task", wxPoint(0, TASK_Y), wxSize(500, 50), wxTE_PROCESS_ENTER);
    removeTask = new wxBitmapButton(this, wxID_REMOVE, wxArtProvider::GetBitmapBundle(wxART_CROSS_MARK), wxPoint(950, TASK_Y + 10), wxSize(30,30));

    typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterNew, this);
    removeTask->Bind(wxEVT_BUTTON, &MyFrame::OnRemoveTask, this, wxID_REMOVE);
}

void MyFrame::OnEnterNew(wxCommandEvent &event) {
    wxString taskDescription = typeTask->GetValue();
    //create INSERT INTO statement
    insertTask(taskDescription);
    typeTask->Destroy();
    task = new wxCheckBox(this, wxID_ANY, taskDescription, wxPoint(50, TASK_Y), wxSize(500, 50));
    CAN_EDIT = true;
}

void MyFrame::OnEnterEdit(wxCommandEvent &event) {
    wxString taskDescription = typeTask->GetValue();
    wxPoint position = typeTask->GetPosition();
    int index = position.y / 50;
    editTask(index + 1, taskDescription);
    typeTask->Destroy();
    task = new wxCheckBox(this, wxID_ANY, taskDescription, wxPoint(50, position.y), wxSize(500, 50));
    editTaskButton = new wxBitmapButton(this, wxID_EDIT, wxArtProvider::GetBitmapBundle(wxART_EDIT), wxPoint(900, position.y + 10), wxSize(30, 30));
    tasks[index] = task;
    cout << tasks.size();
    editButtons[index] = editTaskButton;
    editTaskButton->Bind(wxEVT_BUTTON, &MyFrame::OnEditTask, this, wxID_EDIT);
    CAN_EDIT = true;
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnAddTask(wxCommandEvent& event) {
    //add an if-else statement that'll stop the button from moving when it's at the bottom of the screen
    if (CAN_EDIT) {
        CAN_EDIT = false;
        ADD_TASK_Y += 50;
        TASK_Y += 50;
        CreateTask();
        addTask->wxWindow::Move(0,ADD_TASK_Y, wxSIZE_USE_EXISTING);
    }
}

void MyFrame::OnEditTask(wxCommandEvent& event) {
    if (CAN_EDIT) {
        CAN_EDIT = false;
        wxObject* obj = event.GetEventObject();
        wxBitmapButton* button = wxDynamicCast(obj, wxBitmapButton);
        wxPoint position = button->GetPosition();
        int index = (position.y - 10) / 50;
        wxString label = tasks[index]->GetLabel();
        cout << tasks.size() << endl;
        tasks[index]->Destroy();
        editButtons[index]->Destroy();
        typeTask = new wxTextCtrl(this, wxID_ANY,label.c_str(), wxPoint(0, 50 * index), wxSize(500, 50), wxTE_PROCESS_ENTER);
        typeTask->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEnterEdit, this);
    }

    //turn checkbox into textbox and remove edit button

}

void MyFrame::OnRemoveTask(wxCommandEvent& event) {
    wxObject* obj = event.GetEventObject();
    wxBitmapButton* button = wxDynamicCast(obj, wxBitmapButton);
    wxPoint position = button->GetPosition();
    int index = (position.y - 10) / 50;
}