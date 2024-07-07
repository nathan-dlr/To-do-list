#include <iostream>
#include  <wx/wx.h>
#include <sqlite3.h>
#include <format>

using namespace std;

int TASK_ID = 0;

int countEntries() {
    sqlite3* DB;
    sqlite3_stmt * stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open database" << endl;
    }
//    cout << "Opened file" << endl;
    string insert_stmnt = "SELECT COUNT(*) FROM tasks";
//    cout << insert_stmnt << endl;
    return rc;
}


int insertTask(const wxString &taskDescription) {
    sqlite3* DB;
    sqlite3_stmt * stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open database" << endl;
    }
//    cout << "Opened file" << endl;
    string insert_stmnt = format("INSERT INTO tasks VALUES ({}, '{}')", to_string(TASK_ID), taskDescription.ToStdString());
//    cout << insert_stmnt << endl;
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK){
        cout << "Couldn't prepare statement" << endl;
    }
//    else {
//        cout << "Prepared statement" << endl;
//    }
    sqlite3_bind_int(stmt, 1, TASK_ID);
    sqlite3_bind_text(stmt, 2, taskDescription.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Couldn't execute statement" << endl;
    }
//    else //        cout << "Executed with step" << endl;
//    }
    sqlite3_finalize(stmt);
//    cout << "Destroyed statement" << endl;
    sqlite3_close(DB);
//    cout << "Closed database" << endl;
    TASK_ID++;
    return rc;
}