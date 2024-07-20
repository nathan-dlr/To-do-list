#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "Database.h"
#include "Task.h"
#include "MainFrame.h"
#include "App.h"

const int INIT_W = 1000;
const int INIT_H = 600;

//"wxIMPLEMENT_APP provides appropriate entry function depending on the platform being used"
wxIMPLEMENT_APP(App);

bool App::OnInit() {
    // Using new here is fine since wxWidgets automatically manages memory
    auto *frame = new MainFrame();
    frame->SetClientSize(INIT_W, INIT_H);
    //"frames are hidden by default to allow filling them to be filled with contents before showing everything at once."
    //"Show() must be called for the application to appear"
    frame->Show(true);

    //"returning true allows the application to start running"
    return true;
}