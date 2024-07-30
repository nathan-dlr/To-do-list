#include "MainFrame.h"
#include "App.h"
#include "catch.hpp"

const int INIT_WIDTH = 1000;
const int INIT_HEIGHT = 600;


bool App::OnInit() {
    frame = new MainFrame();
    frame->SetClientSize(INIT_WIDTH, INIT_HEIGHT);
    frame->Show(true);

    return true;
}