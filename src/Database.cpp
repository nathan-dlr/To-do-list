#include <iostream>
#include  <wx/wx.h>
#include <sqlite3.h>
#include <format>
#include "Database.h"

int Database::CountEntries() {
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        std::cout << "Couldn't open Database for count" << std::endl;
    }
    std::string insert_stmnt = "SELECT COUNT(*) FROM tasks;";
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        std::cout << "Couldn't prepare count statement" << std::endl;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        rc = sqlite3_column_int(stmt, 0);
    }
    else {
       std::cout << "Execution with step failed for count" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    TASK_ID = rc;
    return rc;
}


int Database::InsertData(const wxString &taskDescription) {
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        std::cout << "Couldn't open Database for insert" << std::endl;
    }
    std::string insert_stmnt = format("INSERT INTO tasks VALUES({}, '{}');",TASK_ID, taskDescription.ToStdString());
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        std::cout << "Couldn't prepare statement for insert" << std::endl;
    }
    sqlite3_bind_int(stmt, 1, TASK_ID);
    sqlite3_bind_text(stmt, 2, taskDescription.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cout << "Couldn't execute statement for insert" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    TASK_ID++;
    return rc;
}

std::vector<std::string> Database::GetDescription() {
    std::vector<std::string> descriptions;
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        std::cout << "Couldn't open file for get" << std::endl;
    }
   std::string insert_stmt = "SELECT description FROM tasks;";
    rc = sqlite3_prepare_v2(DB, insert_stmt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Couldn't prepare statement for get" << std::endl;
    }
    std::string description_str;
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

int Database::EditData(int id, const wxString &taskDescription) {
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        std::cout << "Couldn't open file" << std::endl;
    }
    std::string insert_stmnt = format("UPDATE tasks SET description = '{}' WHERE id = {};",taskDescription.ToStdString(), id);
    rc = sqlite3_prepare_v2(DB, insert_stmnt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        std::cout << "Couldn't prepare statement" << std::endl;
    }
    sqlite3_bind_text(stmt, 1, taskDescription.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cout << "Couldn't execute statement" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return rc;
}

int Database::RemoveData(int id) {
    int rc = sqlite3_open(filename, &DB);
    if (rc) {
        std::cout << "Couldn't open file" << std::endl;
    }
    //delete row
    std::string insert_stmt = std::format("DELETE FROM tasks WHERE id = {};", id);
    rc = sqlite3_prepare_v2(DB, insert_stmt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Couldn't prepare statement" << std::endl;
    }
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cout << "Couldn't execute statement" << std::endl;
    }
    sqlite3_finalize(stmt);
    TASK_ID--;
    //decrement ID's
    insert_stmt = std::format("UPDATE tasks set id = id-1 where id > {};", id);
    rc = sqlite3_prepare_v2(DB, insert_stmt.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Couldn't prepare statement" << std::endl;
    }
    sqlite3_bind_int(stmt,1,id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cout << "Couldn't execute statement" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return rc;
}