/**
 * Student Record System
 * This program allows managing student records, including adding, modifying,
 * displaying, searching, and deleting studentarray. It also supports saving
 * records to a file and loading them, calculating average marks, and sorting.
 */

#include <stdio.h>
#include <stdlib.h> // For dynamic memory (malloc, realloc, free) and qsort
#include <string.h> // For string manipulation (strcpy, strcmp, fgets)
#include <ctype.h>  // For toupper()

// Will contain User's Name
char userName[50];

// The structure to hold student information
typedef struct {
    char name[100]; // Allows up to 100 characters only
    int rollNumber;
    float marks;
} Student;

// --- Global Variables ---
Student *studentarray = NULL; // Pointer for the dynamic array of students
int totalStudents = 0;      // Current number of students in memory
int capacity = 0;         // Current allocated capacity of the array
const float PASS_THRESHOLD = 50.0;

// --- Function Prototypes ---
void welcomeUser();
void showMenu();
void clearInputBuffer();
void addStudent();
void modifyStudent();
void displayAllStudents();
void searchStudent();
void deleteStudent();
void calculateAverage();
int compareAsc(const void *a, const void *b);
int compareDesc(const void *a, const void *b);
void saveToFile();
void loadFromFile();
int findStudentByRoll(int roll);

// --- Main Function ---
int main() {
    welcomeUser();

    char option;
    do {
        showMenu();

        // Read user's choice as a character
        if (scanf(" %c", &option) != 1) {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        option = toupper(option); // Convert to uppercase to handle 'a' or 'A'

        switch (option) {
            case 'A':
                addStudent();
                break;
            case 'M':
                modifyStudent();
                break;
            case 'R':
                deleteStudent();
                break;
            case 'S':
                searchStudent();
                break;
            case 'D':
                displayAllStudents();
                break;
            case 'C':
                calculateAverage();
                break;
            case 'E':
                saveToFile();
                break;
            case 'L':
                loadFromFile();
                break;
            case 'Q':
                printf("\nGoodbye %s!\n\n", userName);
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (option != 'Q');

    // Free memory
    free(studentarray);
    studentarray = NULL;

    return 0;
}

/**
 * @brief Displays a welcome message and greets the user by name.
 */
void welcomeUser() {
    printf("--- Welcome to the Student Record System ---\n");
    printf("Please enter your name: ");
    fgets(userName, sizeof(userName), stdin);
    // Remove trailing newline character from fgets
    userName[strcspn(userName, "\n")] = 0;
    printf("\nHello, %s! Let's get you started.\n", userName);
}

/**
 * @brief Displays the main menu options.
 */
void showMenu() {
    printf("\n==== Student Record System Menu ====\n");
    printf("A. Add a new student\n");
    printf("D. Display all students\n");
    printf("M. Modify a student record\n");
    printf("R. Remove a student record\n");
    printf("S. Search for a student by roll number\n");
    printf("C. Show average marks\n");
    printf("E. Save records to file\n");
    printf("L. Load records from file\n");
    printf("Q. Exit program\n");
    printf("=====================================\n");
    printf("Please enter your choice using the letters above: >>> ");
}

/**
 * @brief Clears the standard input buffer.
 */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Adds a new student record.
 */
void addStudent() {
    char addMore;
    do {
        // Check if we need to resize the dynamic array
        if (totalStudents == capacity) {
            int newCapacity = (capacity == 0) ? 10 : capacity * 2; // Start with 10, then double when capacity is filled
            Student *temp = realloc(studentarray, newCapacity * sizeof(Student));

            if (temp == NULL) {
                printf("Error: Memory allocation failed. Cannot add student.\n");
                return;
            }
            studentarray = temp;
            capacity = newCapacity;
        }

        // Get new student details
        int roll;
        printf("\nEnter Student's Roll Number: ");
        if (scanf("%d", &roll) != 1) {
            printf("Invalid input for Roll Number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        // Check if roll number already exists
        if (findStudentByRoll(roll) != -1) {
            printf("\n======================================================");
            printf("\nError: A student with Roll Number %d already exists.\n", roll);
            printf("======================================================\n\n");
            continue;
        }

        studentarray[totalStudents].rollNumber = roll;

        printf("Enter Student's Name: ");
        fgets(studentarray[totalStudents].name, sizeof(studentarray[totalStudents].name), stdin);
        studentarray[totalStudents].name[strcspn(studentarray[totalStudents].name, "\n")] = 0; // Remove newline

        printf("Enter Student's Mark: ");
        if (scanf("%f", &studentarray[totalStudents].marks) != 1) {
            printf("Invalid input for Marks.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();


        printf("============================\n");
        printf("Student added successfully!\n");
        printf("============================\n");


        // Display Pass/Fail status
        const char* status = (studentarray[totalStudents].marks > PASS_THRESHOLD) ? "PASS" : "FAIL";
        printf("Student Status: %s\n", status);
        totalStudents++;
        

        // Ask to add another student ---
        printf("\nAdd another student? (Y/N): >>> ");
        if (scanf(" %c", &addMore) != 1) {
            addMore = 'N'; // Default to No on bad input
        }
        clearInputBuffer();

    } while (toupper(addMore) == 'Y');
}

/**
 * @brief Modifies an existing student's record based on roll number.
 */
void modifyStudent() {
    int roll;
    printf("\nEnter Roll Number of student to modify: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input. Operation Aborted... \n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = findStudentByRoll(roll);
    if (index == -1) {
        printf("\n======================================================");
        printf("\nError: Student with Roll Number %d was not found.\n", roll);
        printf("======================================================\n\n");
        return;
    }
    // Ask for more options on what needs to be modified
    printf("Student Found: %d | %s | %.2f\n", studentarray[index].rollNumber, studentarray[index].name, studentarray[index].marks);
    printf("\nWhat would you like to modify?\n");
    printf("  R - Roll Number\n");
    printf("  N - Name\n");
    printf("  M - Marks\n");
    printf("  C - Cancel\n\n");
    printf("Enter your choice: (R, N, M or C) >>> ");

    char choice;
    if (scanf(" %c", &choice) != 1) {
        printf("\nInvalid input. Aborting modification.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    choice = toupper(choice); 

    switch (choice) {
        case 'R': {
            int newRoll;
            printf("Enter new Roll Number: ");
            if (scanf("%d", &newRoll) != 1) {
                printf("\nInvalid input. Operation Aborted...\n");
                clearInputBuffer();
                return;
            }
            clearInputBuffer();

            // Check if new roll number is already taken
            if (findStudentByRoll(newRoll) != -1) {
                printf("\nError: A student with Roll Number %d already exists. Record unchanged.\n", newRoll);
            } else {
                studentarray[index].rollNumber = newRoll;
                printf("Roll Number updated successfully.\n");
            }
            break;
        }
        case 'N':
            printf("Enter new Name: ");
            fgets(studentarray[index].name, sizeof(studentarray[index].name), stdin);
            studentarray[index].name[strcspn(studentarray[index].name, "\n")] = 0;
            printf("Name updated successfully.\n");
            break;
        case 'M':
            printf("Enter new Mark: ");
            if (scanf("%f", &studentarray[index].marks) != 1) {
                printf("\nInvalid input for Mark. Record unchanged.\n");
                clearInputBuffer();
                return;
            }
            clearInputBuffer();
            printf("Mark updated successfully.\n");
            break;
        case 'C':
            printf("\nModification cancelled.\n");
            break;
        default:
            printf("=======================================\n");
            printf("Invalid choice. Operation Aborted...\n");
            printf("=======================================\n\n");
            break;
    }
}


/**
 * @brief Displays all student records currently in memory.
 */
void displayAllStudents() {
    if (totalStudents == 0) {
        printf("\n================================\n");
        printf("No student records found.\n");
        printf("================================\n");
        return;
    }

    // Ask for sorting preference ---
    int order;
    printf("\nHow will you like the records to be displayed?\n");
    printf(" 1. Sort by Marks (in Ascending Order)\n");
    printf(" 2. Sort by Marks (in Descending Order)\n");
    printf(" 0. Display as is (No sorting)\n");
    printf("Enter choice: ");

    if (scanf("%d", &order) != 1) {
        printf("Invalid input. Defaulting to no sorting.\n");
        order = 0;
    }
    clearInputBuffer();

    // Setup COPIES
    Student *displayArray = studentarray; 
    Student *tempArray = NULL;            

    if (order == 1 || order == 2) {
        tempArray = malloc(totalStudents * sizeof(Student));
        
        if (tempArray != NULL) {
            memcpy(tempArray, studentarray, totalStudents * sizeof(Student));
            
            // Sort the COPY
            if (order == 1) {
                qsort(tempArray, totalStudents, sizeof(Student), compareAsc);
                printf("\n(Displaying sorted by Marks: Ascending)\n");
            } else {
                qsort(tempArray, totalStudents, sizeof(Student), compareDesc);
                printf("\n(Displaying sorted by Marks: Descending)\n");
            }
            
            displayArray = tempArray;
        } else {
            printf("\n - Displaying records as-is - \n");
        }
    } else {
        printf("\n(Displaying records as-is)\n");
    }

    printf("\n--- Displaying All Student Records ---\n");
    printf("----------------------------------------------------------\n");
    printf("Roll No. | Name                   | Marks  | Status\n");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < totalStudents; i++) {
        const char* status = (displayArray[i].marks > PASS_THRESHOLD) ? "Pass" : "Fail";
        printf("%-8d | %-22s | %-6.2f | %s\n",
               displayArray[i].rollNumber,
               displayArray[i].name,
               displayArray[i].marks,
               status);
    }
    printf("----------------------------------------------------------\n\n");

    if (tempArray != NULL) {
        free(tempArray);
    }
}

/**
 * @brief Searches for a student by roll number and displays their details.
 */
void searchStudent() {
    int roll;
    printf("\nEnter Roll Number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("===============\n");
        printf("Invalid input.\n");
        printf("===============\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = findStudentByRoll(roll);
    if (index == -1) {
        printf("==================================================\n");
        printf("Error: Student with Roll Number %d was not found.\n", roll);
        printf("==================================================\n");
        return;
    }

    printf("\n--- Student Found ---\n");
    printf("Roll Number: %d\n", studentarray[index].rollNumber);
    printf("Name:        %s\n", studentarray[index].name);
    printf("Marks:       %.2f\n", studentarray[index].marks);
    const char* status = (studentarray[index].marks > PASS_THRESHOLD) ? "Pass" : "Fail";
    printf("Status:      %s\n", status);
    printf("----------------------\n");
}

/**
 * @brief Removes a student record by roll number.
 */
void deleteStudent() {
    int roll;
    printf("\nEnter Roll Number of student to remove: ");
    if (scanf("%d", &roll) != 1) {
        printf("==============\n");
        printf("Invalid input.\n");
        printf("==============\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = findStudentByRoll(roll);
    if (index == -1) {
        printf("==================================================\n");
        printf("Error: Student with Roll Number %d was not found.\n", roll);
        printf("==================================================\n");
        return;
    }

    // Shift all subsequent elements one position to the left
    for (int i = index; i < totalStudents - 1; i++) {
        studentarray[i] = studentarray[i + 1];
    }

    totalStudents--;
    printf("=====================================\n");
    printf("Student record removed successfully.\n");
    printf("=====================================\n");

    // Shrink the array if it's very empty
    if (totalStudents > 0 && totalStudents < capacity / 4) {
        capacity = capacity / 2;
        Student *temp = realloc(studentarray, capacity * sizeof(Student));
        if (temp != NULL) {
            studentarray = temp;
        }
    }
}

/**
 * @brief Calculates and displays the average marks of all studentarray.
 */
void calculateAverage() {
    if (totalStudents == 0) {
        printf("\n======================================\n");
        printf("No Students to calculate average for.\n");
        printf("======================================\n");
        return;
    }

    double totalMarks = 0;
    for (int i = 0; i < totalStudents; i++) {
        totalMarks += studentarray[i].marks;
    }
    printf("\n===========================================\n");
    printf("Average marks for %d studentarray: %.2f\n", totalStudents, totalMarks / totalStudents);
    printf("===========================================\n");
}

/**
 * @brief Comparison function for qsort (ascending order).
 */
int compareAsc(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    if (s1->marks < s2->marks) return -1;
    if (s1->marks > s2->marks) return 1;
    return 0;
}

/**
 * @brief Comparison function for qsort (descending order).
 */
int compareDesc(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    if (s1->marks > s2->marks) return -1;
    if (s1->marks < s2->marks) return 1;
    return 0;
}

/**
 * @brief Saves all student records to a text file.
 */
void saveToFile() {
    char filename[100]; 

    printf("\nEnter the filename to save (leave empty to cancel): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    if (strlen(filename) == 0) {
        printf("\nNo filename entered. Save Operation cancelled.\n");
        return;
    }

    // Open the file for writing
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("\nError: Could not open file %s for writing.\n", filename);
        return;
    }

    // Write the total number of students first
    fprintf(f, "%d\n", totalStudents);
    // Write each student record
    for (int i = 0; i < totalStudents; i++) {
        fprintf(f, "%s\n", studentarray[i].name);
        fprintf(f, "%d\n", studentarray[i].rollNumber);
        fprintf(f, "%f\n", studentarray[i].marks);
    }

    fclose(f);
    printf("\n====================================================\n");
    printf("Successfully saved %d records to %s.\n", totalStudents, filename);
    printf("====================================================\n");
}


/**
 * @brief Loads student records from a text file.
 */
void loadFromFile() {
    char filename[100];

    printf("Enter the filename to load (leave empty to cancel): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strlen(filename) == 0) {
        printf("No filename entered. Load cancelled.\n");
        return;
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("=============================================\n");
        printf("Error: File (%s) not found.\n", filename);
        printf("=============================================\n");
        return;
    }

    int loadedCount = 0;
    if (fscanf(f, "%d\n", &loadedCount) != 1 || loadedCount < 0) {
        printf("==========================================================================================\n");
        printf("Error reading record count from file. Kindly check that the file is in the correct format\n");
        printf("==========================================================================================\n");
        fclose(f);
        return;
    }
    if (loadedCount == 0) {
        printf("==============\n");
        printf("File is empty.\n");
        printf("==============\n");
        fclose(f);
        return;
    }

    // Allocate memory
    int newCapacity = totalStudents + loadedCount;
    Student *temp = (Student *)realloc(studentarray, newCapacity * sizeof(Student));
    if (temp == NULL) {
        printf("\nError: Memory allocation failed while loading. Aborting load.\n");
        fclose(f);
        return;
    }
    studentarray = temp;
    capacity = newCapacity;

    int actuallyLoaded = 0;
    for (int i = 0; i < loadedCount; i++) {
        int currentIndex = totalStudents;

        if (fgets(studentarray[currentIndex].name, sizeof(studentarray[currentIndex].name), f) == NULL) break;
        studentarray[currentIndex].name[strcspn(studentarray[currentIndex].name, "\n")] = 0;

        int tempRoll;
        if (fscanf(f, "%d\n", &tempRoll) != 1) break;

        // Check for duplicates
        if (tempRoll != -1 && findStudentByRoll(tempRoll) != -1) {
            printf("Duplicate Roll Number detected for Roll Number %d. Assigning -1.\n", tempRoll);
            tempRoll = -1;
        }
        studentarray[currentIndex].rollNumber = tempRoll;

        if (fscanf(f, "%f\n", &studentarray[currentIndex].marks) != 1) break;

        totalStudents++;
        actuallyLoaded++;
    }

    fclose(f);

    if(actuallyLoaded != loadedCount) {
        printf("Warning: File may be corrupt. Loaded %d out of %d expected records.\n", actuallyLoaded, loadedCount);
    } else {
        printf("=============================================================================================\n");
        printf("Successfully loaded %d records from %s.\n", actuallyLoaded, filename);
        printf("=============================================================================================\n");
    }
}

/**
 * @brief To find a student's index by roll number.
 */
int findStudentByRoll(int roll) {
    for (int i = 0; i < totalStudents; i++) {
        if (studentarray[i].rollNumber == roll) {
            return i; // Found it
        }
    }
    return -1; // Not found
}