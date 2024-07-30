#include  <wx/wx.h>
#include <sqlite3.h>


#ifndef TO_DO_DATABASE_H
#define TO_DO_DATABASE_H


class Database {
private:
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int TASK_ID = 0;
public:
    int InsertData(const wxString &taskDescription);
    int CountEntries();
    std::vector<std::string> GetDescription();
    int EditData(int id, const wxString &taskDescription);
    int RemoveData(int id);
    int DeleteRows();
};


#endif //TO_DO_DATABASE_H
