#include  <wx/wx.h>

#ifndef TO_DO_TASK_H
#define TO_DO_TASK_H


class Task {

private:
    wxPanel* panel = nullptr;
    wxCheckBox* checkBox = nullptr;
    wxBitmapButton* removeButton = nullptr;
    wxBitmapButton* editButton = nullptr;
    wxTextCtrl* typeTask = nullptr;

public:
    Task();
    Task(wxWindow* Frame, int panelY,const std::string &taskDescription);
    Task(wxWindow* Frame, int panelY);
    wxTextCtrl* GetTextCtrl();
    wxCheckBox* GetCheckBox();
    wxBitmapButton* GetRemoveButton();
    wxBitmapButton* GetEditButton();
    wxBitmapButton* PublishTask(const wxString &taskDescription);
    wxTextCtrl* EditTask(const wxString &label);
    wxBitmapButton* PublishEditedTask(const wxString &taskDescription);
    void MovePanel();
    void DestroyPanel();

};


#endif //TO_DO_TASK_H
