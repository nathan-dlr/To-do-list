#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "MainFrame.h"

#ifndef TO_DO_APP_H
#define TO_DO_APP_H


class App : public wxApp
{
public:
    MainFrame* frame;
    bool OnInit() override;
};


#endif //TO_DO_APP_H
