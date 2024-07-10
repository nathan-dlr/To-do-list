#include  <wx/wx.h>


#ifndef TO_DO_DATABASE_H
#define TO_DO_DATABASE_H

#endif //TO_DO_DATABASE_H


int insertTask(const wxString &taskDescription);
int countEntries();
std::vector<std::string> getDescription();