# Student Record System

This is a simple, command-line student record management system written in C. It allows a user to perform full CRUD (Create, Read, Update, Delete) operations on student records, which are stored in memory using a dynamic array. The system also supports saving the records to a file and loading them back into the program.

This entire application is contained within a single `main.c` file, demonstrating C programming fundamentals like dynamic memory management, file I/O, and user input handling.

## 🚀 Features

The program provides a text-based menu with the following capabilities:

  * **A - Add Student:** Add a new student record (Roll Number, Name, Marks).
    * **Note:** Students scoring above 50 marks are marked as **PASS**, otherwise **FAIL**.
  * **D - Display Students:** Display all current student records.
      * Provides sorting options:
        1.  Sort by Marks (Ascending)
        2.  Sort by Marks (Descending)
        3.  Display as-is (no sorting)
  * **M - Modify Student:** Find a student by roll number and update their...
      * Roll Number
      * Name
      * Marks
  * **R - Remove Student:** Find a student by roll number and remove their record.
  * **S - Search Student:** Find and display a single student's details by their roll number.
  * **C - Calculate Average:** Calculate and display the average marks for all students.
  * **E - Save to File:** Save the current list of students to a user-specified filename.
      This uses a structured plain-text format as shown below:
      ```
      <total_number_of_students>
      <student_1_name>
      <student_1_roll_number>
      <student_1_marks>
      <student_2_name>
      <student_2_roll_number>
      <student_2_marks>
      ... 
      ```
  * **L - Load from File:** Load student records from a user-specified file and append them to the current list of students in-memory.
      * **Note:** If a loaded student's roll number already exists in the system, it will be set to `-1` to prevent duplicates.
  * **Q - Quit:** Exit the program.

## 🔧 Getting Started

Follow these steps to get the project up and running on a Linux-based system.

### 1\. Prerequisites

You must have `git`, `cmake`, and a C compiler toolchain (like `gcc` and `make`) installed.

On Debian-based systems (like Ubuntu), you can install them all with:

```bash
sudo apt update
sudo apt install git cmake build-essential
```

### 2\. Clone the Repository

Use `git` to clone the project to your local machine.

```bash
git clone https://github.com/GRIITTYY/LabOne.git
cd LabOne
```

### 3\. Build and Run (Recommended)

The easiest way to run the program is using the included shell script. This script automatically creates a `build` directory, compiles the code, and runs the final executable.

```bash
./run.sh
```

*(**Note:** If you get a "Permission denied" error, you may need to make the script executable first by running: `chmod +x run.sh`)*


## 💻 Technical Details

  * **Dynamic Array:** The project uses a pointer (`Student *studentarray`) and `realloc` to manage a dynamic array of student records.

      * **Growth:** The array starts with a capacity of 10 and doubles in size whenever it becomes full (`totalStudents == capacity`).
      * **Shrinking:** To save memory, the array capacity is halved if the number of students drops below 25% of the total capacity (`totalStudents < capacity / 4`).

  * **Input Handling:**

      * A `clearInputBuffer()` function is used to consume leftover newline characters (`\n`) from the input buffer. This prevents `scanf` from conflicting with `fgets`.
      * `fgets` is used to read student names, allowing for names that include spaces.
      * `strcspn` is used to safely remove the trailing newline character from strings captured by `fgets`.

  * **File Format:** The save files (`.txt`) use a simple format:

    1.  The first line contains a single integer: the total number of students.
    2.  Each subsequent student record is saved as three lines:
          * Line 1: Student Name
          * Line 2: Student Roll Number
          * Line 3: Student Marks
            This format is used by both `saveToFile()` and `loadFromFile()`.

## 📁 File Structure

```
.
├── .gitignore       
├── CMakeLists.txt   
├── main.c           
└── run.sh           
```