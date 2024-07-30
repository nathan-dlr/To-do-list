#include <iostream>
#include <wx/wx.h>
#include "Task.h"
#include "MainFrame.h"
#include "wx/uiaction.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "App.h"
wxDECLARE_APP(App);
wxIMPLEMENT_APP_NO_MAIN(App);


TEST_CASE("App and frame initialization") {
    int argc = 0;
    char** argv = nullptr;
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    App* test_app = &wxGetApp();
    REQUIRE(test_app != nullptr);
    MainFrame* test_frame = test_app->frame;
    REQUIRE(test_frame != nullptr);
    wxTheApp->OnExit();
    wxEntryCleanup();
}

TEST_CASE("Frame should be 600-700 x 1000 pixels") {
    int argc = 0;
    char **argv = nullptr;
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    const App* test_app = &wxGetApp();
    const MainFrame* test_frame = test_app->frame;
    wxSize frame_size = test_frame->GetSize();
    REQUIRE(frame_size.x == 1000);
    REQUIRE((frame_size.y >= 600 && frame_size.y <= 700));
    wxTheApp->OnExit();
    wxEntryCleanup();
}

TEST_CASE("Creating task should make vector size increase") {
    int argc = 0;
    char **argv = nullptr;
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    const App* test_app = &wxGetApp();
    MainFrame* test_frame = test_app->frame;
    test_frame->CreateTask();
    REQUIRE(test_frame->tasks.size() == 1);
    wxTheApp->OnExit();
    wxEntryCleanup();
}

TEST_CASE("When no DB entries, tasks vector should be empty") {
    int argc = 0;
    char **argv = nullptr;
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    const App* test_app = &wxGetApp();
    const MainFrame* test_frame = test_app->frame;
    REQUIRE(test_frame->tasks.empty());
    wxTheApp->OnExit();
    wxEntryCleanup();
}

TEST_CASE("OnKeyboardEnterNew should make a new task in the DB") {
    int argc = 0;
    char **argv = nullptr;
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    const App *test_app = &wxGetApp();
    MainFrame *test_frame = test_app->frame;
    test_frame->CreateTask();
    wxString test_val = test_frame->textBox->GetValue();
    REQUIRE(test_val == "Enter task");
    test_frame->OnKeyboardEnterNew(wxCommandEvent());
//    REQUIRE(test_frame->DB->CountEntries() == 1);
    test_frame->DB->DeleteRows();
//    REQUIRE(test_frame->DB->CountEntries() == 0);
    wxTheApp->OnExit();
    wxEntryCleanup();
}

TEST_CASE("OnKeyboardEnterEdit should edit task in DB") {
    int argc = 0;
    char **argv = nullptr;
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    const App *test_app = &wxGetApp();
    MainFrame *test_frame = test_app->frame;
    test_frame->CreateTask();
    wxString test_val = test_frame->textBox->GetValue();
    REQUIRE(test_val == "Enter task");
    test_frame->OnKeyboardEnterNew(wxCommandEvent());
    wxString label = test_frame->tasks[0]->GetCheckBox()->GetLabel();
    test_frame->textBox = test_frame->tasks[0]->EditTask(label);
    test_frame->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnKeyboardEnterEdit, test_frame);

    test_frame->textBox->SetLabelText("Edited Task");
    test_frame->OnKeyboardEnterEdit(wxCommandEvent());
    REQUIRE(test_frame->DB->CountEntries() == 1);
    REQUIRE(test_frame->DB->GetDescription()[0] == "Edited Task");
    test_frame->DB->DeleteRows();

    wxTheApp->OnExit();
    wxEntryCleanup();
}