#include <iostream>
#include  <wx/wx.h>
#include <sqlite3.h>
#include <format>

using namespace std;

int TASK_ID;

int countEntries() {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open database for count" << endl;
    }
    string insert_stmnt = "SELECT COUNT(*) FROM tasks;";
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        cout << "Couldn't prepare count statement" << endl;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        rc = sqlite3_column_int(stmt, 0);
    }
    else {
       cout << "Execution with step failed for count" << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    TASK_ID = rc;
    return rc;
}


int insertTask(const wxString &taskDescription) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open database for insert" << endl;
    }
    string insert_stmnt = format("INSERT INTO tasks VALUES({}, '{}');",TASK_ID, taskDescription.ToStdString());
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        cout << "Couldn't prepare statement for insert" << endl;
    }
    sqlite3_bind_int(stmt, 1, TASK_ID);
    sqlite3_bind_text(stmt, 2, taskDescription.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Couldn't execute statement for insert" << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    TASK_ID++;
    return rc;
}

vector<string> getDescription() {
    vector<string> descriptions;
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open file for get" << endl;
    }
    string insert_stmt = "SELECT description FROM tasks;";
    rc = sqlite3_prepare_v2(DB, insert_stmt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Couldn't prepare statement for get" << endl;
    }
    string description_str;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* description = sqlite3_column_text(stmt, 0);
        int size = sqlite3_column_bytes(stmt, 0);
        for (int i = 0; i < size; i++) {
            description_str += description[i];
        }
        descriptions.push_back(description_str);
        description_str.clear();
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    return descriptions;
}

int editTask(int id, const wxString &taskDescription) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open file" << endl;
    }
    string insert_stmnt = format("UPDATE tasks SET description = '{}' WHERE id = {};",taskDescription.ToStdString(), id);
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        cout << "Couldn't prepare statement" << endl;
    }
    sqlite3_bind_text(stmt, 1, taskDescription.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Couldn't execute statement" << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return rc;
}

int removeTask(int id) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    const char *filename = "/Users/nathan/Dev/tasks";
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        cout << "Couldn't open file" << endl;
    }
    //delete row
    string insert_stmt = format("DELETE FROM tasks WHERE id = {};", id);
    rc = sqlite3_prepare_v2(DB, insert_stmt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Couldn't prepare statement" << endl;
    }
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Couldn't execute statement" << endl;
    }
    sqlite3_finalize(stmt);
    TASK_ID--;
    //decrement ID's
    insert_stmt = format("UPDATE tasks set id = id-1 where id > {};", id);
    rc = sqlite3_prepare_v2(DB, insert_stmt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "Couldn't prepare statement" << endl;
    }
    sqlite3_bind_int(stmt,1,id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Couldn't execute statement" << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return rc;
}