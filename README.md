# To-do-list

## Overview
The goal of this project was to create a to-do list application in C++ that features a GUI and persistent storage. I used the wxWidgets library for the frontend, and SQLite for the backened database.

## Features
The main features include adding a task, editing a task, and removing a task.

### Add Task
Press the "Add Task" button to create a new textbox and press enter to publish the task. Only one task can be added at a time after all other tasks have been published. If the window is not big enough to fit another task,resize the window to add more tasks. Published tasks are automatically saved to the SQLite database.

### Edit Task
Press the "Edit Task" button to create a new textbox over the selected task. You can edit any task as soon as it's been published. Once republished, the task description in the database is updated to reflect the changes.

### Remove Task
Press the "Remove Task" button to delete a task, doing so  pushes all subsequent tasks up to maintain consecutive rows. Tasks can be removed at any time. Deleting a task removes its entry from the database.

## Resizing Window
The application maintains its layout as the window size changes.
