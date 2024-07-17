#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "Database.h"
#include "Task.h"

using namespace std;

Database* DB = new Database;

class App : public wxApp
{
public:
    bool OnInit() override;
};

//"wxIMPLEMENT_APP provides appropriate entry function depending on the platform being used"
wxIMPLEMENT_APP(App);

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    int TASK_Y = -50;
    bool CAN_EDIT = true;
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* taskContainer = new wxBoxSizer(wxVERTICAL);
    wxButton* addTask;
    wxTextCtrl* typeTask;
    wxBitmapButton* editTaskButton;
    wxBitmapButton* removeTaskButton;
    Task* task;
    vector<Task*> tasks;

    void OnExit(wxCommandEvent const& event);
    void OnAddTask(wxCommandEvent const& event);
    void OnEnterNew(wxCommandEvent const& event);
    void OnEnterEdit(wxCommandEvent const& event);
    void OnEditButtonClick(wxCommandEvent const& event);
    void OnRemoveTask(wxCommandEvent const& event);
    void CreateTask();
    void LoadTasks();
    void SetupSizer();
};

bool App::OnInit()
{
    //"there is no memory leak in this: wxWidgets takes ownership of all the window objects and destroys them automatically when the corresponding on-screen window is destroyed."
    // Using new here is fine since wxWidgets automatically manages memory
    auto *frame = new MainFrame();
    // TODO - add these sizes as constants
    frame->SetClientSize(1000, 600);
     //"frames are hidden by default to allow filling them to be filled with contents before showing everything at once."
    //"Show() must be called for the application to appear"
    frame->Show(true);

    //"returning true allows the application to start running"
    return true;
}

 MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "To Do") {
    //"menu pointers don't need to (and, in fact, must not) be destroyed because they are owned by the menu bar, which is itself owned by the frame, which is owned, i.e. will be destroyed, by wxWidgets."
    auto *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    auto *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

     LoadTasks();

    addTask = new wxButton(this, wxID_ADD, "Add Task", wxDefaultPosition, wxSize(900,50));
    addTask->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_BUTTON));
    addTask->Bind(wxEVT_BUTTON, &MainFrame::OnAddTask, this, wxID_ADD);

    SetupSizer();
    //"We have to connect our event handlers to the events we want to handle in them. We do this by calling Bind() to send all the menu events (identified by wxEVT_MENU event type) with the specified ID to the given function."
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

}

void MainFrame::LoadTasks() {
    int count = DB->CountEntries();
    vector<string> descriptions = DB->GetDescription();
    for (int i = 0; i < count; i++) {
        TASK_Y += 50; // TODO - should be a const; If refactor doesn't autosize task, then create taskSize consts
        task = new Task(this, TASK_Y, descriptions[i], taskContainer);

        tasks.push_back(task);
        editTaskButton = task->GetEditButton();
        removeTaskButton = task->GetRemoveButton();
        editTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditButtonClick, this, wxID_EDIT);
        removeTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnRemoveTask, this, wxID_REMOVE);
    }
}

void MainFrame::SetupSizer() {
    mainSizer->Add(taskContainer, wxSizerFlags().Expand());
    mainSizer->AddStretchSpacer();
    mainSizer->Add(addTask, wxSizerFlags().Expand().Border(wxALL, 25));
    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
}

void MainFrame::OnAddTask(wxCommandEvent const& event) {
    if (CAN_EDIT && addTask->GetPosition().y >= TASK_Y + 50) {
        CAN_EDIT = false;
        TASK_Y += 50;
        CreateTask();
    }
}

void MainFrame::CreateTask() {
    task = new Task(this, TASK_Y, taskContainer);
    tasks.push_back(task);

    typeTask = task->GetTextCtrl(); // TODO - rename to something better like textBox
    removeTaskButton = task->GetRemoveButton();
    typeTask->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnEnterNew, this); // TODO - what is winid?
    removeTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnRemoveTask, this, wxID_REMOVE);

    Layout();
}
// TODO - consider renaming to OnKeyboardEnterNew
void MainFrame::OnEnterNew(wxCommandEvent const &event) { // TODO - figure out if you need this param, if not then comment why unsused and needed
    wxString taskDescription = typeTask->GetValue();
    DB->InsertData(taskDescription);

    editTaskButton = task->PublishTask(taskDescription);
    editTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditButtonClick, this, wxID_EDIT);

    CAN_EDIT = true;
    Layout();
}

// TODO - consider renaming to OnEditButtonClick()
void MainFrame::OnEditButtonClick(wxCommandEvent const& event) {
    if (CAN_EDIT) {
        CAN_EDIT = false;
        wxObject const* obj = event.GetEventObject();
        auto const* button = wxDynamicCast(obj, wxBitmapButton);
        auto const* currentPanel = button->GetParent();
        int index = (currentPanel->GetPosition().y) / currentPanel->GetSize().y;
        wxString label = tasks[index]->GetCheckBox()->GetLabel();

        typeTask = tasks[index]->EditTask(label);
        typeTask->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnEnterEdit, this);
        Layout();
    }
}
// TODO - consider renaming to OnKeyboardEnterEditTask / onEditTask
// TODO - figure out if this only can happen on enter. What happens on Tab or mouse/cursor leave?
void MainFrame::OnEnterEdit(wxCommandEvent const &event) {
    wxString taskDescription = typeTask->GetValue();
    auto const* currentPanel = typeTask->GetParent();
    int index = (currentPanel->GetPosition().y) / currentPanel->GetSize().y;
    DB->EditData(index, taskDescription);

    editTaskButton = tasks[index]->PublishTask(taskDescription);
    editTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnEditButtonClick, this, wxID_EDIT);

    CAN_EDIT = true;
    Layout();
}

void MainFrame::OnRemoveTask(wxCommandEvent const& event) {
    const wxObject* obj = event.GetEventObject();
    auto const* button = wxDynamicCast(obj, wxBitmapButton);
    auto const* currentPanel = button->GetParent();
    if (CAN_EDIT || typeTask->GetParent() == currentPanel) {
        int index = (currentPanel->GetPosition().y) / currentPanel->GetSize().y;
        //delete pre-established taskCheckBoxes
        tasks[index]->DestroyPanel();
        delete tasks[index];
        tasks.erase(tasks.begin() + index);
        DB->RemoveData(index);

        for (int i = index ; i < tasks.size(); i++) {
            tasks[i]->MovePanel();
        }

        TASK_Y -= 50;
        CAN_EDIT = true;
        Layout();
    }
}
// OnAppClose
void MainFrame::OnExit(wxCommandEvent const& event) {
    for (Task* current : tasks) {
        delete current;
    }
    delete DB;
    Close(true);
}

