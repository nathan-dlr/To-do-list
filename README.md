# To Do List

## Overview
This To Do List is a desktop application in which users can add, edit, and delete tasks from their list. It was written in C++ using [wxWidgets](https://www.wxwidgets.org/) for the graphical user interface and has a SQLite database for persistent storage. The application was unit tested using the [Catch2](https://github.com/catchorg/Catch2) framework. 

## Technologies 
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![sqlite](https://img.shields.io/badge/Sqlite-003B57?style=for-the-badge&logo=sqlite&logoColor=white)
![clion](https://img.shields.io/badge/CLion-000000?style=for-the-badge&logo=clion&logoColor=white)
![sonarlint](https://img.shields.io/badge/SonarLint-CB2029?style=for-the-badge&logo=sonarlint&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)

### Add Task
Press the "Add Task" button to create a new textbox and press enter to publish the task. Only one task can be added at a time after all other tasks have been published. If the window is not big enough to fit another task, resize the window to add more tasks. Published tasks are automatically saved to the SQLite database.

<p align="center">
 <img src="https://github.com/user-attachments/assets/3a3ea956-d5bc-41fa-9525-023fae72773d" alt="Add Task" width="700">
</p>

### Edit Task
Press the "Edit Task" button to create a new textbox over the selected task. You can edit any task as soon as it's been published. Once republished, the task description in the database is updated to reflect the changes.

<p align="center">
 <img src="https://github.com/user-attachments/assets/b0e3edfe-c105-482f-bbac-703b1c7fd356" alt="Edit Task" width="700">
</p>

### Remove Task
Press the "Remove Task" button to delete a task, doing so  pushes all subsequent tasks up to maintain consecutive rows. Tasks can be removed at any time. Deleting a task removes its entry from the database.

<p align="center">
 <img src="https://github.com/user-attachments/assets/20904b32-9612-4625-bb23-6c6bfbf4c8ab" alt="Edit Task" width="700">
</p>

## Resizing Window
The application maintains its layout as the window size changes.

<p align="center">
 <img src="https://github.com/user-attachments/assets/edc956b7-911b-4b3c-9ac8-78a57462f829" alt="Resize Window" width="700">
</p>

## Unit Testing
The Catch2 framework creates its own main function, so wxWidgets had to be initialized without its own main function by using the wxIMPLEMENT_APP_NO_MAIN() macro along with the wxEntryStart() method to allow access to the app. 
Creating an instance of the app allowed access its members to verify that the vector of tasks and database were updated correctly during add, edit, and remove operations.

## Challenges and New Concepts 
Learning and configuring wxWidgets, SQLite, and Catch2 was especially time-consuming since they all had to intertwine. 
Learning the four pillars of OOP revealed a lack of abstraction and encapsulation, so I restructured the internal logic to simplify bloated code.
