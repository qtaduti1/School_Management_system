// we have imported a header file of raylib
// and used functions like strcmp sprintf so we imported string.h file also
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// we have speciied the window size in which our contents will be displayed

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
// below are for making a grid in which we can show students in a grid
#define MAX_ROWS 15
#define MAX_COLS 5
#define CELL_WIDTH 150
#define CELL_HEIGHT 30
#define GRID_X_OFFSET 200
#define GRID_Y_OFFSET 200
#define RECTANGLE_WIDTH (WINDOW_WIDTH - 100)
#define RECTANGLE_HEIGHT (WINDOW_HEIGHT - 100)

#define RECTANGLE_THICKNESS 10
// basically we have defined two kinds of structs:
//  1)storing student info
//  2)for storing info about the students courses....

struct StudentInfo
{
    char ID[10];
    char Name[20];
    char Email[30];
    char Phone[20];
    int NumberOfCourse;
};

struct CourseInfo
{
    char StudentID[10];
    char Code[10];
    char Name[20];
};

struct StudentInfo Students[100];
struct CourseInfo Courses[500];
int TotalStudents = 0;
int TotalCourse = 0;
char data[MAX_ROWS][MAX_COLS][30];
Texture2D total_students_texture;

// here we have a enum of functions
// could have used structs also...
typedef enum
{
    MAIN_MENU,
    NEW_STUDENT,
    SHOW_STUDENTS,
    SEARCH_STUDENT,
    EDIT_STUDENT,
    DELETE_STUDENT,
    DELETEALL_STUDENT,
    TOTAL_STUDENTS,
    USER_GUIDELINE,
    ABOUT_US,
    EXIT

} MetaState;

// const char* data[15][5];
int BOX_WIDTH = 300;
int BOX_HEIGHT = 200;

void Dummydata();

void Menu(MetaState *meta_state);
int AddNewStudent(MetaState *meta_state);
int IsAlreadyExists(char GivenLine[300], char InfoType, char StudentID[300]);
void showStudents(MetaState *meta_state);
void EditStudent(int StudentFoundIndex);
void DeleteStudentByIndex(int CourseIndex);
void DeleteCourseByIndex(int CourseIndex);
void DeleteStudent(int StudentIndex);
void total_students(MetaState *meta_state);
void DeleteAllStudents(MetaState *meta_state);
int SearchStudent(MetaState *meta_state, char *StudentID);
void UserGuideline(MetaState *meta_state);
void AboutUs(MetaState *meta_state);
void Dummydata();
// here is the dummy data

void Dummydata()
{

    // Student data 1
    strcpy(Students[0].ID, "1835355");
    strcpy(Students[0].Name, "Student 1");
    strcpy(Students[0].Phone, "9342687263");
    Students[0].NumberOfCourse = 2;
    strcpy(Students[0].Email, "student1@gmail.com");
    TotalStudents++;
    strcpy(Courses[0].StudentID, "1835355");
    strcpy(Courses[0].Code, "CSE-1");
    strcpy(Courses[0].Name, "Course - 1");
    strcpy(Courses[1].StudentID, "1835355");
    strcpy(Courses[1].Code, "MA101");
    strcpy(Courses[1].Name, "Mathematics");
    TotalCourse += 2;

    // Student data 2
    strcpy(Students[1].ID, "5309164");
    strcpy(Students[1].Name, "Student 2");
    strcpy(Students[1].Phone, "7892357504");
    Students[1].NumberOfCourse = 1;
    strcpy(Students[1].Email, "student2@gmail.com");
    TotalStudents++;
    strcpy(Courses[2].StudentID, "5309164");
    strcpy(Courses[2].Code, "CSE-1");
    strcpy(Courses[2].Name, "Course - 1");
    TotalCourse++;

    // Student data 3
    strcpy(Students[2].ID, "5318584");
    strcpy(Students[2].Name, "Student 3");
    strcpy(Students[2].Phone, "8372664378");
    Students[2].NumberOfCourse = 1;
    strcpy(Students[2].Email, "student3@gmail.com");
    TotalStudents++;
    strcpy(Courses[3].StudentID, "5318584");
    strcpy(Courses[3].Code, "CSE-1");
    strcpy(Courses[3].Name, "Course - 1");
    TotalCourse++;
}

int main(void) // krishna
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Student management system IITPATNA");

    SetTargetFPS(60);
    total_students_texture = LoadTexture("iitpatna.png");

    MetaState meta_state = MAIN_MENU;
    char StdID[10];
    Dummydata();
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();

        switch (meta_state)
        {
        case MAIN_MENU:
            Menu(&meta_state);
            break;
        case NEW_STUDENT:
            AddNewStudent(&meta_state);
            break;

        case SHOW_STUDENTS:
            showStudents(&meta_state);
            break;

        case SEARCH_STUDENT:
            printf("Enter the ID of the Student:");
            scanf("%s", StdID);
            int Index = SearchStudent(&meta_state, StdID);
            if (Index == -1)
            {
                printf(" No such Student Exists");
            }
            break;

        case EDIT_STUDENT:
            printf("Enter the ID of the Student:");
            scanf("%s", StdID);
            int Index1 = SearchStudent(&meta_state, StdID);
            EditStudent(Index1);
            break;

        case DELETE_STUDENT:
            printf("Enter the ID of the Student:");
            scanf("%s", StdID);
            int Index2 = SearchStudent(&meta_state, StdID);
            DeleteStudent(Index2);
            break;

        case DELETEALL_STUDENT:
            DeleteAllStudents(&meta_state);
            break;

        case TOTAL_STUDENTS:
            // here will be the function
            total_students(&meta_state);
            break;

        case USER_GUIDELINE:
            UserGuideline(&meta_state);
            break;

        case ABOUT_US:
            AboutUs(&meta_state);
            break;

        case EXIT:
            ClearBackground(RAYWHITE);
            // DrawText(const char *text, int posX, int posY, int fontSize, Color color);
            DrawText("ARE YOU SURE YOU WANT TO LEAVE {Y/N}??", 50, 300, 50, RED);

            if (IsKeyPressed(KEY_Y))
            {
                return 0;
            }

            else if (IsKeyPressed(KEY_N))
            {
                meta_state = MAIN_MENU;
                // Menu(&meta_state);
            }
            break;
        }
        EndDrawing();
    }
    UnloadTexture(total_students_texture);
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

// the below given function ensures that if we are entering
// an already existing students with same details such as
// phone,email etc so it will throw a msg and notify that about it....

int IsAlreadyExists(char GivenLine[300], char InfoType, char StudentID[300]) // ashish
{
    int EmailExists = 0;
    int PhoneExists = 0;
    int IDExists = 0;
    int ep;

    for (ep = 0; ep < TotalStudents; ep++)
    {
        if (strcmp(GivenLine, Students[ep].ID) == 0)
        {
            IDExists++;
        }
        if (strcmp(GivenLine, Students[ep].Email) == 0 && strcmp(StudentID, Students[ep].ID) != 0)
        {
            EmailExists++;
        }
        if (strcmp(GivenLine, Students[ep].Phone) == 0 && strcmp(StudentID, Students[ep].ID) != 0)
        {
            PhoneExists++;
        }
    }

    if (InfoType == 'i')
    {
        return IDExists;
    }
    else if (InfoType == 'e')
    {
        return EmailExists;
    }
    else if (InfoType == 'p')
    {
        return PhoneExists;
    }
    else
    {
        return 0;
    }
}

// this is our menu function which is our original page....
// all functions provided will be displayed here

void Menu(MetaState *meta_state) // krishna,srichetan
{
    ClearBackground(RAYWHITE);
    const char *heading = "WELCOME TO IITPATNA";
    int headingWidth = MeasureText(heading, 40);
    DrawText(heading, WINDOW_WIDTH / 2 - headingWidth / 2, 20, 40, BLACK);

    // Draw main menu box
    DrawRectangleLines(WINDOW_WIDTH / 4, 120, WINDOW_WIDTH / 2, 40, BLACK);

    // Draw main menu text
    const char *menuText = "Main Menu";
    int menuWidth = MeasureText(menuText, 30);
    DrawText(menuText, WINDOW_WIDTH / 2 - menuWidth / 2, 125, 30, BLACK);

    DrawText("How may we assist you today??", 300, 220, 40, BLACK);

    // now we add the options.....
    DrawText("PRESS:", 50, 300, 40, BLACK);
    DrawText("[1] To Add A New student. ", 50, 340, 20, BLACK);
    DrawText("[2] To Show All students.  ", 50, 380, 20, BLACK);
    DrawText("[3] To Search A student.", 50, 420, 20, BLACK);
    DrawText("[4] To Edit A student.", 50, 460, 20, BLACK);
    DrawText("[5] To Delete A student.  ", 50, 500, 20, BLACK);
    DrawText("[6] To  Delete All students.", 50, 540, 20, BLACK);
    DrawText("[7] To check the number of students ", 50, 580, 20, BLACK);
    DrawText("[8] To check the User Guideline.", 50, 620, 20, BLACK);
    DrawText("[9] To know more About Us.", 50, 660, 20, BLACK);
    DrawText("[0] To Exit the Program.   ", 50, 700, 20, BLACK);

    // NOTE:
    // getkeypressed function waits for the user to press a key
    // and on its basis it will go into a switch case statement and change the value of enum

    // for example in our int main function we have assigned meta_state as MAIN_MENU

    // but as this function gets executed so getkeypressed will change meta_state on the

    // basis of switch case statements

    switch (GetKeyPressed())
    {
    case KEY_ONE:
        *meta_state = NEW_STUDENT;
        break;

    case KEY_TWO:
        *meta_state = SHOW_STUDENTS;
        break;

    case KEY_THREE:
        *meta_state = SEARCH_STUDENT; // going to remove this function....
        break;

    case KEY_FOUR:
        *meta_state = EDIT_STUDENT;
        break;

    case KEY_FIVE:
        *meta_state = DELETE_STUDENT;
        break;

    case KEY_SIX:
        *meta_state = DELETEALL_STUDENT;
        break;

    case KEY_SEVEN:
        *meta_state = TOTAL_STUDENTS;
        break;

    case KEY_EIGHT:
        *meta_state = USER_GUIDELINE;
        break;

    case KEY_NINE:
        *meta_state = ABOUT_US;
        break;

    case KEY_ZERO:
        *meta_state = EXIT;
        break;
    }
}

// this function was written by aditivyas and yatarth

void DeleteStudentByIndex(int CourseIndex)
{
    int s;
    for (s = 0; s < TotalStudents - 1; s++)
    {
        if (s >= CourseIndex)
        {
            Students[s] = Students[s + 1];
        }
    }
    TotalStudents--;
}

void DeleteCourseByIndex(int CourseIndex) // anvithapr,yatarth
{
    int c;
    for (c = 0; c < TotalCourse - 1; c++)
    {
        if (c >= CourseIndex)
        {
            Courses[c] = Courses[c + 1];
        }
    }
    TotalCourse--;
}

int AddNewStudent(MetaState *meta_state) // bibhuti//aryaprakash
{
    ClearBackground(RAYWHITE);

    DrawRectangle(50, 50, WINDOW_WIDTH - 100, 80, GRAY);
    DrawRectangleLines(50, 50, WINDOW_WIDTH - 100, 80, BLACK);

    // Draw title text
    DrawText("Adding a new student", 80, 70, 30, BLACK);

    DrawText("Following details will be required:", 40, 200, 30, BLACK);
    // DrawText("Enter ID:",40,300,30,BLACK);

    char StudentID[300];
    char Name[300];
    char Phone[300];
    char Email[300];
    int NumberOfCourses;
    char CourseCode[300];
    char CourseName[300];

    int IsValidID = 0;
    while (!IsValidID)
    {
        printf(" Enter The ID: ");
        scanf("%s", StudentID);
        // data[TotalStudents][0]=StudentID;
        if (IsAlreadyExists(StudentID, 'i', StudentID) > 0)
        {
            printf(" Error: This ID is already exists.\n\n");
            IsValidID = 0;
        }
        else if (strlen(StudentID) > 10)
        {
            printf(" Error: ID can not be more than 10 characters.\n\n");
            IsValidID = 0;
        }
        else if (strlen(StudentID) <= 0)
        {
            printf(" Error: ID can not be empty.\n\n");
            IsValidID = 0;
        }
        else
        {
            IsValidID = 1;
        }
    }

    int IsValidName = 0;
    while (!IsValidName)
    {
        printf(" Enter The Name: ");
        scanf(" %[^\n]s", Name);
        //  data[TotalStudents][1]=Name;
        if (strlen(Name) > 20)
        {
            printf(" Error: Name can not be more than 20 characters.\n\n");
            IsValidName = 0;
        }
        if (strlen(Name) <= 0)
        {
            printf(" Error: Name can not be empty.\n\n");
            IsValidName = 0;
        }
        else
        {
            IsValidName = 1;
        }
    }

    int IsValidEmail = 0;
    while (!IsValidEmail)
    {
        printf(" Enter The Email: ");
        scanf("%s", Email);
        //  data[TotalStudents][2]=Email;
        if (IsAlreadyExists(Email, 'e', StudentID) > 0)
        {
            printf(" This Email is Already Exists.\n");
            IsValidEmail = 0;
        }
        else if (strlen(Email) > 30)
        {
            printf(" Error: Email can not be more than 30 characters.\n\n");
            IsValidEmail = 0;
        }
        else if (strlen(Email) <= 0)
        {
            printf(" Error: Email can not be empty.\n\n");
            IsValidEmail = 0;
        }
        else
        {
            IsValidEmail = 1;
        }
    }

    int IsValidPhone = 0;
    while (!IsValidPhone)
    {
        printf(" Enter The Phone: ");
        scanf("%s", Phone);
        // data[TotalStudents][3]=Phone;
        if (IsAlreadyExists(Phone, 'p', StudentID) > 0)
        {
            printf(" This Phone Number is Already Exists\n");
            IsValidPhone = 0;
        }
        else if (strlen(Phone) > 20)
        {
            printf(" Error: Phone can not be more than 20 characters.\n\n");
            IsValidPhone = 0;
        }
        else if (strlen(Phone) <= 0)
        {
            printf(" Error: Phone can not be empty.\n\n");
            IsValidPhone = 0;
        }
        else
        {
            IsValidPhone = 1;
        }
    }

    int IsValidNumberOfCourse = 0;
    while (!IsValidNumberOfCourse)
    {
        printf(" Number of courses: ");
        scanf("%d", &NumberOfCourses);
        // char x=(char)NumberOfCourses;
        // data[TotalStudents][4]=x;
        if (NumberOfCourses <= 0 || NumberOfCourses > 4)
        {
            printf(" Error: Number of courses can not be more than 4 and lees than 1.\n\n");
            IsValidNumberOfCourse = 0;
        }
        else
        {
            IsValidNumberOfCourse = 1;
        }
    }

    strcpy(Students[TotalStudents].ID, StudentID);
    strcpy(Students[TotalStudents].Name, Name);
    strcpy(Students[TotalStudents].Phone, Phone);
    strcpy(Students[TotalStudents].Email, Email);
    Students[TotalStudents].NumberOfCourse = NumberOfCourses;
    TotalStudents++;

    for (int i = 0; i < NumberOfCourses; i++)
    {

        printf(" Enter Course %d Code: ", i + 1);
        scanf("%s", CourseCode);

        printf(" Enter Course %d Name: ", i + 1);
        scanf(" %[^\n]s", CourseName);

        strcpy(Courses[TotalCourse].StudentID, StudentID);
        strcpy(Courses[TotalCourse].Code, CourseCode);
        strcpy(Courses[TotalCourse].Name, CourseName);
        TotalCourse++;
    }

    printf("\n Student Added Successfully.\n\n");

    // Draw main menu box
    // DrawRectangleLines(WINDOW_WIDTH/4,120,WINDOW_WIDTH/2,40,BLACK);
    *meta_state = MAIN_MENU;
    return 0;
}

void showStudents(MetaState *meta_state) // srichetan,krishna
{
    ClearBackground(RAYWHITE);

    DrawRectangle(50, 50, WINDOW_WIDTH - 100, 80, GRAY);
    DrawRectangleLines(50, 50, WINDOW_WIDTH - 100, 80, BLACK);

    // Draw title text
    DrawText("showing all students", 80, 70, 30, BLACK);
    DrawText("showing all students", 80, 70, 30, BLACK);

    int cellX = GRID_X_OFFSET;
    int cellY = GRID_Y_OFFSET - CELL_HEIGHT;
    // Draw cell border
    DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

    // Draw cell text
    DrawText("StudentID", cellX + 5, cellY + 5, 20, BLACK);
    cellX+=CELL_WIDTH;
    // Draw cell border
    DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

    // Draw cell text
    DrawText("Name", cellX + 5, cellY + 5, 20, BLACK);
    cellX+=CELL_WIDTH;
    // Draw cell border
    DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

    // Draw cell text
    DrawText("Phone No.", cellX + 5, cellY + 5, 20, BLACK);
    cellX+=CELL_WIDTH;
    // Draw cell border
    DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

    // Draw cell text
    DrawText("No.of Courses", cellX + 5, cellY + 5, 20, BLACK);
    cellX=cellX+CELL_WIDTH;
    // Draw cell border
    DrawRectangleLines(cellX, cellY, CELL_WIDTH+100, CELL_HEIGHT, BLACK);

    // Draw cell text
    DrawText("Email ID", cellX + 5, cellY + 5, 20, BLACK);

    for (int row = 0; row < TotalStudents; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {

            // Calculate cell position
            if (col == 0)
            {
                strcpy(data[row][col], Students[row].ID);
                int cellX = GRID_X_OFFSET + col * CELL_WIDTH;
                int cellY = GRID_Y_OFFSET + row * CELL_HEIGHT;

                // Draw cell border
                DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

                // Draw cell text
                DrawText(data[row][col], cellX + 5, cellY + 5, 20, BLACK);
            }
            if (col == 1)
            {
                strcpy(data[row][col], Students[row].Name);
                int cellX = GRID_X_OFFSET + col * CELL_WIDTH;
                int cellY = GRID_Y_OFFSET + row * CELL_HEIGHT;

                // Draw cell border
                DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

                // Draw cell text
                DrawText(data[row][col], cellX + 5, cellY + 5, 20, BLACK);
            }
            if (col == 2)
            {
                strcpy(data[row][col], Students[row].Phone);
                int cellX = GRID_X_OFFSET + col * CELL_WIDTH;
                int cellY = GRID_Y_OFFSET + row * CELL_HEIGHT;

                // Draw cell border
                DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

                // Draw cell text
                DrawText(data[row][col], cellX + 5, cellY + 5, 20, BLACK);
            }
            if (col == 3)
            {
                sprintf(data[row][col], "%d", Students[row].NumberOfCourse);
                int cellX = GRID_X_OFFSET + col * CELL_WIDTH;
                int cellY = GRID_Y_OFFSET + row * CELL_HEIGHT;

                // Draw cell border
                DrawRectangleLines(cellX, cellY, CELL_WIDTH, CELL_HEIGHT, BLACK);

                // Draw cell text
                DrawText(data[row][col], cellX + 60, cellY+5 , 20, BLACK);
            }
            if (col == 4)
            {
                strcpy(data[row][col], Students[row].Email);

                int cellX = GRID_X_OFFSET + col * CELL_WIDTH;
                int cellY = GRID_Y_OFFSET + row * CELL_HEIGHT;

                // Draw cell border
                DrawRectangleLines(cellX, cellY, CELL_WIDTH + 100, CELL_HEIGHT, BLACK);

                // Draw cell text
                DrawText(data[row][col], cellX + 5, cellY + 5, 20, BLACK);
            }
        }
    }

    DrawText("To go back to main menu press (b)", 80, 700, 30, BLACK);

    // printf("\n");
    switch (GetKeyPressed())
    {
    case KEY_B:
        *meta_state = MAIN_MENU;
        break;
    }
}

void AboutUs(MetaState *meta_state) // krishna,srichetan
{
    ClearBackground(RAYWHITE);
    // DrawRectangle(500,500,10,100,BLACK);
    int posX = (WINDOW_WIDTH - RECTANGLE_WIDTH) / 2;
    int posY = (WINDOW_HEIGHT - RECTANGLE_HEIGHT) / 2;

    // Draw the rectangle outline
    DrawRectangleLinesEx((Rectangle){posX, posY, RECTANGLE_WIDTH, RECTANGLE_HEIGHT}, RECTANGLE_THICKNESS, BLACK);
    DrawRectangleLinesEx((Rectangle){posX + 40, posY + 40, RECTANGLE_WIDTH - 40, RECTANGLE_HEIGHT - 40}, RECTANGLE_THICKNESS, BLACK);
    // DrawRectangle(int posX, int posY, int width, int height, Color color);
    DrawText("These were the part of this project", 120, 150, 30, BLACK);

    DrawText("Bibhuti Jha", 140, 200, 30, BLACK);
    DrawText("Hamid", 140, 250, 30, BLACK);
    DrawText("Aditi Vyas", 140, 300, 30, BLACK);
    DrawText("Yash Vardhan Tyoharia", 140, 350, 30, BLACK);
    DrawText("Anvitha", 140, 400, 30, BLACK);
    DrawText("Yatharth Gupta", 140, 450, 30, BLACK);
    DrawText("Krishna Gupta", 140, 500, 30, BLACK);
    DrawText("G Srichetan Reddy", 140, 550, 30, BLACK);
    DrawText("Arya Prakash", 140, 600, 30, BLACK);
    DrawText("Ashish raj", 140, 650, 30, BLACK);
    DrawText("To go back to main menu press (b)", 120, 700, 30, BLACK);
    switch (GetKeyPressed())
    {
    case KEY_B:
        *meta_state = MAIN_MENU;
        break;
    }
}

void UserGuideline(MetaState *meta_state) // krishna,srichetan
{

    ClearBackground(RAYWHITE);
    DrawText(" **** How it Works? ****", 100, 200, 30, BLACK);
    DrawText("-> You will only able to store the Student's ID, Name etc", 20, 250, 30, BLACK);
    DrawText("-> A student can have maximum 4 courses and minimum 1 course. ", 20, 300, 30, BLACK);
    DrawText("-> Student ID can be maximum 10 characters long and unique.", 20, 350, 30, BLACK);
    DrawText("-> Student Name can be maximum 12 characters long.", 20, 400, 30, BLACK);
    DrawText("-> Student Email can be maximum 20 characters long and unique.", 20, 450, 30, BLACK);
    DrawText("-> Student Phone can be maximum 10 characters long and unique.", 20, 500, 30, BLACK);
    DrawText("-> Course code can be maximum 10 characters long. ", 20, 550, 30, BLACK);
    DrawText("-> Course Name can be maximum 20 characters long.", 20, 600, 30, BLACK);

    DrawText("To go back to main menu press (b)", 80, 700, 30, BLACK);
    switch (GetKeyPressed())
    {
    case KEY_B:
        *meta_state = MAIN_MENU;
        break;
    }
}

int SearchStudent(MetaState *meta_state, char *StudentID) // arya,aditivyas
{
    int StudentFoundIndex = -1;

    int i;
    for (i = 0; i < TotalStudents; i++)
    {
        if (strcmp(StudentID, Students[i].ID) == 0)
        {
            StudentFoundIndex = i;
            printf("\n One Student Found for ID: %s\n\n", StudentID);
            printf(" Student Informations\n");
            printf("-------------------------\n");

            printf(" ID:    %s\n", Students[i].ID);
            printf(" Name:  %s\n", Students[i].Name);
            printf(" Email: %s\n", Students[i].Email);
            printf(" Phone: %s\n", Students[i].Phone);
            printf("\n Total Number of Courses: %d\n", Students[i].NumberOfCourse);
        }
    }
    int CourseCount = 0;
    int j;
    for (j = 0; j < TotalCourse; j++)
    {
        if (strcmp(StudentID, Courses[j].StudentID) == 0)
        {
            CourseCount++;
            printf(" Course %d Code: %s\n", CourseCount, Courses[j].Code);
            printf(" Course %d Name: %s\n", CourseCount, Courses[j].Name);
        }
    }
    *meta_state = MAIN_MENU;
    return StudentFoundIndex;
}

void DeleteAllStudents(MetaState *meta_state) // hamid
{

    ClearBackground(RAYWHITE);
    DrawText("ARE YOU SURE YOU WANT TO", 20, 300, 50, RED);
    DrawText(" DELETE ALL THE DATA{Y/N}??", 20, 500, 50, RED);
    DrawText("To go back to main menu press (b)", 80, 700, 30, BLACK);
    switch (GetKeyPressed())
    {
    case KEY_B:
        *meta_state = MAIN_MENU;
        break;

    case KEY_Y:
        if (TotalStudents == 0)
        {
            printf("No students left to delete\n");
        }
        else
        {
            TotalCourse = 0;
            TotalStudents = 0;
            printf("All student data has been deleted");
        }
        *meta_state = MAIN_MENU;
        break;
    }
}

void EditStudent(int StudentFoundIndex) // bibhuti,ashish
{
    // NOTE:
    // We are not changing the Meta State here as we are changing it int Search student function passing pointer
    if (StudentFoundIndex == -1)
    {
        printf("\nNo Such Student exists\n\n");
    }
    else
    {
        printf("\n\t\t **** Update The New Student ****\n\n");

        char NewName[300];
        char NewPhone[300];
        char NewEmail[300];
        int NewNumberOfCourses;
        char StudentID[300];
        char CourseCode[100];
        char CourseName[100];
        strcpy(StudentID, Students[StudentFoundIndex].ID);

        int IsValidName = 0;
        while (!IsValidName)
        {
            printf(" Enter The New Name(0 for skip): ");
            scanf(" %[^\n]s", NewName);
            if (strlen(NewName) > 20)
            {
                printf(" Error: Name can not be more than 20 characters.\n\n");
                IsValidName = 0;
            }
            else if (strlen(NewName) <= 0)
            {
                printf(" Error: Name can not be empty.\n\n");
                IsValidName = 0;
            }
            else
            {
                IsValidName = 1;
            }
        }

        int IsValidEmail = 0;
        while (!IsValidEmail)
        {
            printf(" Enter The New Email(0 for skip): ");
            scanf("%s", NewEmail);

            if (strlen(NewEmail) > 30)
            {
                printf(" Error: Email can not be more than 30 characters.\n\n");
                IsValidEmail = 0;
            }
            else if (strlen(NewEmail) <= 0)
            {
                printf(" Error: Email can not be empty.\n\n");
                IsValidEmail = 0;
            }
            else if (IsAlreadyExists(NewEmail, 'e', StudentID) > 0)
            {
                printf(" Error: This Email Already Exists.\n\n");
                IsValidEmail = 0;
            }
            else
            {
                IsValidEmail = 1;
            }
        }

        int IsValidPhone = 0;
        while (!IsValidPhone)
        {
            printf(" Enter The New Phone(0 for skip): ");
            scanf("%s", NewPhone);

            if (strlen(NewPhone) > 20)
            {
                printf(" Error: Phone can not be more than 20 characters.\n\n");
                IsValidPhone = 0;
            }
            else if (strlen(NewPhone) <= 0)
            {
                printf(" Error: Phone can not be empty.\n\n");
                IsValidPhone = 0;
            }
            else if (IsAlreadyExists(NewPhone, 'p', StudentID) > 0)
            {
                printf(" Error: This Phone Number is Already Exists.\n\n");
                IsValidPhone = 0;
            }
            else
            {
                IsValidPhone = 1;
            }
        }

        int IsValidNumberOfCourse = 0;
        while (IsValidNumberOfCourse == 0)
        {
            printf(" Number of New courses(0 for skip): ");
            scanf("%d", &NewNumberOfCourses);

            if (NewNumberOfCourses > 4 || NewNumberOfCourses < 0)
            {
                printf(" Error: A Student can have maximum 4 , Minimum 1 number of courses and 0 to skip.\n\n");
                IsValidNumberOfCourse = 0;
            }
            else
            {
                IsValidNumberOfCourse = 1;
            }
        }
        if (NewNumberOfCourses != 0)
        {
            int OldTotalCourse = Students[StudentFoundIndex].NumberOfCourse;
            Students[StudentFoundIndex].NumberOfCourse = NewNumberOfCourses;

            int FirstCourseIndex;
            int dc;
            for (dc = 0; dc < TotalCourse; dc++)
            {
                if (strcmp(StudentID, Courses[dc].StudentID) == 0)
                {
                    FirstCourseIndex = dc; // store the index for delete
                    break;
                }
            }
            // after every delete array index will update (decrease by one)
            // we store the courses sequential
            // so if we know the first course index and total number of course we can delete all
            for (dc = 1; dc <= OldTotalCourse; dc++)
            {
                DeleteCourseByIndex(FirstCourseIndex);
            }

            for (int i = 1; i <= NewNumberOfCourses; i++)
            {
                printf(" Enter New Course %d Code: ", i);
                scanf("%s", CourseCode);

                printf(" Enter New Course %d Name: ", i);
                scanf(" %[^\n]s", CourseName);

                strcpy(Courses[TotalCourse].StudentID, StudentID);
                strcpy(Courses[TotalCourse].Code, CourseCode);
                strcpy(Courses[TotalCourse].Name, CourseName);
                TotalCourse++;
            }
        }

        if (strcmp(NewName, "0") != 0)
        {
            strcpy(Students[StudentFoundIndex].Name, NewName);
        }

        if (strcmp(NewEmail, "0") != 0)
        {
            strcpy(Students[StudentFoundIndex].Email, NewEmail);
        }

        if (strcmp(NewPhone, "0") != 0)
        {
            strcpy(Students[StudentFoundIndex].Phone, NewPhone);
        }

        printf(" Student Updated Successfully.\n\n");
    }
}

void DeleteStudent(int StudentIndex) // yashvardhan //anvitha+
{
    // NOTE:
    // We are not changing the Meta State here as we are changing it int Search student function passing pointer
    int d;
    int FirstCourseIndexs;
    struct StudentInfo ThisStudents;
    ThisStudents = Students[StudentIndex];
    if (StudentIndex == -1)
    {
        printf("\nNo Such Student exists\n\n");
    }
    else
    {
        for (d = 0; d < TotalCourse; d++)
        {
            if (strcmp(ThisStudents.ID, Courses[d].StudentID) == 0)
            {
                FirstCourseIndexs = d;
                break;
            }
        }
        for (d = 1; d <= ThisStudents.NumberOfCourse; d++)
        {
            DeleteCourseByIndex(FirstCourseIndexs);
        }
        DeleteStudentByIndex(StudentIndex);
        printf(" Student Deleted Successfully.\n\n");
    }
}

void total_students(MetaState *meta_state)
{
    ClearBackground(RAYWHITE);
    // Convert loaded image to texture to display

    // printf("c");
    // Unload image from RAM because it's already been uploaded to VRAM as a texture

    // printf("d");
    ClearBackground(RAYWHITE);
    // printf("e");
    DrawTextureV(total_students_texture, (Vector2){0, 0}, WHITE);
    // printf("f");

    DrawRectangle(50, 50, WINDOW_WIDTH - 100, 80, GRAY);
    DrawRectangleLines(50, 50, WINDOW_WIDTH - 100, 80, BLACK);

    // Draw title text
    DrawText("Total number of students", 80, 70, 30, BLACK);

    // Calculate position to draw the number at the center of the screen
    int number = TotalStudents; // Change this to the number you want to display
    Vector2 textSize = MeasureTextEx(GetFontDefault(), TextFormat("%d", number), 20, 1);
    Vector2 position = {700, 70};

    // Draw the number
    DrawTextEx(GetFontDefault(), TextFormat("%d", number), position, 50, 1, BLUE);

    DrawText("To go back to main menu press (b)", 80, 700, 30, BLACK);
    switch (GetKeyPressed())
    {
    case KEY_B:
        *meta_state = MAIN_MENU;
        break;
    }
}
