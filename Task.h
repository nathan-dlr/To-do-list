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
    wxTextCtrl* getTextCtrl();
    wxCheckBox* getCheckBox();
    wxBitmapButton* getRemoveButton();
    wxBitmapButton* getEditButton();
    static std::size_t getEditOffset();
    static std::size_t getRemoveOffset();
    void publishTask(const wxString &taskDescription);
    wxTextCtrl* editTask(const wxString &label);
    wxBitmapButton* publishEditedTask(const wxString &taskDescription);
    void movePanel();
    void destroyPanel();

};


#endif //TO_DO_TASK_H
