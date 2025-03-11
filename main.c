#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MAX_COURSE_LEN 50

// Structure to hold student details
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int age;
    char course[MAX_COURSE_LEN];
    float marks;
} Student;

// Function declarations
void addStudent(FILE *file);
void displayStudents(FILE *file);
void modifyStudent(FILE *file);
void deleteStudent(FILE *file);
int findStudent(FILE *file, int id);

int main() {
    FILE *file = fopen("students.dat", "r+b");
    if (!file) {
        file = fopen("students.dat", "w+b");
    }

    int choice;

    while (1) {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Modify Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(file);
                break;
            case 2:
                displayStudents(file);
                break;
            case 3:
                modifyStudent(file);
                break;
            case 4:
                deleteStudent(file);
                break;
            case 5:
                fclose(file);
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void addStudent(FILE *file) {
    Student newStudent;

    printf("\nEnter student ID: ");
    scanf("%d", &newStudent.id);
    getchar(); // to capture newline character after entering id

    printf("Enter student name: ");
    fgets(newStudent.name, MAX_NAME_LEN, stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = 0; // Remove newline character

    printf("Enter student age: ");
    scanf("%d", &newStudent.age);

    getchar(); // to capture newline character after entering age

    printf("Enter course name: ");
    fgets(newStudent.course, MAX_COURSE_LEN, stdin);
    newStudent.course[strcspn(newStudent.course, "\n")] = 0;

    printf("Enter student marks: ");
    scanf("%f", &newStudent.marks);

    fseek(file, 0, SEEK_END);
    fwrite(&newStudent, sizeof(Student), 1, file);
    printf("Student added successfully!\n");
}

void displayStudents(FILE *file) {
    Student student;

    rewind(file);
    printf("\nStudent List:\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("ID: %d\n", student.id);
        printf("Name: %s\n", student.name);
        printf("Age: %d\n", student.age);
        printf("Course: %s\n", student.course);
        printf("Marks: %.2f\n", student.marks);
        printf("---------------------------------\n");
    }
}

void modifyStudent(FILE *file) {
    int id;
    printf("\nEnter student ID to modify: ");
    scanf("%d", &id);

    int pos = findStudent(file, id);
    if (pos == -1) {
        printf("Student with ID %d not found!\n", id);
        return;
    }

    Student student;
    fseek(file, pos * sizeof(Student), SEEK_SET);
    fread(&student, sizeof(Student), 1, file);

    getchar(); // to capture newline character after reading id

    printf("Enter new name: ");
    fgets(student.name, MAX_NAME_LEN, stdin);
    student.name[strcspn(student.name, "\n")] = 0;

    printf("Enter new age: ");
    scanf("%d", &student.age);

    getchar(); // to capture newline character after entering age

    printf("Enter new course: ");
    fgets(student.course, MAX_COURSE_LEN, stdin);
    student.course[strcspn(student.course, "\n")] = 0;

    printf("Enter new marks: ");
    scanf("%f", &student.marks);

    fseek(file, pos * sizeof(Student), SEEK_SET);
    fwrite(&student, sizeof(Student), 1, file);
    printf("Student details updated successfully!\n");
}

void deleteStudent(FILE *file) {
    int id;
    printf("\nEnter student ID to delete: ");
    scanf("%d", &id);

    int pos = findStudent(file, id);
    if (pos == -1) {
        printf("Student with ID %d not found!\n", id);
        return;
    }

    Student temp;
    FILE *tempFile = fopen("temp.dat", "w+b");

    rewind(file);
    while (fread(&temp, sizeof(Student), 1, file)) {
        if (temp.id != id) {
            fwrite(&temp, sizeof(Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    file = fopen("students.dat", "r+b");
    printf("Student with ID %d deleted successfully!\n", id);
}

int findStudent(FILE *file, int id) {
    Student student;
    int pos = 0;

    rewind(file);
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            return pos;
        }
        pos++;
    }
    return -1;
}
