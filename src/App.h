#include <iostream>
#include  <wx/wx.h>
#include <wx/artprov.h>
#include "MainFrame.h"

#ifndef TO_DO_APP_H
#define TO_DO_APP_H


class App : public wxApp
{
public:
    bool OnInit() override;

private:
    MainFrame* frame;

};


#endif //TO_DO_APP_H
