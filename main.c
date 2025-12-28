#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include "mainLibrary.h"

// Global variables
block buf;
char filename[50];
int C2, C31, C32, C33, C34, C35, C36;

#define MAX_INDEX 10000
Pr_index index_pr[MAX_INDEX];
Pr_index index_Yearbirth[MAX_INDEX];
Pr_index index_Datebirth[MAX_INDEX];
Pr_index index_Monthbirth[MAX_INDEX];
Pr_index index_Wilayabirth[MAX_INDEX];
Pr_index index_Gender[MAX_INDEX];
Pr_index index_BooldType[MAX_INDEX];
Pr_index index_YearStudy[MAX_INDEX];
Pr_index index_Speciality[MAX_INDEX];
Pr_index index_CP[MAX_INDEX];

int index_size = 0;
int index_size1 = 0;
int index_size2 = 0;
int index_size3 = 0;
int index_size4 = 0;
int index_size5 = 0;
int index_size6 = 0;
int index_size7 = 0;
int index_size8 = 0;
int index_size_CP = 0;

char *fname = "STUDENTS_ESI.bin";
char *fname1 = "StudentID_INDEX.idx";
char *fname2 = "YearBirth_INDEX.idx";
char *fname3 = "DateBirth_INDEX.idx";
char *fname4 = "MonthBirth_INDEX.idx";
char *fname5 = "WilayaBirth_INDEX.idx";
char *fname6 = "Gender_INDEX.idx";
char *fname7 = "BloodType_INDEX.idx";
char *fname8 = "YearStudy_INDEX.idx";
char *fname9 = "Speciality_INDEX.idx";
char *fname10 = "STUDENTS_CP_INDEX.idx";

int current_main_menu_selection = 0;
int current_submenu_selection = 0;
// Add this helper function near the top
char *format(const char *format, ...)
{
    static char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return buffer;
}

int get_terminal_width();
int get_terminal_height();
void gotoxy(int x, int y);
void set_color(int color);
void hide_cursor();
void showTOFNavigationMenu(int *currentPage, int *cursorIndex, int *continueDisplay,
                           int totalRecords, int totalPages, int recordsPerPage,
                           Pr_index *tofRecords, char *filename);
void show_cursor();
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CLEAR_SCREEN() system("cls")
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define CLEAR_SCREEN() system("clear")
#define SLEEP_MS(ms) usleep(ms * 1000)
#endif

// ========== COLOR CONSTANTS ==========
#define COLOR_BLACK 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_YELLOW 3
#define COLOR_BLUE 4
#define COLOR_MAGENTA 5
#define COLOR_CYAN 6
#define COLOR_WHITE 7
#define COLOR_GRAY 8 // Add this line

// ANSI Color Codes for terminal (for Linux/macOS)
#define ANSI_RESET "\033[0m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN "\033[36m"
#define ANSI_WHITE "\033[37m"
#define ANSI_BLACK "\033[30m"
#define ANSI_GRAY "\033[90m" // Add this lin

void showNavigationMenu(int *currentPage, int *cursorIndex, int *continueDisplay,
                        int tableSize, int totalPages, int recordsPerPage,
                        Pr_index *indexTable);
/**
 * Display TOF_ip index file with organized paging
 * @param filename Name of the TOF_ip file to display
 * @param cursorLine Line to highlight with cursor (0 for none)
 * @param recordsPerPage Number of records per page (default 20)
 *
 *
 *
 */
/**
 * Display TOF_ip index file with organized paging
 * @param filename Name of the TOF_ip file to display
 * @param recordsPerPage Number of records per page (default 20)
 */
// ========== ENHANCED DISPLAY FUNCTIONS ==========

/**
 * Display TOF_ip index file with enhanced UI
 * @param filename Name of the TOF_ip file to display
 * @param recordsPerPage Number of records per page (default 20)
 */
/**


/**
 * Display Pr_index table with organized layout - Fixed record details display
 */
void displayPrIndexTableWithData(Pr_index *indexTable, int tableSize,
                                 char *dataFilename, int recordsPerPage)
{
    int currentPage = 1;
    int cursorIndex = 0;
    int totalPages = (tableSize + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    // Open the main data file
    t_LnOVS *dataFile = NULL;
    if (dataFilename != NULL && strlen(dataFilename) > 0)
    {
        Open(&dataFile, dataFilename, 'E');
    }

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER ==========
        gotoxy(center_x - 20, 1);
        set_color(COLOR_MAGENTA);
        printf("PRIMARY INDEX TABLE WITH RECORD DATA");

        gotoxy(center_x - 25, 2);
        set_color(COLOR_CYAN);
        for (int i = 0; i < 50; i++)
            printf("=");
        printf("\n");

        // ========== STATISTICS ==========
        gotoxy(center_x - 25, 4);
        set_color(COLOR_WHITE);
        printf("Records: %-5d  Page: %-2d/%-2d  Cursor: Index %-4d",
               tableSize, currentPage, totalPages, cursorIndex + 1);

        gotoxy(center_x - 25, 5);
        printf("Data File: %s", dataFilename ? dataFilename : "None");

        gotoxy(center_x - 25, 6);
        set_color(COLOR_CYAN);
        for (int i = 0; i < 50; i++)
            printf("-");
        printf("\n\n");

        // ========== INDEX TABLE ==========
        int startIndex = (currentPage - 1) * recordsPerPage;
        int endIndex = startIndex + recordsPerPage;
        if (endIndex > tableSize)
            endIndex = tableSize;

        // Table header
        int tableWidth = 68;
        int tableStartX = center_x - (tableWidth / 2);
        int tableStartY = 8;

        gotoxy(tableStartX, tableStartY);
        set_color(COLOR_BLUE);
        printf("+----+------------+----------+----------+--------------+----------------------+\n");

        gotoxy(tableStartX, tableStartY + 1);
        set_color(COLOR_CYAN);
        printf("| #  |    ID      |  Block   |  Offset  |    Status    |   Record Preview     |\n");

        gotoxy(tableStartX, tableStartY + 2);
        set_color(COLOR_BLUE);
        printf("+----+------------+----------+----------+--------------+----------------------+\n");

        // Display records
        int lineY = tableStartY + 3;
        for (int i = startIndex; i < endIndex; i++)
        {
            gotoxy(tableStartX, lineY);

            int isCursorIndex = (i == cursorIndex);
            char status[20] = "Unknown";
            char namePreview[22] = "N/A";

            // Try to read actual record data
            if (dataFile != NULL && indexTable[i].crdt.block_number > 0)
            {
                block dataBlock;
                Pr_cor coords = indexTable[i].crdt;
                ReadBlock(dataFile, coords.block_number, &dataBlock);

                if (coords.offset >= 0 && coords.offset < dataBlock.Nb)
                {
                    rec studentRecord = dataBlock.tab[coords.offset];

                    if (studentRecord.Student_ID == -1)
                    {
                        strcpy(status, "Deleted");
                        strcpy(namePreview, "DELETED");
                    }
                    else if (studentRecord.Student_ID != indexTable[i].Identifier)
                    {
                        strcpy(status, "ID Mismatch");
                        snprintf(namePreview, sizeof(namePreview), "ID:%d", indexTable[i].Identifier);
                    }
                    else
                    {
                        strcpy(status, "Valid");

                        // Create compact name preview
                        if (strlen(studentRecord.Family_Name) > 0)
                        {
                            strncpy(namePreview, studentRecord.Family_Name, 8);
                            namePreview[8] = '\0';
                            strcat(namePreview, ".");
                            if (strlen(studentRecord.First_Name) > 0)
                            {
                                strncat(namePreview, &studentRecord.First_Name[0], 1);
                            }
                        }
                    }
                }
            }

            if (isCursorIndex)
            {
                set_color(COLOR_GREEN);
                printf("|>%-3d", i + 1);
                set_color(COLOR_WHITE);
                printf("| %-10d | %-8d | %-8d | ",
                       indexTable[i].Identifier,
                       indexTable[i].crdt.block_number,
                       indexTable[i].crdt.offset);

                // Status color
                if (strcmp(status, "Valid") == 0)
                    set_color(COLOR_GREEN);
                else if (strcmp(status, "Deleted") == 0)
                    set_color(COLOR_RED);
                else if (strcmp(status, "ID Mismatch") == 0)
                    set_color(COLOR_YELLOW);
                else
                    set_color(COLOR_WHITE);

                printf("%-12s | ", status);
                set_color(COLOR_WHITE);
                printf("%-20s |\n", namePreview);
            }
            else
            {
                set_color(COLOR_WHITE);
                printf("| %-3d| %-10d | %-8d | %-8d | ",
                       i + 1,
                       indexTable[i].Identifier,
                       indexTable[i].crdt.block_number,
                       indexTable[i].crdt.offset);

                // Status color
                if (strcmp(status, "Valid") == 0)
                    set_color(COLOR_GREEN);
                else if (strcmp(status, "Deleted") == 0)
                    set_color(COLOR_RED);
                else if (strcmp(status, "ID Mismatch") == 0)
                    set_color(COLOR_YELLOW);
                else
                    set_color(COLOR_GRAY);

                printf("%-12s | ", status);
                set_color(COLOR_WHITE);
                printf("%-20s |\n", namePreview);
            }

            lineY++;
        }

        gotoxy(tableStartX, lineY);
        set_color(COLOR_BLUE);
        printf("+----+------------+----------+----------+--------------+----------------------+\n");

        // Page info
        gotoxy(center_x - 20, lineY + 2);
        set_color(COLOR_CYAN);
        printf("Page %d/%d: Records %d to %d of %d",
               currentPage, totalPages, startIndex + 1, endIndex, tableSize);

        // ========== CURRENT RECORD DETAILS (VERTICAL STACK WITH SPACING) ==========
        int detailsStartY = lineY + 5; // More space after table

        // Clear details area (15 lines)
        for (int y = detailsStartY; y < detailsStartY + 20; y++)
        {
            gotoxy(center_x - 25, y);
            printf("%-50s", " ");
        }

        if (cursorIndex >= 0 && cursorIndex < tableSize &&
            dataFile != NULL && indexTable[cursorIndex].crdt.block_number > 0)
        {
            block dataBlock;
            Pr_cor coords = indexTable[cursorIndex].crdt;
            ReadBlock(dataFile, coords.block_number, &dataBlock);

            if (coords.offset >= 0 && coords.offset < dataBlock.Nb)
            {
                rec studentRecord = dataBlock.tab[coords.offset];

                if (studentRecord.Student_ID != -1)
                {
                    // Title with spacing
                    gotoxy(center_x - 25, detailsStartY);
                    set_color(COLOR_YELLOW);
                    printf("CURRENT RECORD DETAILS (Index: %d)", cursorIndex + 1);

                    // Empty line after title
                    gotoxy(center_x - 25, detailsStartY + 1);
                    printf(" ");

                    // Details - each on its own line with good spacing
                    int currentLine = detailsStartY + 2;

                    // ID
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tStudent ID: ");
                    set_color(COLOR_BLUE);
                    printf("%d", studentRecord.Student_ID);
                    currentLine += 2; // Space after each detail

                    // Name
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tName: ");
                    set_color(COLOR_CYAN);
                    printf("%s %s", studentRecord.Family_Name, studentRecord.First_Name);
                    currentLine += 2;

                    // Birth Date
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tDate of Birth: ");
                    set_color(COLOR_CYAN);
                    printf("%02d/%02d/%04d",
                           studentRecord.Date_Birth.day,
                           studentRecord.Date_Birth.month,
                           studentRecord.Date_Birth.year);
                    currentLine += 2;

                    // Gender and Wilaya
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tGender: ");
                    set_color(COLOR_CYAN);
                    printf("%s", studentRecord.Gender == 1 ? "Male" : "Female");

                    gotoxy(center_x + 5, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tWilaya: ");
                    set_color(COLOR_CYAN);
                    printf("%s", studentRecord.Wilaya_Birth);
                    currentLine += 2;

                    // Year and Speciality
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tYear: ");
                    set_color(COLOR_CYAN);
                    printf("%s", studentRecord.Year_Study);

                    gotoxy(center_x + 5, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tSpeciality: ");
                    set_color(COLOR_CYAN);
                    printf("%s", studentRecord.Speciality);
                    currentLine += 2;

                    // Blood Type and Resident
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tBlood Type:");
                    set_color(COLOR_CYAN);
                    printf("%s", studentRecord.Blood_Type);

                    gotoxy(center_x + 5, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tResident: ");
                    set_color(COLOR_CYAN);
                    printf("%s", studentRecord.Resident_UC ? "Yes" : "No");
                    currentLine += 2;

                    // Location
                    gotoxy(center_x - 25, currentLine);
                    set_color(COLOR_WHITE);
                    printf("\n\t\t\tLocation: ");
                    set_color(COLOR_CYAN);
                    printf("Block %d, Offset %d", coords.block_number, coords.offset);
                    currentLine += 3;

                    // Separator line
                    gotoxy(center_x - 25, currentLine);

                    detailsStartY = currentLine + 2; // Update for navigation prompt
                }
            }
        }
        else
        {
            // If no valid record, still show separator
            gotoxy(center_x - 25, detailsStartY);
            set_color(COLOR_CYAN);
            for (int i = 0; i < 50; i++)
                printf("-");
            detailsStartY += 2;
        }

        // ========== NAVIGATION PROMPT (WITH SPACING) ==========
        int promptY = detailsStartY + 3; // More space after details

        // Clear prompt area
        for (int y = promptY; y < promptY + 5; y++)
        {
            gotoxy(center_x - 25, y);
            printf("%-50s", " ");
        }

        gotoxy(center_x - 20, promptY);
        set_color(COLOR_CYAN);
        printf("\n\n\t\tPress Enter to continue... ");
        printf("\n\n");

        gotoxy(center_x - 15, promptY + 1);

        // ========== INPUT HANDLING ==========
        int keyPressed = 0;

        show_cursor();

#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0) // Arrow key
        {
            key = _getch();
            if (key == 72) // Up arrow
            {
                if (cursorIndex > 0)
                {
                    cursorIndex--;
                    if (cursorIndex < startIndex)
                    {
                        currentPage--;
                        if (currentPage < 1)
                            currentPage = 1;
                    }
                }
                keyPressed = 1;
            }
            else if (key == 80) // Down arrow
            {
                if (cursorIndex < tableSize - 1)
                {
                    cursorIndex++;
                    if (cursorIndex >= endIndex)
                    {
                        currentPage++;
                        if (currentPage > totalPages)
                            currentPage = totalPages;
                    }
                }
                keyPressed = 1;
            }
            else if (key == 75) // Left arrow - Previous page
            {
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
            }
            else if (key == 77) // Right arrow - Next page
            {
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
            }
        }
        else if (key == 13) // Enter key - show navigation menu
        {
            // Show navigation menu
            showNavigationMenu(&currentPage, &cursorIndex, &continueDisplay,
                               tableSize, totalPages, recordsPerPage, indexTable);
            keyPressed = 1;
        }
        else if (key == 27) // ESC key
        {
            continueDisplay = 0;
            keyPressed = 1;
        }
        else
        {
            // Check for single-key navigation
            switch (key)
            {
            case 'n':
            case 'N':
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
                break;

            case 'p':
            case 'P':
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
                break;

            case 'q':
            case 'Q':
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#endif

        hide_cursor();

        // If no key was processed, wait a bit
        if (!keyPressed)
        {
            SLEEP_MS(100);
        }
    }

    // Close the data file
    if (dataFile != NULL)
        Close(dataFile);

    // Clear and show exit message
    CLEAR_SCREEN();
    gotoxy(center_x - 15, height / 2);
    set_color(COLOR_GREEN);
    printf("Display Completed!\n");
    SLEEP_MS(1000);
}

// Add these function prototypes near the other prototypes
void delete_student_workflow();
void modify_student_workflow();
void modify_student_name_workflow();
void modify_student_year_workflow();
void modify_student_speciality_workflow();
void modify_student_resident_workflow();

// Add these function implementations:

void delete_student_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // Step 1: Get student ID
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("DELETE STUDENT - STEP 1/2");

    gotoxy(center_x - 20, 8);
    set_color(COLOR_WHITE);
    printf("Enter Student ID to delete: ");

    set_color(COLOR_CYAN);
    show_cursor();
    int studentID;
    scanf("%d", &studentID);
    getchar(); // Clear newline
    hide_cursor();

    // Step 2: Confirmation
    CLEAR_SCREEN();

    // Search for the student first
    bool found;
    int position;
    binary_search(index_pr, index_size, studentID, &found, &position);

    if (!found)
    {
        gotoxy(center_x - 15, 8);
        set_color(COLOR_RED);
        printf("Student ID %d not found!", studentID);

        gotoxy(center_x - 15, 10);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Get student details for confirmation
    t_LnOVS *F;
    Open(&F, fname, 'E');

    block dataBlock;
    Pr_cor coords = index_pr[position].crdt;
    ReadBlock(F, coords.block_number, &dataBlock);

    rec studentRecord = dataBlock.tab[coords.offset];
    Close(F);

    gotoxy(center_x - 15, 2);
    set_color(COLOR_MAGENTA);
    printf("CONFIRM STUDENT DELETION");

    gotoxy(center_x - 30, 5);
    set_color(COLOR_WHITE);
    printf("+------------------------------------------------------------+");

    gotoxy(center_x - 30, 6);
    printf("|                    STUDENT TO DELETE                       |");

    gotoxy(center_x - 30, 7);
    printf("+------------------------------------------------------------+");

    int y = 8;

    // Display student information
    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Student ID", "N/A"); // ID will be shown separately

    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Name", studentRecord.First_Name);

    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Family Name", studentRecord.Family_Name);

    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Year", studentRecord.Year_Study);

    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Speciality", studentRecord.Speciality);

    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Wilaya", studentRecord.Wilaya_Birth);

    char date_str[20];
    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
             studentRecord.Date_Birth.day,
             studentRecord.Date_Birth.month,
             studentRecord.Date_Birth.year);
    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Birth Date", date_str);

    gotoxy(center_x - 30, y++);
    printf("|  %-20s: %-35s |", "Location",
           format("Block %d, Offset %d", coords.block_number, coords.offset));

    gotoxy(center_x - 30, y++);
    printf("+------------------------------------------------------------+");

    y += 2;

    // Warning message
    gotoxy(center_x - 25, y++);
    set_color(COLOR_RED);
    printf("WARNING: This action cannot be undone!");

    gotoxy(center_x - 25, y++);
    printf("The student record will be marked as deleted.");

    y += 1;

    // Confirmation prompt
    gotoxy(center_x - 15, y++);
    set_color(COLOR_YELLOW);
    printf("Delete student ID %d? (Y/N): ", studentID);

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        int C35 = 0;
        Delete_by_student_ID(index_pr, &index_size, studentID, fname, &C35);

        // Delete from ALL indexes including CP index
        Delete_from_index(index_pr, &index_size, studentID);
        Delete_from_index1(index_Wilayabirth, &index_size8, coords.block_number, coords.offset);
        Delete_from_index1(index_Speciality, &index_size7, coords.block_number, coords.offset);
        Delete_from_index1(index_YearStudy, &index_size6, coords.block_number, coords.offset);
        Delete_from_index1(index_Gender, &index_size5, coords.block_number, coords.offset);
        Delete_from_index1(index_Monthbirth, &index_size4, coords.block_number, coords.offset);
        Delete_from_index1(index_Yearbirth, &index_size3, coords.block_number, coords.offset);
        Delete_from_index1(index_BooldType, &index_size2, coords.block_number, coords.offset);
        Delete_from_index1(index_Datebirth, &index_size1, coords.block_number, coords.offset);
        Delete_from_index1(index_CP, &index_size_CP, coords.block_number, coords.offset); // Delete from CP index

        // Save ALL updated indexes including CP index
        int cost;
        save_indextable(index_pr, index_size, fname1, &cost);
        save_indextable(index_BooldType, index_size2, fname7, &cost);
        save_indextable(index_Datebirth, index_size1, fname3, &cost);
        save_indextable(index_Yearbirth, index_size3, fname2, &cost);
        save_indextable(index_Monthbirth, index_size4, fname4, &cost);
        save_indextable(index_Gender, index_size5, fname6, &cost);
        save_indextable(index_YearStudy, index_size6, fname8, &cost);
        save_indextable(index_Speciality, index_size7, fname9, &cost);
        save_indextable(index_Wilayabirth, index_size8, fname5, &cost);
        save_indextable(index_CP, index_size_CP, fname10, &cost); // Save CP index

        // Success message
        gotoxy(center_x - 15, y + 2);
        set_color(COLOR_GREEN);
        printf("Student deleted successfully!");

        gotoxy(center_x - 15, y + 3);
        set_color(COLOR_CYAN);
        printf("Deletion cost C35: %d", C35);
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Deletion cancelled.");
    }

    // Wait for user
    gotoxy(center_x - 15, y + 6);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");
    getchar();
}

/**
 * Complete workflow for modifying student's first name
 */
void modify_student_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // Step 1: Get student ID
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("MODIFY STUDENT FIRST NAME - STEP 1/3");

    gotoxy(center_x - 20, 8);
    set_color(COLOR_WHITE);
    printf("Enter Student ID to modify: ");

    set_color(COLOR_CYAN);
    show_cursor();
    int studentID;
    scanf("%d", &studentID);
    getchar(); // Clear newline
    hide_cursor();

    // Search for the student
    bool found;
    int position;
    binary_search(index_pr, index_size, studentID, &found, &position);

    if (!found)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 8);
        set_color(COLOR_RED);
        printf("Student ID %d not found!", studentID);

        gotoxy(center_x - 15, 10);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Get current student information
    t_LnOVS *F;
    Open(&F, fname, 'E');

    block dataBlock;
    Pr_cor coords = index_pr[position].crdt;
    ReadBlock(F, coords.block_number, &dataBlock);

    rec currentRecord = dataBlock.tab[coords.offset];
    Close(F);

    // Step 2: Show current information
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("MODIFY STUDENT FIRST NAME - STEP 2/3");

    // Display current student info
    int y = 6;

    gotoxy(center_x - 25, y);
    set_color(COLOR_YELLOW);
    printf("CURRENT STUDENT INFORMATION");
    y += 2;

    gotoxy(center_x - 25, y);
    set_color(COLOR_WHITE);
    printf("Student ID: ");
    set_color(COLOR_BLUE);
    printf("%d", studentID);
    y += 1;

    gotoxy(center_x - 25, y);
    set_color(COLOR_WHITE);
    printf("Full Name: ");
    set_color(COLOR_CYAN);
    printf("%s %s", currentRecord.First_Name, currentRecord.Family_Name);
    y += 1;

    gotoxy(center_x - 25, y);
    set_color(COLOR_WHITE);
    printf("Year & Speciality: ");
    set_color(COLOR_CYAN);
    printf("%s - %s", currentRecord.Year_Study, currentRecord.Speciality);
    y += 1;

    gotoxy(center_x - 25, y);
    set_color(COLOR_WHITE);
    printf("Wilaya: ");
    set_color(COLOR_CYAN);
    printf("%s", currentRecord.Wilaya_Birth);
    y += 2;

    // Step 3: Get new first name
    gotoxy(center_x - 25, y);
    set_color(COLOR_WHITE);
    printf("Current First Name: ");
    set_color(COLOR_GREEN);
    printf("%s", currentRecord.First_Name);
    y += 1;

    gotoxy(center_x - 25, y);
    set_color(COLOR_WHITE);
    printf("Enter new First Name: ");

    set_color(COLOR_CYAN);
    show_cursor();
    char newFirstName[30];
    fgets(newFirstName, sizeof(newFirstName), stdin);
    newFirstName[strcspn(newFirstName, "\n")] = 0;
    hide_cursor();

    // Step 4: Confirmation
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_MAGENTA);
    printf("CONFIRM NAME CHANGE");

    // Create confirmation box
    int boxY = 6;
    gotoxy(center_x - 30, boxY);
    set_color(COLOR_WHITE);
    printf("+------------------------------------------------------------+");

    gotoxy(center_x - 30, boxY + 1);
    printf("|                    CHANGE CONFIRMATION                     |");

    gotoxy(center_x - 30, boxY + 2);
    printf("+------------------------------------------------------------+");

    gotoxy(center_x - 30, boxY + 3);
    printf("|  %-20s: %-35s |", "Student ID", "N/A");

    gotoxy(center_x - 30, boxY + 4);
    printf("|  %-20s: %-35s |", "Family Name", currentRecord.Family_Name);

    gotoxy(center_x - 30, boxY + 5);
    printf("|  %-20s: %-35s |", "Current First Name", currentRecord.First_Name);

    gotoxy(center_x - 30, boxY + 6);
    printf("|  %-20s: %-35s |", "New First Name", newFirstName);

    // Show other info for context
    gotoxy(center_x - 30, boxY + 7);
    printf("|  %-20s: %-35s |", "Year", currentRecord.Year_Study);

    gotoxy(center_x - 30, boxY + 8);
    printf("|  %-20s: %-35s |", "Speciality", currentRecord.Speciality);

    gotoxy(center_x - 30, boxY + 9);
    printf("+------------------------------------------------------------+");

    // Warning about consequences
    gotoxy(center_x - 25, boxY + 11);
    set_color(COLOR_YELLOW);
    printf("This change will update the student's record in the database.");

    // Confirmation prompt
    gotoxy(center_x - 15, boxY + 13);
    set_color(COLOR_WHITE);
    printf("Confirm name change for ID ");
    set_color(COLOR_BLUE);
    printf("%d", studentID);
    set_color(COLOR_WHITE);
    printf("? (Y/N): ");

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        int C36 = 0;
        modification_first_name(studentID, newFirstName, index_pr, index_size, fname, &C36);

        // Success message
        gotoxy(center_x - 15, boxY + 15);
        set_color(COLOR_GREEN);
        printf("✓ First name updated successfully!");

        gotoxy(center_x - 15, boxY + 16);
        set_color(COLOR_CYAN);
        printf("Modification cost C36: %d", C36);

        // Show the change
        gotoxy(center_x - 25, boxY + 18);
        set_color(COLOR_WHITE);
        printf("Changed from: ");
        set_color(COLOR_YELLOW);
        printf("%s", currentRecord.First_Name);

        gotoxy(center_x - 25, boxY + 19);
        set_color(COLOR_WHITE);
        printf("Changed to:   ");
        set_color(COLOR_GREEN);
        printf("%s", newFirstName);
    }
    else
    {
        gotoxy(center_x - 10, boxY + 15);
        set_color(COLOR_RED);
        printf("✗ Modification cancelled.");
    }

    // Wait for user
    gotoxy(center_x - 15, boxY + 21);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");
    getchar();
}

/**
 * Helper function to show navigation menu
 */
void showNavigationMenu(int *currentPage, int *cursorIndex, int *continueDisplay,
                        int tableSize, int totalPages, int recordsPerPage,
                        Pr_index *indexTable)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Clear area for menu
    int menuStartY = height - 15;
    for (int y = menuStartY; y < height; y++)
    {
        gotoxy(0, y);
        printf("%-*s", width, " ");
    }

    // Draw menu box
    gotoxy(center_x - 25, menuStartY);
    set_color(COLOR_BLUE);
    printf("+----------------------------------------------------+\n");

    gotoxy(center_x - 25, menuStartY + 1);
    printf("|                 NAVIGATION MENU                   |\n");

    gotoxy(center_x - 25, menuStartY + 2);
    printf("+----------------------------------------------------+\n");

    char *navOptions[] = {
        "N - Next Page",
        "P - Previous Page",
        "G - Go to Page",
        "S - Search by ID",
        "C - Set Cursor Position",
        "Q - Quit"};

    for (int i = 0; i < 6; i++)
    {
        gotoxy(center_x - 25, menuStartY + 3 + i);
        printf("|  ");
        set_color(COLOR_GREEN);
        printf("%c", navOptions[i][0]);
        set_color(COLOR_WHITE);
        printf("%-45s", navOptions[i] + 2);
        set_color(COLOR_BLUE);
        printf(" |\n");
    }

    gotoxy(center_x - 25, menuStartY + 9);
    printf("+----------------------------------------------------+\n");

    gotoxy(center_x - 20, menuStartY + 11);
    set_color(COLOR_YELLOW);
    printf("Enter command: ");

    show_cursor();
    char command;
    scanf(" %c", &command);
    hide_cursor();

    switch (command)
    {
    case 'n':
    case 'N':
        if (*currentPage < totalPages)
        {
            (*currentPage)++;
            *cursorIndex = (*currentPage - 1) * recordsPerPage;
        }
        break;

    case 'p':
    case 'P':
        if (*currentPage > 1)
        {
            (*currentPage)--;
            *cursorIndex = (*currentPage - 1) * recordsPerPage;
        }
        break;

    case 'g':
    case 'G':
        printf("Enter page (1-%d): ", totalPages);
        int newPage;
        scanf("%d", &newPage);
        if (newPage >= 1 && newPage <= totalPages)
        {
            *currentPage = newPage;
            *cursorIndex = (*currentPage - 1) * recordsPerPage;
        }
        break;

    case 's':
    case 'S':
        printf("Enter Student ID: ");
        int searchId;
        scanf("%d", &searchId);

        bool found;
        int position;
        binary_search(indexTable, tableSize, searchId, &found, &position);

        if (found)
        {
            *cursorIndex = position;
            *currentPage = (position / recordsPerPage) + 1;
        }
        else
        {
            gotoxy(center_x - 20, menuStartY + 13);
            set_color(COLOR_RED);
            printf("ID %d not found!", searchId);
            SLEEP_MS(1500);
        }
        break;

    case 'c':
    case 'C':
        printf("Enter cursor index (1-%d): ", tableSize);
        int newCursor;
        scanf("%d", &newCursor);
        if (newCursor >= 1 && newCursor <= tableSize)
        {
            *cursorIndex = newCursor - 1;
            *currentPage = (*cursorIndex / recordsPerPage) + 1;
        }
        break;

    case 'q':
    case 'Q':
        *continueDisplay = 0;
        break;
    }

    // Clear the menu area
    for (int y = menuStartY; y < height; y++)
    {
        gotoxy(0, y);
        printf("%-*s", width, " ");
    }
}

/**
 * Display TOF_ip index file with organized paging - Enhanced version
 */
void displayTOFIndexFile(char *filename, int recordsPerPage)
{
    TOF_ip *F;
    int currentPage = 1;
    int cursorIndex = 0;
    int totalRecords = 0;
    int totalBlocks = 0;

    // Open the TOF_ip file
    Open_TOF(&F, filename, 'E');

    // Get total blocks
    totalBlocks = getHeader_ip(F, "nblk");

    // Count total records and collect all records for indexing
    block_ip buf;
    Pr_index *tofRecords = NULL;
    int recordCount = 0;

    // First pass: count total records
    for (int i = 1; i <= totalBlocks; i++)
    {
        ReadBlock_ip(F, i, &buf);
        totalRecords += buf.Nb;
    }

    // Allocate array for all records
    tofRecords = malloc(totalRecords * sizeof(Pr_index));

    // Second pass: collect all records
    for (int i = 1; i <= totalBlocks; i++)
    {
        ReadBlock_ip(F, i, &buf);
        for (int offset = 0; offset < buf.Nb; offset++)
        {
            tofRecords[recordCount].Identifier = buf.tab[offset].Identifier;
            tofRecords[recordCount].crdt.block_number = buf.tab[offset].crdt.block_number;
            tofRecords[recordCount].crdt.offset = buf.tab[offset].crdt.offset;
            recordCount++;
        }
    }

    // Calculate total pages
    int totalPages = (totalRecords + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER ==========
        gotoxy(center_x - 20, 1);
        set_color(COLOR_MAGENTA);
        printf("TOF INDEX FILE VIEWER");

        gotoxy(center_x - 25, 2);
        set_color(COLOR_CYAN);
        for (int i = 0; i < 50; i++)
            printf("=");
        printf("\n");

        // ========== STATISTICS ==========
        gotoxy(center_x - 25, 4);
        set_color(COLOR_WHITE);
        printf("File: %-20s", filename);

        gotoxy(center_x + 10, 4);
        printf("Blocks: %-4d", totalBlocks);

        gotoxy(center_x - 25, 5);
        printf("Records: %-5d  Page: %-2d/%-2d  Cursor: Index %-4d",
               totalRecords, currentPage, totalPages, cursorIndex + 1);

        gotoxy(center_x - 25, 6);
        set_color(COLOR_CYAN);
        for (int i = 0; i < 50; i++)
            printf("-");
        printf("\n\n");

        // ========== INDEX TABLE ==========
        int startIndex = (currentPage - 1) * recordsPerPage;
        int endIndex = startIndex + recordsPerPage;
        if (endIndex > totalRecords)
            endIndex = totalRecords;

        // Table header
        int tableWidth = 72;
        int tableStartX = center_x - (tableWidth / 2);
        int tableStartY = 8;

        gotoxy(tableStartX, tableStartY);
        set_color(COLOR_BLUE);
        printf("+----+------------+----------+----------+------------------------+\n");

        gotoxy(tableStartX, tableStartY + 1);
        set_color(COLOR_CYAN);
        printf("| #  |    ID      |  Block   |  Offset  |   Physical Position   |\n");

        gotoxy(tableStartX, tableStartY + 2);
        set_color(COLOR_BLUE);
        printf("+----+------------+----------+----------+------------------------+\n");

        // Display records
        int lineY = tableStartY + 3;
        for (int i = startIndex; i < endIndex; i++)
        {
            gotoxy(tableStartX, lineY);

            int isCursorIndex = (i == cursorIndex);

            // Calculate physical position
            long physicalPos = sizeof(header_ip) +
                               (tofRecords[i].crdt.block_number - 1) * sizeof(block_ip) +
                               tofRecords[i].crdt.offset * sizeof(rec_ip);

            if (isCursorIndex)
            {
                set_color(COLOR_GREEN);
                printf("|>%-3d", i + 1);
                set_color(COLOR_WHITE);
                printf("| %-10d | %-8d | %-8d | %-22ld |\n",
                       tofRecords[i].Identifier,
                       tofRecords[i].crdt.block_number,
                       tofRecords[i].crdt.offset,
                       physicalPos);
            }
            else
            {
                set_color(COLOR_WHITE);
                printf("| %-3d| %-10d | %-8d | %-8d | %-22ld |\n",
                       i + 1,
                       tofRecords[i].Identifier,
                       tofRecords[i].crdt.block_number,
                       tofRecords[i].crdt.offset,
                       physicalPos);
            }

            lineY++;
        }

        gotoxy(tableStartX, lineY);
        set_color(COLOR_BLUE);
        printf("+----+------------+----------+----------+------------------------+\n");

        // Page info
        gotoxy(center_x - 20, lineY + 2);
        set_color(COLOR_CYAN);
        printf("Page %d/%d: Records %d to %d of %d",
               currentPage, totalPages, startIndex + 1, endIndex, totalRecords);

        // ========== CURRENT ENTRY DETAILS (VERTICAL STACK WITH SPACING) ==========
        int detailsStartY = lineY + 5; // More space after table

        // Clear details area (20 lines)
        for (int y = detailsStartY; y < detailsStartY + 20; y++)
        {
            gotoxy(center_x - 25, y);
            printf("%-50s", " ");
        }

        if (cursorIndex >= 0 && cursorIndex < totalRecords)
        {
            // Title with spacing
            gotoxy(center_x - 25, detailsStartY);
            set_color(COLOR_YELLOW);
            printf("CURRENT ENTRY DETAILS (Index: %d)", cursorIndex + 1);

            // Empty line after title
            gotoxy(center_x - 25, detailsStartY + 1);
            printf(" ");

            // Details - each on its own line with good spacing
            int currentLine = detailsStartY + 2;

            // ID
            gotoxy(center_x - 25, currentLine);
            set_color(COLOR_WHITE);
            printf("\n\t\t\tIndex ID: ");
            set_color(COLOR_BLUE);
            printf("%d", tofRecords[cursorIndex].Identifier);
            currentLine += 2;

            // Block and Offset
            gotoxy(center_x - 25, currentLine);
            set_color(COLOR_WHITE);
            printf("\n\t\t\tData Location: ");
            set_color(COLOR_CYAN);
            printf("Block %d, Offset %d",
                   tofRecords[cursorIndex].crdt.block_number,
                   tofRecords[cursorIndex].crdt.offset);
            currentLine += 2;

            // Physical Position
            gotoxy(center_x - 25, currentLine);
            set_color(COLOR_WHITE);
            printf("\n\t\t\tPhysical Position: ");
            set_color(COLOR_MAGENTA);

            long physicalPos = sizeof(header_ip) +
                               (tofRecords[cursorIndex].crdt.block_number - 1) * sizeof(block_ip) +
                               tofRecords[cursorIndex].crdt.offset * sizeof(rec_ip);
            printf("%ld bytes", physicalPos);
            currentLine += 2;

            // File Structure
            gotoxy(center_x - 25, currentLine);
            set_color(COLOR_WHITE);
            printf("\n\t\t\tFile Structure: ");
            set_color(COLOR_CYAN);
            printf("TOF Index File");
            currentLine += 2;

            // Status
            gotoxy(center_x - 25, currentLine);
            set_color(COLOR_WHITE);
            printf("\n\t\t\tStatus: ");
            set_color(COLOR_GREEN);
            printf("Valid Index Entry");
            currentLine += 3;

            // Separator line
            gotoxy(center_x - 25, currentLine);
            set_color(COLOR_CYAN);

            detailsStartY = currentLine + 2; // Update for navigation prompt
        }
        else
        {
            // If no valid record, still show separator
            gotoxy(center_x - 25, detailsStartY);
            set_color(COLOR_CYAN);
            for (int i = 0; i < 50; i++)
                printf("-");
            detailsStartY += 2;
        }

        // ========== NAVIGATION PROMPT (WITH SPACING) ==========
        int promptY = detailsStartY + 3; // More space after details

        // Clear prompt area
        for (int y = promptY; y < promptY + 5; y++)
        {
            gotoxy(center_x - 25, y);
            printf("%-50s", " ");
        }

        gotoxy(center_x - 25, promptY);

        // ========== INPUT HANDLING ==========
        int keyPressed = 0;

        show_cursor();

#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0) // Arrow key
        {
            key = _getch();
            if (key == 72) // Up arrow
            {
                if (cursorIndex > 0)
                {
                    cursorIndex--;
                    if (cursorIndex < startIndex)
                    {
                        currentPage--;
                        if (currentPage < 1)
                            currentPage = 1;
                    }
                }
                keyPressed = 1;
            }
            else if (key == 80) // Down arrow
            {
                if (cursorIndex < totalRecords - 1)
                {
                    cursorIndex++;
                    if (cursorIndex >= endIndex)
                    {
                        currentPage++;
                        if (currentPage > totalPages)
                            currentPage = totalPages;
                    }
                }
                keyPressed = 1;
            }
            else if (key == 75) // Left arrow - Previous page
            {
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
            }
            else if (key == 77) // Right arrow - Next page
            {
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
            }
        }
        else if (key == 13) // Enter key - show navigation menu
        {
            // Show navigation menu
            showTOFNavigationMenu(&currentPage, &cursorIndex, &continueDisplay,
                                  totalRecords, totalPages, recordsPerPage,
                                  tofRecords, filename);
            keyPressed = 1;
        }
        else if (key == 27) // ESC key
        {
            continueDisplay = 0;
            keyPressed = 1;
        }
        else
        {
            // Check for single-key navigation
            switch (key)
            {
            case 'n':
            case 'N':
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
                break;

            case 'p':
            case 'P':
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
                break;

            case 'q':
            case 'Q':
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux/macOS implementation
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        int key = getchar();

        if (key == 27) // Escape sequence
        {
            key = getchar(); // Should be '['
            if (key == '[')
            {
                key = getchar();
                switch (key)
                {
                case 'A': // Up arrow
                    if (cursorIndex > 0)
                    {
                        cursorIndex--;
                        if (cursorIndex < startIndex)
                        {
                            currentPage--;
                            if (currentPage < 1)
                                currentPage = 1;
                        }
                    }
                    keyPressed = 1;
                    break;
                case 'B': // Down arrow
                    if (cursorIndex < totalRecords - 1)
                    {
                        cursorIndex++;
                        if (cursorIndex >= endIndex)
                        {
                            currentPage++;
                            if (currentPage > totalPages)
                                currentPage = totalPages;
                        }
                    }
                    keyPressed = 1;
                    break;
                case 'D': // Left arrow
                    if (currentPage > 1)
                    {
                        currentPage--;
                        cursorIndex = (currentPage - 1) * recordsPerPage;
                    }
                    keyPressed = 1;
                    break;
                case 'C': // Right arrow
                    if (currentPage < totalPages)
                    {
                        currentPage++;
                        cursorIndex = (currentPage - 1) * recordsPerPage;
                    }
                    keyPressed = 1;
                    break;
                }
            }
        }
        else if (key == 10 || key == 13) // Enter key
        {
            showTOFNavigationMenu(&currentPage, &cursorIndex, &continueDisplay,
                                  totalRecords, totalPages, recordsPerPage,
                                  tofRecords, filename);
            keyPressed = 1;
        }
        else if (key == 27) // ESC key (single press)
        {
            continueDisplay = 0;
            keyPressed = 1;
        }
        else
        {
            // Check for single-key navigation
            switch (key)
            {
            case 'n':
            case 'N':
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
                break;

            case 'p':
            case 'P':
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                keyPressed = 1;
                break;

            case 'q':
            case 'Q':
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

        hide_cursor();

        // If no key was processed, wait a bit
        if (!keyPressed)
        {
            SLEEP_MS(100);
        }
    }

    // Close the TOF index file
    Close_TOF(F);

    // Free allocated memory
    free(tofRecords);

    // Clear and show exit message
    CLEAR_SCREEN();
    gotoxy(center_x - 15, height / 2);
    set_color(COLOR_GREEN);
    printf("Display Completed!\n");
    SLEEP_MS(1000);
}

/**
 * Helper function to show TOF navigation menu
 */
void showTOFNavigationMenu(int *currentPage, int *cursorIndex, int *continueDisplay,
                           int totalRecords, int totalPages, int recordsPerPage,
                           Pr_index *tofRecords, char *filename)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Clear area for menu
    int menuStartY = height - 15;
    for (int y = menuStartY; y < height; y++)
    {
        gotoxy(0, y);
        printf("%-*s", width, " ");
    }

    // Draw menu box
    gotoxy(center_x - 25, menuStartY);
    set_color(COLOR_BLUE);
    printf("+----------------------------------------------------+\n");

    gotoxy(center_x - 25, menuStartY + 1);
    printf("|                 NAVIGATION MENU                   |\n");

    gotoxy(center_x - 25, menuStartY + 2);
    printf("+----------------------------------------------------+\n");

    char *navOptions[] = {
        "N - Next Page",
        "P - Previous Page",
        "G - Go to Page",
        "S - Search by ID",
        "C - Set Cursor Position",
        "V - View Entry Details",
        "Q - Quit"};

    for (int i = 0; i < 7; i++)
    {
        gotoxy(center_x - 25, menuStartY + 3 + i);
        printf("|  ");
        set_color(COLOR_GREEN);
        printf("%c", navOptions[i][0]);
        set_color(COLOR_WHITE);
        printf("%-45s", navOptions[i] + 2);
        set_color(COLOR_BLUE);
        printf(" |\n");
    }

    gotoxy(center_x - 25, menuStartY + 10);
    printf("+----------------------------------------------------+\n");

    gotoxy(center_x - 20, menuStartY + 11);
    set_color(COLOR_YELLOW);
    printf("Enter command: ");

    show_cursor();
    char command;
    scanf(" %c", &command);
    hide_cursor();

    switch (command)
    {
    case 'n':
    case 'N':
        if (*currentPage < totalPages)
        {
            (*currentPage)++;
            *cursorIndex = (*currentPage - 1) * recordsPerPage;
        }
        break;

    case 'p':
    case 'P':
        if (*currentPage > 1)
        {
            (*currentPage)--;
            *cursorIndex = (*currentPage - 1) * recordsPerPage;
        }
        break;

    case 'g':
    case 'G':
        printf("Enter page (1-%d): ", totalPages);
        int newPage;
        scanf("%d", &newPage);
        if (newPage >= 1 && newPage <= totalPages)
        {
            *currentPage = newPage;
            *cursorIndex = (*currentPage - 1) * recordsPerPage;
        }
        break;

    case 's':
    case 'S':
        printf("Enter Index ID: ");
        int searchId;
        scanf("%d", &searchId);

        bool found = false;
        int position = -1;

        // Linear search in TOF records
        for (int i = 0; i < totalRecords; i++)
        {
            if (tofRecords[i].Identifier == searchId)
            {
                found = true;
                position = i;
                break;
            }
        }

        if (found)
        {
            *cursorIndex = position;
            *currentPage = (position / recordsPerPage) + 1;
        }
        else
        {
            gotoxy(center_x - 20, menuStartY + 13);
            set_color(COLOR_RED);
            printf("ID %d not found in TOF index!", searchId);
            SLEEP_MS(1500);
        }
        break;

    case 'c':
    case 'C':
        printf("Enter cursor index (1-%d): ", totalRecords);
        int newCursor;
        scanf("%d", &newCursor);
        if (newCursor >= 1 && newCursor <= totalRecords)
        {
            *cursorIndex = newCursor - 1;
            *currentPage = (*cursorIndex / recordsPerPage) + 1;
        }
        break;

    case 'v':
    case 'V':
        if (*cursorIndex >= 0 && *cursorIndex < totalRecords)
        {
            // Show detailed view
            CLEAR_SCREEN();
            gotoxy(center_x - 15, 5);
            set_color(COLOR_MAGENTA);
            printf("TOF INDEX ENTRY DETAILS\n");

            gotoxy(center_x - 20, 7);
            set_color(COLOR_CYAN);
            printf("File: %s\n", filename);

            gotoxy(center_x - 20, 9);
            set_color(COLOR_WHITE);
            printf("Index Position: %d of %d\n", *cursorIndex + 1, totalRecords);

            gotoxy(center_x - 20, 10);
            printf("Student ID: %d\n", tofRecords[*cursorIndex].Identifier);

            gotoxy(center_x - 20, 11);
            printf("Block Number: %d\n", tofRecords[*cursorIndex].crdt.block_number);

            gotoxy(center_x - 20, 12);
            printf("Offset: %d\n", tofRecords[*cursorIndex].crdt.offset);

            // Calculate and show physical position
            long physicalPos = sizeof(header_ip) +
                               (tofRecords[*cursorIndex].crdt.block_number - 1) * sizeof(block_ip) +
                               tofRecords[*cursorIndex].crdt.offset * sizeof(rec_ip);

            gotoxy(center_x - 20, 13);
            printf("Physical Position: %ld bytes\n", physicalPos);

            gotoxy(center_x - 20, 14);
            printf("Hex Offset: 0x%08lX\n", physicalPos);

            gotoxy(center_x - 20, 16);
            set_color(COLOR_CYAN);
            printf("File Structure:");

            gotoxy(center_x - 20, 17);
            printf("Header: %zu bytes", sizeof(header_ip));

            gotoxy(center_x - 20, 18);
            printf("Block Size: %zu bytes", sizeof(block_ip));

            gotoxy(center_x - 20, 19);
            printf("Record Size: %zu bytes", sizeof(rec_ip));

            gotoxy(center_x - 15, 22);
            set_color(COLOR_YELLOW);
            printf("Press Enter to continue...");
            getchar(); // Clear buffer
            getchar();
        }
        break;

    case 'q':
    case 'Q':
        *continueDisplay = 0;
        break;
    }

    // Clear the menu area
    for (int y = menuStartY; y < height; y++)
    {
        gotoxy(0, y);
        printf("%-*s", width, " ");
    }
}
// Color codes
#ifdef _WIN32
#define COLOR_RESET 7
#define COLOR_GRAY 8
#else
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_WHITE "\033[1;37m"
#define COLOR_GRAY "\033[1;90m"
#endif

// Function prototypes
void initial_loading(int N, int *C2);
void display_welcome_screen();
void display_main_menu();
void display_file_operations_menu();
void display_search_operations_menu();
void display_optional_operations_menu();
void display_exit_confirmation();
void press_any_key_to_continue();
void operation_completed_prompt(int return_to_main);
void gotoxy(int x, int y);
void set_color(int color);
void hide_cursor();
void show_cursor();
int get_terminal_width();
int get_terminal_height();
int select_from_list(const char *title, const char *items[], int count, int start_y, int center_x);
int select_from_paged_list(const char *title, const char *items[], int count, int items_per_page, int start_y, int center_x);
int select_wilaya(int start_y, int center_x);
int select_blood_type(int start_y, int center_x);
int select_birth_day(int year, int month, int start_y, int center_x);
void insert_student_workflow();
void show_confirmation_screen(char *Family_Name, char *First_Name, int Gender,
                              char *Wilaya_Birth, char *Blood_Type, char *Year_Study,
                              char *Speciality_name, DateS Date_Birth, bool Resident_UC,
                              int center_x);
//===========================================================================
void display_blood_type_ui(Pr_index index_table[], int size, int value, char *fname, int *C41);
void display_speciality_ui(Pr_index index_table[], int size, char *fname, int *C42);
void display_birth_interval_ui(Pr_index index_table[], int size, int Y1, int Y2, char *fname, int *C43);
void display_year_study_ui(Pr_index index_table[], int size, int value, char *fname, int *C44);
void display_all_contents_ui(t_LnOVS *F, char *Fname);
void display_block_ui(t_LnOVS *F, int blkN, char *Fname);
void display_header_ui(char *filename);
void display_students_list_ui(const char *title, const char *subtitle, Pr_index index_table[], int size, int filter_value, char *fname, int totalStudents, int start_index, int *cost);
void display_students_list_interval_ui(const char *title, Pr_index index_table[], int size, int Y1, int Y2, char *fname, int totalStudents, int *cost);
void display_record_detail_ui(rec student, int center_x);
void display_search_operations_menu_blood_year();
char *select_file_menu(int start_y, int center_x);
int select_file_for_analysis(int start_y, int center_x);
int get_block_number_ui(int start_y, int center_x, char *filename);
void create_lof_file_workflow();
//===========================================================================

// Platform-specific implementations
#ifdef _WIN32
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hide_cursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void show_cursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

int get_terminal_width()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int get_terminal_height()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

#else
void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
}

void set_color(const char *color)
{
    printf("%s", color);
}

void hide_cursor()
{
    printf("\033[?25l");
}

void show_cursor()
{
    printf("\033[?25h");
}

int get_terminal_width()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int get_terminal_height()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}
#endif

// Menu functions implementation
int select_from_list(const char *title, const char *items[], int count, int start_y, int center_x)
{
    int cursor = 0;
    int selected = -1;

    while (selected == -1)
    {
        // Clear the menu area
        for (int i = 0; i < count + 4; i++)
        {
            gotoxy(center_x - 25, start_y + i);
            printf("%-50s", "");
        }

        // Draw simple ASCII box
        gotoxy(center_x - 25, start_y);
        set_color(COLOR_CYAN);
        printf("==================================================");

        gotoxy(center_x - 25, start_y + 1);
        printf("|");
        gotoxy(center_x + 21, start_y + 1);
        printf("|");

        // Title - centered
        int title_len = strlen(title);
        int title_start = center_x - (title_len / 2);
        gotoxy(title_start, start_y + 1);
        set_color(COLOR_YELLOW);
        printf("%s", title);

        gotoxy(center_x - 25, start_y + 2);
        set_color(COLOR_CYAN);
        printf("==================================================");

        // Items
        for (int i = 0; i < count; i++)
        {
            gotoxy(center_x - 25, start_y + 3 + i);
            set_color(COLOR_CYAN);
            printf("|");

            gotoxy(center_x - 22, start_y + 3 + i);
            if (i == cursor)
            {
                set_color(COLOR_GREEN);
                printf("> ");
                set_color(COLOR_WHITE);
                printf("%-40s", items[i]);
            }
            else
            {
                set_color(COLOR_GRAY);
                printf("  %-40s", items[i]);
            }

            gotoxy(center_x + 21, start_y + 3 + i);
            set_color(COLOR_CYAN);
            printf("|");
        }

        gotoxy(center_x - 25, start_y + 3 + count);
        set_color(COLOR_CYAN);
        printf("==================================================");

        // Instructions
        gotoxy(center_x - 25, start_y + 5 + count);
        set_color(COLOR_YELLOW);
        printf("UP/DOWN: Navigate   ENTER: Select   ESC: Cancel");

        // Get input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && cursor > 0)
            {
                cursor--;
            }
            else if (key == 80 && cursor < count - 1)
            {
                cursor++;
            }
        }
        else if (key == 13)
        {
            selected = cursor;
        }
        else if (key == 27)
        {
            return -1;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");

            if (key == 65 && cursor > 0)
            {
                cursor--;
            }
            else if (key == 66 && cursor < count - 1)
            {
                cursor++;
            }
        }
        else if (key == 10)
        {
            selected = cursor;
        }
        else if (key == 27)
        {
            return -1;
        }
#endif
    }

    return selected;
}

// Replace the select_wilaya function with this corrected version:
int select_wilaya(int start_y, int center_x)
{
    int cursor = 0;
    int page = 0;
    int selected = -1;
    int total_pages = 3;
    int items_per_page = 20;

    // Create display strings
    char wilaya_display[58][35];
    for (int i = 0; i < 58; i++)
    {
        snprintf(wilaya_display[i], sizeof(wilaya_display[i]), "%02d. %s",
                 wilayas[i].number, wilayas[i].name);
    }

    while (selected == -1)
    {
        // 1. FIRST, CLEAR THE ENTIRE AREA PROPERLY
        // Clear from start_y to start_y + 25 lines (more than enough)
        for (int i = 0; i < 25; i++)
        {
            gotoxy(center_x - 35, start_y + i);
            // Clear 70 characters
            for (int j = 0; j < 70; j++)
                printf(" ");
            gotoxy(center_x - 35, start_y + i);
        }

        // 2. Draw the box border (clean)
        gotoxy(center_x - 35, start_y);
        set_color(COLOR_BLUE);
        printf("+------------------------------------------------------------------+");

        // 3. Title line - draw the complete line
        gotoxy(center_x - 35, start_y + 1);
        printf("|");

        // Clear the middle part first
        gotoxy(center_x - 34, start_y + 1);
        for (int i = 0; i < 68; i++)
            printf(" ");

        gotoxy(center_x - 35, start_y + 1);
        printf("|");
        gotoxy(center_x + 33, start_y + 1);
        printf("|");

        // Title with page info
        char title_with_page[100];
        snprintf(title_with_page, sizeof(title_with_page), "SELECT WILAYA OF BIRTH - Page %d/%d", page + 1, total_pages);
        int title_len = strlen(title_with_page);
        int title_start = center_x - (title_len / 2);

        gotoxy(title_start, start_y + 1);
        set_color(COLOR_YELLOW);
        printf("%s", title_with_page);

        // 4. Separator line
        gotoxy(center_x - 35, start_y + 2);
        set_color(COLOR_BLUE);
        printf("+------------------------------------------------------------------+");

        // 5. Display wilayas for current page
        int start = page * items_per_page;
        int end = start + items_per_page;
        if (end > 58)
            end = 58;

        // First, clear the display area (lines 3-12)
        for (int i = 0; i < 10; i++) // 10 rows
        {
            gotoxy(center_x - 35 + 1, start_y + 3 + i); // Inside the box
            for (int j = 0; j < 68; j++)
                printf(" ");
        }

        // Now display items in 2 columns
        for (int i = start; i < end; i++)
        {
            int row = (i - start) % 10; // 0-9
            int col = (i - start) / 10; // 0 or 1

            int x_pos, y_pos;

            if (col == 0)
            {
                x_pos = center_x - 35 + 3; // Left column
            }
            else
            {
                x_pos = center_x - 35 + 35; // Right column (moved right a bit)
            }

            y_pos = start_y + 3 + row;

            // Go to position
            gotoxy(x_pos, y_pos);

            // Clear exactly 30 chars for this item
            for (int j = 0; j < 30; j++)
                printf(" ");
            gotoxy(x_pos, y_pos);

            if (i == cursor)
            {
                set_color(COLOR_GREEN);
                printf("> ");
                set_color(COLOR_WHITE);
            }
            else
            {
                set_color(COLOR_GRAY);
                printf("  ");
                set_color(COLOR_WHITE);
            }

            // Print wilaya (truncate if too long)
            if (strlen(wilaya_display[i]) > 28)
            {
                char temp[30];
                strncpy(temp, wilaya_display[i], 27);
                temp[27] = '\0';
                strcat(temp, "..");
                printf("%s", temp);
            }
            else
            {
                printf("%s", wilaya_display[i]);
            }
        }

        // 6. Bottom border - EXACT position
        gotoxy(center_x - 35, start_y + 13); // 3 + 10 rows
        set_color(COLOR_BLUE);
        printf("+------------------------------------------------------------------+");

        // 7. Clear the instruction area first (line 15 onward)
        for (int i = 15; i < 20; i++)
        {
            gotoxy(center_x - 35, start_y + i);
            for (int j = 0; j < 70; j++)
                printf(" ");
            gotoxy(center_x - 35, start_y + i);
        }

        // 8. Now print instructions at line 15
        gotoxy(center_x - 35, start_y + 15);
        set_color(COLOR_YELLOW);
        printf("ARROWS: Navigate  ");
        set_color(COLOR_CYAN);
        printf("PgUp/PgDn: ");
        set_color(COLOR_YELLOW);
        printf("Change Page  ");
        set_color(COLOR_GREEN);
        printf("ENTER: ");
        set_color(COLOR_YELLOW);
        printf("Select  ");
        set_color(COLOR_RED);
        printf("ESC: Cancel");

        // 9. Get input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();

            if (key == 72) // Up arrow
            {
                if (cursor > start)
                {
                    cursor--;
                }
                else if (page > 0)
                {
                    page--;
                    cursor = (page + 1) * items_per_page - 1;
                    if (cursor >= 58)
                        cursor = 57;
                }
            }
            else if (key == 80) // Down arrow
            {
                if (cursor < end - 1)
                {
                    cursor++;
                }
                else if (page < total_pages - 1)
                {
                    page++;
                    cursor = page * items_per_page;
                }
            }
            else if (key == 73 && page > 0) // Page Up
            {
                page--;
                cursor = page * items_per_page;
            }
            else if (key == 81 && page < total_pages - 1) // Page Down
            {
                page++;
                cursor = page * items_per_page;
            }
            else if (key == 75) // Left arrow
            {
                int current_row = cursor - start;
                int current_col = current_row / 10;

                if (current_col > 0)
                {
                    cursor -= 10;
                }
            }
            else if (key == 77) // Right arrow
            {
                int current_row = cursor - start;
                int current_col = current_row / 10;

                if (current_col == 0 && (cursor + 10) < end)
                {
                    cursor += 10;
                }
            }
        }
        else if (key == 13) // Enter
        {
            selected = cursor;
        }
        else if (key == 27) // ESC
        {
            return -1;
        }
#endif

        // Keep cursor in bounds
        if (cursor < 0)
            cursor = 0;
        if (cursor >= 58)
            cursor = 57;
        page = cursor / items_per_page;
    }

    // 10. Clear everything before returning
    for (int i = 0; i < 25; i++)
    {
        gotoxy(center_x - 35, start_y + i);
        for (int j = 0; j < 70; j++)
            printf(" ");
    }

    return selected;
}

// Also update the select_from_paged_list function to handle navigation better:
int select_from_paged_list(const char *title, const char *items[], int count, int items_per_page, int start_y, int center_x)
{
    int cursor = 0;
    int page = 0;
    int selected = -1;
    int total_pages = (count + items_per_page - 1) / items_per_page;

    // Clear the entire selection area before starting
    int clear_height = items_per_page + 8;
    for (int i = 0; i < clear_height; i++)
    {
        gotoxy(center_x - 35, start_y + i);
        printf("%-70s", "");
    }

    while (selected == -1)
    {
        // Clear only the dynamic content area
        for (int i = 3; i < items_per_page + 3; i++)
        {
            gotoxy(center_x - 35, start_y + i);
            printf("%-70s", "");
        }

        gotoxy(center_x - 35, start_y);
        set_color(COLOR_BLUE);
        printf("+------------------------------------------------------------------+");

        // Title line
        gotoxy(center_x - 35, start_y + 1);
        printf("|");
        gotoxy(center_x + 33, start_y + 1);
        printf("|");

        // Create title with page info
        char title_with_page[100];
        snprintf(title_with_page, sizeof(title_with_page), "%s - Page %d/%d", title, page + 1, total_pages);
        int title_len = strlen(title_with_page);
        int title_start = center_x - (title_len / 2);

        gotoxy(title_start, start_y + 1);
        set_color(COLOR_YELLOW);
        printf("%s", title_with_page);

        // Separator line
        gotoxy(center_x - 35, start_y + 2);
        set_color(COLOR_BLUE);
        printf("+------------------------------------------------------------------+");

        // Display items for current page
        int start = page * items_per_page;
        int end = start + items_per_page;
        if (end > count)
            end = count;

        // Calculate layout - use 2 columns for better organization
        int cols = 2;
        int rows_per_col = (items_per_page + cols - 1) / cols;
        int col_width = 30;

        for (int i = start; i < end; i++)
        {
            int row = (i - start) % rows_per_col;
            int col = (i - start) / rows_per_col;

            int x_pos = center_x - 35 + 3 + (col * col_width);
            int y_pos = start_y + 3 + row;

            gotoxy(x_pos, y_pos);

            // Clear the line first
            printf("%-30s", "");
            gotoxy(x_pos, y_pos);

            if (i == cursor)
            {
                set_color(COLOR_GREEN);
                printf("> ");
                set_color(COLOR_WHITE);
            }
            else
            {
                set_color(COLOR_GRAY);
                printf("  ");
            }

            // Display item with truncation if needed
            char display_item[col_width - 2];
            strncpy(display_item, items[i], sizeof(display_item) - 1);
            display_item[sizeof(display_item) - 1] = '\0';

            // Add ellipsis if too long
            if (strlen(items[i]) > sizeof(display_item) - 1)
            {
                int len = sizeof(display_item) - 4;
                if (len > 0)
                {
                    strncpy(display_item, items[i], len);
                    display_item[len] = '\0';
                    strcat(display_item, "...");
                }
            }
            printf("%s", display_item);
        }

        // Bottom border
        gotoxy(center_x - 35, start_y + 3 + rows_per_col);
        set_color(COLOR_BLUE);
        printf("+------------------------------------------------------------------+");

        // Instructions
        gotoxy(center_x - 35, start_y + 5 + rows_per_col);
        set_color(COLOR_YELLOW);
        printf("ARROWS: Navigate  PAGE UP/DOWN: Change Page  ENTER: Select  ESC: Cancel");

        // Get input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && cursor > start) // Up arrow
            {
                cursor--;
            }
            else if (key == 80 && cursor < end - 1) // Down arrow
            {
                cursor++;
            }
            else if (key == 73 && page > 0) // Page Up
            {
                page--;
                cursor = page * items_per_page;
            }
            else if (key == 81 && page < total_pages - 1) // Page Down
            {
                page++;
                cursor = page * items_per_page;
            }
            else if (key == 75 && cursor > start) // Left arrow
            {
                int current_col = (cursor - start) / rows_per_col;
                if (current_col > 0)
                {
                    cursor -= rows_per_col;
                    if (cursor < start)
                        cursor = start;
                }
            }
            else if (key == 77 && cursor < end - 1) // Right arrow
            {
                int current_col = (cursor - start) / rows_per_col;
                if (current_col < cols - 1 && cursor + rows_per_col < end)
                {
                    cursor += rows_per_col;
                }
            }
        }
        else if (key == 13) // Enter
        {
            selected = cursor;
        }
        else if (key == 27) // ESC
        {
            return -1;
        }
#else
        // Linux implementation
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            int key2 = getchar(); // Skip [
            if (key2 == '[')
            {
                key = getchar();
                system("stty cooked");

                if (key == 'A' && cursor > start) // Up
                {
                    cursor--;
                }
                else if (key == 'B' && cursor < end - 1) // Down
                {
                    cursor++;
                }
                else if (key == '5') // Page Up
                {
                    system("stty raw");
                    getchar(); // Skip ~
                    system("stty cooked");
                    if (page > 0)
                    {
                        page--;
                        cursor = page * items_per_page;
                    }
                }
                else if (key == '6') // Page Down
                {
                    system("stty raw");
                    getchar(); // Skip ~
                    system("stty cooked");
                    if (page < total_pages - 1)
                    {
                        page++;
                        cursor = page * items_per_page;
                    }
                }
                else if (key == 'D' && cursor > start) // Left
                {
                    int current_col = (cursor - start) / rows_per_col;
                    if (current_col > 0)
                    {
                        cursor -= rows_per_col;
                        if (cursor < start)
                            cursor = start;
                    }
                }
                else if (key == 'C' && cursor < end - 1) // Right
                {
                    int current_col = (cursor - start) / rows_per_col;
                    if (current_col < cols - 1 && cursor + rows_per_col < end)
                    {
                        cursor += rows_per_col;
                    }
                }
            }
            else
            {
                system("stty cooked");
                return -1; // ESC alone
            }
        }
        else if (key == 10) // Enter
        {
            selected = cursor;
        }
#endif
    }

    // Clear the entire selection area after selection
    for (int i = 0; i < clear_height; i++)
    {
        gotoxy(center_x - 35, start_y + i);
        printf("%-70s", "");
    }

    return selected;
}

int select_birth_day(int year, int month, int start_y, int center_x)
{
    int max_days = getDaysInMonth(year, month);

    // Create day strings
    char day_strings[31][10];
    const char *day_options[31];

    for (int i = 0; i < max_days; i++)
    {
        snprintf(day_strings[i], sizeof(day_strings[i]), "%2d", i + 1);
        day_options[i] = day_strings[i];
    }

    // Use a grid layout (7 columns for days of week)
    int cols = 7;
    int rows = (max_days + cols - 1) / cols;

    int cursor = 0;
    int selected = -1;

    while (selected == -1)
    {
        // Calculate display area
        int display_height = rows + 8; // rows + borders + instructions

        // 1. Clear the entire area first
        for (int i = 0; i < display_height + 5; i++) // Extra buffer
        {
            gotoxy(center_x - 25, start_y + i);
            printf("%-50s", " "); // Clear 50 spaces
        }

        // 2. Draw the box
        gotoxy(center_x - 25, start_y);
        set_color(COLOR_MAGENTA);
        printf("+--------------------------------------------------+");

        // 3. Title line - clear and draw
        gotoxy(center_x - 25, start_y + 1);
        printf("|");
        gotoxy(center_x + 23, start_y + 1);
        printf("|");

        // Clear title area
        gotoxy(center_x - 24, start_y + 1);
        for (int i = 0; i < 48; i++)
            printf(" ");

        // Show month and year in title
        char title[50];
        const char *month_names[] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"};
        snprintf(title, sizeof(title), "SELECT DAY - %s %d", month_names[month - 1], year);

        int title_len = strlen(title);
        int title_start = center_x - (title_len / 2);

        gotoxy(title_start, start_y + 1);
        set_color(COLOR_YELLOW);
        printf("%s", title);

        // 4. Separator line
        gotoxy(center_x - 25, start_y + 2);
        set_color(COLOR_MAGENTA);
        printf("+--------------------------------------------------+");

        // 5. Clear the day grid area
        for (int i = 0; i < rows + 1; i++) // +1 for bottom border
        {
            gotoxy(center_x - 25, start_y + 3 + i);
            printf("|");
            gotoxy(center_x + 23, start_y + 3 + i);
            printf("|");

            // Clear inside
            gotoxy(center_x - 24, start_y + 3 + i);
            for (int j = 0; j < 48; j++)
                printf(" ");
        }

        // 6. Display day numbers in a 7-column grid
        // First, display day headers (optional)
        gotoxy(center_x - 25, start_y + 3);
        set_color(COLOR_CYAN);
        printf("|");
        gotoxy(center_x - 22, start_y + 3);
        printf("Su Mo Tu We Th Fr Sa");
        gotoxy(center_x + 23, start_y + 3);
        printf("|");

        // Now display the days in grid format
        for (int day = 1; day <= max_days; day++)
        {
            int day_index = day - 1; // 0-based index
            int row = (day - 1) / cols;
            int col = (day - 1) % cols;

            // Calculate position (centered in each column)
            int col_width = 6; // Each column is 6 chars wide
            int x_pos = center_x - 25 + 3 + (col * col_width) + 1;
            int y_pos = start_y + 4 + row; // +4 because: start_y + 3 (header) + 1

            gotoxy(x_pos, y_pos);

            // Clear this position first (4 chars for " XX ")
            printf("    ");
            gotoxy(x_pos, y_pos);

            if (day_index == cursor)
            {
                // Highlight selected day
                set_color(COLOR_GREEN);
                printf("[%2d]", day);
            }
            else
            {
                set_color(COLOR_WHITE);
                printf(" %2d ", day);
            }
        }

        // 7. Draw bottom border of grid
        gotoxy(center_x - 25, start_y + 4 + rows);
        set_color(COLOR_MAGENTA);
        printf("+--------------------------------------------------+");

        // 8. Clear instruction area
        for (int i = 0; i < 3; i++)
        {
            gotoxy(center_x - 25, start_y + 5 + rows + i);
            printf("%-50s", " ");
        }

        // 9. Display instructions
        gotoxy(center_x - 25, start_y + 5 + rows);
        set_color(COLOR_YELLOW);
        printf("ARROWS: Navigate  ENTER: Select  ESC: Cancel");

        // 10. Show current selection
        gotoxy(center_x - 25, start_y + 6 + rows);
        set_color(COLOR_CYAN);
        printf("Selected: %d/%d/%d", cursor + 1, month, year);

        // 11. Get input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && cursor >= cols) // Up arrow
            {
                cursor -= cols;
            }
            else if (key == 80 && (cursor + cols) < max_days) // Down arrow
            {
                cursor += cols;
            }
            else if (key == 75 && cursor > 0) // Left arrow
            {
                cursor--;
            }
            else if (key == 77 && cursor < max_days - 1) // Right arrow
            {
                cursor++;
            }
        }
        else if (key == 13) // Enter
        {
            selected = cursor;
        }
        else if (key == 27) // ESC
        {
            return -1;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");

            if (key == 'A' && cursor >= cols) // Up
            {
                cursor -= cols;
            }
            else if (key == 'B' && (cursor + cols) < max_days) // Down
            {
                cursor += cols;
            }
            else if (key == 'D' && cursor > 0) // Left
            {
                cursor--;
            }
            else if (key == 'C' && cursor < max_days - 1) // Right
            {
                cursor++;
            }
        }
        else if (key == 10) // Enter
        {
            selected = cursor;
        }
        else if (key == 27) // ESC
        {
            return -1;
        }
#endif
    }

    // Clear the display area before returning
    for (int i = 0; i < 20; i++)
    {
        gotoxy(center_x - 25, start_y + i);
        printf("%-50s", " ");
    }

    return selected;
}

int select_blood_type(int start_y, int center_x)
{
    const char *blood_options[8];
    for (int i = 0; i < 8; i++)
    {
        blood_options[i] = Blood_Types[i].name;
    }

    return select_from_list("SELECT BLOOD TYPE", blood_options, 8, start_y, center_x);
}

// Initial loading function
// Initial loading function
void initial_loading(int N, int *C2)
{
    *C2 = 0;
    t_LnOVS *F;
    index_size = 0;
    index_size1 = 0;
    index_size2 = 0;
    index_size3 = 0;
    index_size4 = 0;
    index_size5 = 0;
    index_size6 = 0;
    index_size7 = 0;
    index_size8 = 0;
    index_size_CP = 0; // Initialize CP index size

    // Seed random number generator
    srand(time(NULL));

    Open(&F, "STUDENTS_ESI.bin", 'N');

    index_size = 0; // Track size of index table
    long current_block = 1;
    long first_block = current_block; // Save first block
    long previous_block = current_block;

    // Initialize buffer
    buf.Nb = 0;
    buf.prev = -1;
    buf.next = -1;

    // Set header - tail should be same as head initially
    setHeader(F, "head", first_block);
    setHeader(F, "tail", first_block);

    // Generate unique student ID
    int Student_ID;
    bool found;
    int pos;

    char Family_Name[30], First_Name[30];
    int year, month, day, wilya_num, Gender, rand_year, rand_blood;
    bool Resident_UC;
    int rand_spec;
    // Generate N records
    for (int k = 0; k < N; k++)
    {
        // Check if current block is full
        if (buf.Nb >= 40)
        {

            // Allocate new block
            previous_block = current_block;
            current_block = AllocBlock(F);

            // Link previous block to new block
            buf.next = current_block;
            WriteBlock(F, previous_block, &buf);
            // incrementing the cost
            (*C2)++;

            // Reset buffer for new block
            buf.Nb = 0;
            buf.prev = previous_block;
            buf.next = -1;

            // Update tail in header
            setHeader(F, "tail", current_block);
        }

        do
        {
            Student_ID = rand_number(9000, 1000);
            binary_search(index_pr, index_size, Student_ID, &found, &pos);
        } while (found); // Continue until we find a unique ID

        // Generate random data

        random_name(Family_Name);
        random_name(First_Name);

        year = rand_number(2008, 2003);
        month = rand_number(12, 1);
        day = randomDay(year, month);

        wilya_num = rand_number(58, 1);
        Gender = rand_number(2, 1);
        rand_blood = rand_number(8, 1);
        rand_year = rand_number(5, 1);
        Resident_UC = rand_number(1, 0);

        // Fill the record
        buf.tab[buf.Nb].Student_ID = Student_ID;
        buf.tab[buf.Nb].Date_Birth.year = year;
        buf.tab[buf.Nb].Date_Birth.month = month;
        buf.tab[buf.Nb].Date_Birth.day = day;

        strcpy(buf.tab[buf.Nb].Family_Name, Family_Name);
        strcpy(buf.tab[buf.Nb].First_Name, First_Name);
        strcpy(buf.tab[buf.Nb].Wilaya_Birth, wilayas[wilya_num - 1].name);

        buf.tab[buf.Nb].Gender = Gender;
        strcpy(buf.tab[buf.Nb].Blood_Type, Blood_Types[rand_blood - 1].name);
        strcpy(buf.tab[buf.Nb].Year_Study, specs[rand_year - 1].years);

        // Set speciality
        if (rand_year <= 3)
        {
            rand_spec = 0;
            strcpy(buf.tab[buf.Nb].Speciality, specs[rand_year - 1].subspec[0].name);
        }
        else
        {
            rand_spec = rand_number(4, 1);
            // Array index range: 0-3
            strcpy(buf.tab[buf.Nb].Speciality, specs[rand_year - 1].subspec[rand_spec - 1].name);
        }

        buf.tab[buf.Nb].Resident_UC = Resident_UC;

        // Insert into ALL indexes including CP index
        Insert_Pr_index(index_pr, &index_size, Student_ID, current_block, buf.Nb);
        Insert_index(index_Datebirth, &index_size1, day, current_block, buf.Nb);
        Insert_index(index_BooldType, &index_size2, rand_blood, current_block, buf.Nb);
        Insert_index(index_Yearbirth, &index_size3, year, current_block, buf.Nb);
        Insert_index(index_Monthbirth, &index_size4, month, current_block, buf.Nb);
        Insert_index(index_Gender, &index_size5, Gender, current_block, buf.Nb);
        Insert_index(index_YearStudy, &index_size6, rand_year, current_block, buf.Nb);
        Insert_index(index_Speciality, &index_size7, rand_spec, current_block, buf.Nb);
        Insert_index(index_Wilayabirth, &index_size8, wilya_num, current_block, buf.Nb);
        Insert_index_CP(index_CP, &index_size_CP, Student_ID, buf.tab[buf.Nb], current_block, buf.Nb);
        buf.Nb++;
    }

    // Write the last block
    buf.prev = previous_block;
    buf.next = -1;
    WriteBlock(F, current_block, &buf);
    // incrementing the cost
    (*C2)++;
    // Update headers
    if (buf.Nb == 40)
    {
        setHeader(F, "freepos", 0);
    }
    else
    {
        setHeader(F, "freepos", buf.Nb);
    }
    setHeader(F, "tail", current_block);

    Close(F);

    printf("Successfully loaded %d students into database.\n", N);
    printf("\t\t\tPrimary index size: %d\n", index_size);
    printf("\t\t\tCP index size: %d (1CP/2CP students only)\n", index_size_CP);
}

// Insert student workflow
void insert_student_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Variables for student data
    int Gender = 0;
    char Family_Name[30], First_Name[30], Wilaya_Birth[20] = "",
                                          Blood_Type[50] = "", Year_Study[10] = "", Speciality_name[60] = "";
    DateS Date_Birth = {0, 0, 0};
    bool Resident_UC = false;

    // Step 1: Family Name
    // Step 2: First Name
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 1/11");

    gotoxy(center_x - 20, 8);
    set_color(COLOR_WHITE);
    printf("Enter Family Name: ");

    set_color(COLOR_CYAN);
    show_cursor();
    fgets(Family_Name, sizeof(Family_Name), stdin);
    Family_Name[strcspn(Family_Name, "\n")] = 0;
    hide_cursor();

    // Step 2: First Name
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 2/11");

    gotoxy(center_x - 20, 8);
    set_color(COLOR_WHITE);
    printf("Enter First Name: ");

    set_color(COLOR_CYAN);
    show_cursor();
    fgets(First_Name, sizeof(First_Name), stdin);
    First_Name[strcspn(First_Name, "\n")] = 0;
    hide_cursor();

    // Step 3: Gender
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 3/11");

    const char *gender_options[] = {"Male", "Female"};
    int gender_choice = select_from_list("SELECT GENDER", gender_options, 2, 8, center_x);
    if (gender_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    Gender = gender_choice + 1;

    // Step 4: Wilaya Birth
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 4/11");

    int wilaya_choice = select_wilaya(8, center_x);
    if (wilaya_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    strcpy(Wilaya_Birth, wilayas[wilaya_choice].name);

    // Step 5: Blood Type
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 5/11");

    int blood_choice = select_blood_type(8, center_x);
    if (blood_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    strcpy(Blood_Type, Blood_Types[blood_choice].name);

    // Step 6: Year of Study
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 6/11");

    const char *year_options[] = {"1CP", "2CP", "1CS", "2CS", "3CS"};
    int year_choice = select_from_list("SELECT YEAR OF STUDY", year_options, 5, 8, center_x);
    if (year_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    strcpy(Year_Study, year_options[year_choice]);

    // Step 7: Speciality
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 7/11");

    // Find the correct specialities based on year
    int year_index = -1;
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(specs[i].years, Year_Study) == 0)
        {
            year_index = i;
            break;
        }
    }

    if (year_index != -1)
    {
        Speciality spec = specs[year_index];
        const char *spec_options[10];
        int spec_count = 0;

        while (spec_count < 10 && strlen(spec.subspec[spec_count].name) > 0)
        {
            spec_options[spec_count] = spec.subspec[spec_count].name;
            spec_count++;
        }

        if (spec_count > 0)
        {
            int spec_choice = select_from_list("SELECT SPECIALITY", spec_options, spec_count, 8, center_x);
            if (spec_choice == -1)
            {
                operation_completed_prompt(0);
                return;
            }
            strcpy(Speciality_name, spec_options[spec_choice]);
        }
    }

    // Step 8: Resident Status
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 8/11");

    const char *resident_options[] = {"No - Not a resident", "Yes - University Campus Resident"};
    int resident_choice = select_from_list("RESIDENT STATUS", resident_options, 2, 8, center_x);
    if (resident_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    Resident_UC = (resident_choice == 1);

    // Step 9: Date of Birth - Year
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 9/11");

    // Create year options (2000-2024 for students)
    char year_strings[25][10];
    const char *year_birth_options[25];

    for (int i = 0; i < 25; i++)
    {
        int year = 2000 + i;
        snprintf(year_strings[i], sizeof(year_strings[i]), "%d", year);
        year_birth_options[i] = year_strings[i];
    }

    int year_birth_choice = select_from_paged_list("SELECT BIRTH YEAR", year_birth_options, 25, 15, 8, center_x);
    if (year_birth_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    Date_Birth.year = 2000 + year_birth_choice;

    // Step 10: Date of Birth - Month
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 10/11");

    const char *month_options[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"};

    int month_choice = select_from_list("SELECT BIRTH MONTH", month_options, 12, 8, center_x);
    if (month_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    Date_Birth.month = month_choice + 1;

    // Step 11: Date of Birth - Day
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("INSERT NEW STUDENT - STEP 11/11");

    int day_choice = select_birth_day(Date_Birth.year, Date_Birth.month, 8, center_x);
    if (day_choice == -1)
    {
        operation_completed_prompt(0);
        return;
    }
    Date_Birth.day = day_choice + 1;

    // Confirmation and insertion
    CLEAR_SCREEN();
    show_confirmation_screen(Family_Name, First_Name, Gender, Wilaya_Birth,
                             Blood_Type, Year_Study, Speciality_name,
                             Date_Birth, Resident_UC, center_x);
}

void show_confirmation_screen(char *Family_Name, char *First_Name, int Gender,
                              char *Wilaya_Birth, char *Blood_Type, char *Year_Study,
                              char *Speciality_name, DateS Date_Birth, bool Resident_UC,
                              int center_x)
{
    int height = get_terminal_height();
    int y = 5;

    // Header
    gotoxy(center_x - 15, 2);
    set_color(COLOR_MAGENTA);
    printf("CONFIRM USER INFORMATION");

    // Display box
    gotoxy(center_x - 30, y);
    set_color(COLOR_WHITE);
    printf("+------------------------------------------------------------+");
    y++;

    gotoxy(center_x - 30, y);
    printf("|                    USER INFORMATION                        |");
    y++;

    gotoxy(center_x - 30, y);
    printf("+------------------------------------------------------------+");
    y++;

    // Display all information
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Family Name", Family_Name);
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "First Name", First_Name);
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Gender", (Gender == 1) ? "Male" : "Female");
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Wilaya of Birth", Wilaya_Birth);
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Blood Type", Blood_Type);
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Year of Study", Year_Study);
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Speciality", Speciality_name);
    y++;

    char date_str[20];
    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
             Date_Birth.day, Date_Birth.month, Date_Birth.year);
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Date of Birth", date_str);
    y++;

    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Campus Resident", Resident_UC ? "Yes" : "No");
    y++;

    gotoxy(center_x - 30, y);
    printf("+------------------------------------------------------------+");

    // Confirmation prompt
    y += 2;
    gotoxy(center_x - 15, y);
    set_color(COLOR_YELLOW);
    printf("Confirm insertion? (Y/N): ");

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        // Load ALL index tables including CP index
        int C32 = 0;
        loadindextable(fname1, index_pr, &index_size, &C32);
        int cost1;
        loadindextable(fname3, index_Datebirth, &index_size1, &cost1);
        loadindextable(fname7, index_BooldType, &index_size2, &cost1);
        loadindextable(fname2, index_Yearbirth, &index_size3, &cost1);
        loadindextable(fname4, index_Monthbirth, &index_size4, &cost1);
        loadindextable(fname6, index_Gender, &index_size5, &cost1);
        loadindextable(fname8, index_YearStudy, &index_size6, &cost1);
        loadindextable(fname9, index_Speciality, &index_size7, &cost1);
        loadindextable(fname5, index_Wilayabirth, &index_size8, &cost1);
        loadindextable(fname10, index_CP, &index_size_CP, &cost1); // Load CP index

        // Insert the new student
        int C34 = 0;
        insert_newStudent("STUDENTS_ESI.bin", index_pr, &index_size, Gender,
                          Family_Name, First_Name, Wilaya_Birth, Blood_Type,
                          Year_Study, Speciality_name, Date_Birth, Resident_UC, &C34);

        // Now we need to find the newly inserted student to get its coordinates
        // Generate the same Student_ID that was used in insert_newStudent
        int Student_ID = 0;
        bool found_after_insert;
        int position_after;
        
        // We need to search for the student in the index
        // Since we just inserted it, it should be there
        // We'll search by matching the name and other details
        t_LnOVS *F;
        Open(&F, "STUDENTS_ESI.bin", 'E');
        block buffer;
        
        // Find the student by searching through recent inserts
        // We'll look in the tail block where new students are usually added
        long tail_block = getHeader(F, "tail");
        ReadBlock(F, tail_block, &buffer);
        
        // Find the last inserted student (should be our new student)
        Pr_cor new_coords;
        int found_student_id = 0;
        
        for (int i = buffer.Nb - 1; i >= 0; i--)
        {
            rec student = buffer.tab[i];
            if (student.Student_ID != -1 && // Not deleted
                strcmp(student.Family_Name, Family_Name) == 0 &&
                strcmp(student.First_Name, First_Name) == 0)
            {
                found_student_id = student.Student_ID;
                new_coords.block_number = tail_block;
                new_coords.offset = i;
                break;
            }
        }
        
        Close(F);
        
        // If we found the student, insert into secondary indexes
        if (found_student_id > 0)
        {
            // Calculate index values for secondary indexes
            int blood_value = 0;
            int speciality_value = 0;
            int year_study_value = 0;
            int year_birth_value = Date_Birth.year;
            int month_birth_value = Date_Birth.month;
            int day_birth_value = Date_Birth.day;
            int gender_value = Gender;
            int wilaya_value = 0;
            
            // Find blood type value
            for (int i = 0; i < 8; i++)
            {
                if (strcmp(Blood_Type, Blood_Types[i].name) == 0)
                {
                    blood_value = i + 1;
                    break;
                }
            }
            
            // Find year of study value
            for (int i = 0; i < 5; i++)
            {
                if (strcmp(Year_Study, specs[i].years) == 0)
                {
                    year_study_value = i + 1;
                    break;
                }
            }
            
            // Find speciality value
            // For 1CP, 2CP, 1CS - speciality value is 0
            if (strcmp(Year_Study, "1CP") == 0 || 
                strcmp(Year_Study, "2CP") == 0 ||
                strcmp(Year_Study, "1CS") == 0)
            {
                speciality_value = 0;
            }
            else
            {
                // For 2CS and 3CS, find the speciality index
                int year_index = year_study_value - 1; // Convert to 0-based
                for (int j = 0; j < 10; j++)
                {
                    if (strlen(specs[year_index].subspec[j].name) > 0 && 
                        strcmp(Speciality_name, specs[year_index].subspec[j].name) == 0)
                    {
                        speciality_value = j + 1;
                        break;
                    }
                }
            }
            
            // Find wilaya value
            for (int i = 0; i < 58; i++)
            {
                if (strcmp(Wilaya_Birth, wilayas[i].name) == 0)
                {
                    wilaya_value = i + 1;
                    break;
                }
            }
            
            // Insert into ALL secondary indexes
            Insert_index(index_BooldType, &index_size2, blood_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_Speciality, &index_size7, speciality_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_YearStudy, &index_size6, year_study_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_Yearbirth, &index_size3, year_birth_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_Monthbirth, &index_size4, month_birth_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_Datebirth, &index_size1, day_birth_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_Gender, &index_size5, gender_value, new_coords.block_number, new_coords.offset);
            Insert_index(index_Wilayabirth, &index_size8, wilaya_value, new_coords.block_number, new_coords.offset);
            
            // Also insert into CP index if student is 1CP or 2CP
            if (strcmp(Year_Study, "1CP") == 0 || strcmp(Year_Study, "2CP") == 0)
            {
                // We need to read the student record to insert into CP index
                Open(&F, "STUDENTS_ESI.bin", 'E');
                ReadBlock(F, new_coords.block_number, &buffer);
                rec student_record = buffer.tab[new_coords.offset];
                Close(F);
                
                Insert_index_CP(index_CP, &index_size_CP, found_student_id, 
                               student_record, new_coords.block_number, new_coords.offset);
            }
        }

        // Save ALL updated index tables including CP index
        int C31 = 0;
        save_indextable(index_pr, index_size, fname1, &C31);
        int cost;
        save_indextable(index_BooldType, index_size2, fname7, &cost);
        save_indextable(index_Datebirth, index_size1, fname3, &cost);
        save_indextable(index_Yearbirth, index_size3, fname2, &cost);
        save_indextable(index_Monthbirth, index_size4, fname4, &cost);
        save_indextable(index_Gender, index_size5, fname6, &cost);
        save_indextable(index_YearStudy, index_size6, fname8, &cost);
        save_indextable(index_Speciality, index_size7, fname9, &cost);
        save_indextable(index_Wilayabirth, index_size8, fname5, &cost);
        save_indextable(index_CP, index_size_CP, fname10, &cost); // Save CP index

        // Success message
        y += 2;
        gotoxy(center_x - 15, y);
        set_color(COLOR_GREEN);
        printf("User inserted successfully!");

        // Show cost
        y += 1;
        gotoxy(center_x - 15, y);
        set_color(COLOR_CYAN);
        printf("Insertion cost C34: %d", C34);
        
        // Show student ID if found
        if (found_student_id > 0)
        {
            y += 1;
            gotoxy(center_x - 15, y);
            set_color(COLOR_WHITE);
            printf("Student ID: %d", found_student_id);
            
            y += 1;
            gotoxy(center_x - 15, y);
            printf("Added to all index tables.");
        }
    }
    else
    {
        // Cancellation message
        y += 2;
        gotoxy(center_x - 10, y);
        set_color(COLOR_RED);
        printf("Insertion cancelled.");
    }

    // Wait for user to press Enter
    y += 3;
    gotoxy(center_x - 15, y);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");

    while (getchar() != '\n')
        ;
}

void display_welcome_screen()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Calculate position - start at 30% from top (70% visible space above)
    int start_y = 12; // Changed to 1/3 from top instead of center

    // Top border
    gotoxy(center_x - 30, start_y);
    set_color(COLOR_CYAN);
    printf("==============================================================");

    // School name - centered
    gotoxy(center_x - 18, start_y + 2);
    set_color(COLOR_YELLOW);
    printf("ECOLE NATIONALE SUPERIEURE D'INFORMATIQUE");

    gotoxy(center_x - 2, start_y + 3);
    set_color(COLOR_YELLOW);
    printf("(ESI)");

    // Project title
    gotoxy(center_x - 28, start_y + 5);
    set_color(COLOR_YELLOW);
    printf("LOF DATA FILE MANAGEMENT SYSTEM - PRACTICAL WORK FSDS");

    // Separator
    gotoxy(center_x - 30, start_y + 6);
    set_color(COLOR_CYAN);
    printf("--------------------------------------------------------------");

    // Teacher information
    gotoxy(center_x - 15, start_y + 8);
    set_color(COLOR_WHITE);
    printf("Supervised by: ");
    set_color(COLOR_CYAN);
    printf("Mr. KERMI Adel");

    gotoxy(center_x - 15, start_y + 9);
    set_color(COLOR_WHITE);
    printf("Academic Year: ");
    set_color(COLOR_CYAN);
    printf("2024-2025");

    // Students information
    gotoxy(center_x - 15, start_y + 11);
    set_color(COLOR_WHITE);
    printf("Done by:");

    gotoxy(center_x - 10, start_y + 12);
    set_color(COLOR_CYAN);
    printf("SIFI Mohamed");

    gotoxy(center_x - 10, start_y + 13);
    printf("BOUDEHANE Mohamed Adem");

    // Bottom border
    gotoxy(center_x - 30, start_y + 15);
    set_color(COLOR_CYAN);
    printf("==============================================================");

    // Continue prompt
    gotoxy(center_x - 20, start_y + 17);
    set_color(COLOR_YELLOW);
    printf("Press ");
    set_color(COLOR_GREEN);
    printf("ENTER");
    set_color(COLOR_YELLOW);
    printf(" to continue to main menu...");

    // Wait for Enter key
    show_cursor();
#ifdef _WIN32
    while (_getch() != 13)
        ;
#else
    system("stty raw");
    while (getchar() != '\n')
        ;
    system("stty cooked");
#endif
}

void display_main_menu()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FIRST FRAME: "SELECT AN OPTION" ==========
    int title_frame_y = 7; // Row for title frame
    int title_frame_width = 40;
    int title_frame_x = center_x - (title_frame_width / 2);

    // Draw top border of title frame
    gotoxy(title_frame_x, title_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of title frame
    gotoxy(title_frame_x, title_frame_y + 1);
    printf("|");
    gotoxy(title_frame_x + title_frame_width - 1, title_frame_y + 1);
    printf("|");

    // Draw title inside the frame
    gotoxy(center_x - 14, title_frame_y + 1);
    set_color(COLOR_CYAN);
    printf("Main MENU - Choos an Option -");

    // Draw bottom border of title frame
    gotoxy(title_frame_x, title_frame_y + 2);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SECOND FRAME: MENU OPTIONS ==========
    int menu_frame_y = title_frame_y + 4; // Leave space between frames
    int menu_frame_width = 70;            // Wider for options
    int menu_frame_x = center_x - (menu_frame_width / 2);

    // Draw top border of menu frame
    gotoxy(menu_frame_x, menu_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of menu frame
    for (int i = 0; i < 15; i++)
    {
        gotoxy(menu_frame_x, menu_frame_y + 1 + i);
        printf("|");
        gotoxy(menu_frame_x + menu_frame_width - 1, menu_frame_y + 1 + i);
        printf("|");
    }

    // Draw bottom border of menu frame
    gotoxy(menu_frame_x, menu_frame_y + 16);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Menu options inside the second frame
    int start_y = menu_frame_y + 2; // Start inside the menu frame
    char *menu_options[] = {
        "1. File Creation & Initial Loading",
        "2. View All File Contents",
        "3. Filter Students by Blood Type / Year / Speciality ..",
        "4. Manage Students File & View Index",
        "5. Create LOF File (STUDENTS_CP.BIN)",
        "6. Optional Operations",
        "7. Exit Program"};

    int num_options = 7;

    while (1)
    {
        // Display all options with ENHANCED highlighting
        for (int i = 0; i < num_options; i++)
        {
            // Calculate position to center within the menu frame
            int option_x = menu_frame_x + 6; // Padding from left border

            // Clear the line first (for highlighting)
            gotoxy(option_x - 2, start_y + i * 2);
            for (int j = 0; j < menu_frame_width - 10; j++)
                printf(" "); // Clear with spaces

            // Now print the option with proper highlighting
            gotoxy(option_x, start_y + i * 2);

            if (i == current_main_menu_selection)
            {
                // ENHANCED: Selected option gets highlighted in CYAN
                if (i == num_options - 1)
                {
                    // Exit option when selected - RED
                    set_color(COLOR_BLUE);
                    printf("> ");
                    set_color(COLOR_BLUE);
                }
                else
                {
                    // Regular option when selected - CYAN
                    set_color(COLOR_YELLOW);
                    printf("> ");
                    set_color(COLOR_YELLOW);
                }

                printf("%s", menu_options[i]);

                // Add padding to fill the frame
                int option_length = strlen(menu_options[i]);
                int padding_needed = menu_frame_width - 12 - option_length;
                for (int p = 0; p < padding_needed; p++)
                {
                    printf(" ");
                }
            }
            else
            {
                printf("  "); // Regular padding for non-selected

                // Default: All options are WHITE when not selected
                set_color(COLOR_WHITE);

                // Except exit option is RED even when not selected
                if (i == num_options - 1)
                {
                    set_color(COLOR_WHITE);
                }

                printf("%s", menu_options[i]);
            }
        }

        // Footer - Make it narrower too
        gotoxy(center_x - 30, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < 60; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 21, height - 2);
        set_color(COLOR_WHITE);
        printf("Use ");
        set_color(COLOR_GREEN);
        printf("ARROW KEYS");
        set_color(COLOR_WHITE);
        printf(" to navigate, ");
        set_color(COLOR_GREEN);
        printf("ENTER");
        set_color(COLOR_WHITE);
        printf(" to select");

        // Get key input - Keep your original logic
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && current_main_menu_selection > 0)
            {
                current_main_menu_selection--;
            }
            else if (key == 80 && current_main_menu_selection < num_options - 1)
            {
                current_main_menu_selection++;
            }
        }
        else if (key == 13)
        {
            int selection = current_main_menu_selection + 1;

            switch (selection)
            {
            case 1:
                display_file_operations_menu();
                current_main_menu_selection = 0;
                return;
            case 2:
                CLEAR_SCREEN();
                t_LnOVS *F;
                display_all_contents_ui(F, "STUDENTS_ESI.bin");
                current_main_menu_selection = 0;
                return;
            case 3:
                display_search_operations_menu_blood_year();
                current_main_menu_selection = 0;
                return;
            case 4:
                display_search_operations_menu();
                current_main_menu_selection = 0;
                return;
            case 5:
                create_lof_file_workflow();
                current_main_menu_selection = 0;
                return;
            case 6:
                display_optional_operations_menu();
                current_main_menu_selection = 0;
                return;
            case 7:
                display_exit_confirmation();
                current_main_menu_selection = 0;
                return;
            }
        }
        else if (key == 27)
        {
            display_exit_confirmation();
            return;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar();
            key = getchar();
            system("stty cooked");

            if (key == 65 && current_main_menu_selection > 0)
            {
                current_main_menu_selection--;
            }
            else if (key == 66 && current_main_menu_selection < num_options - 1)
            {
                current_main_menu_selection++;
            }
        }
        else if (key == 10)
        {
            int selection = current_main_menu_selection + 1;

            switch (selection)
            {
            case 7:
                display_exit_confirmation();
                current_main_menu_selection = 0;
                return;
            default:
                CLEAR_SCREEN();
                printf("\nOption %d selected...\n", selection);
                operation_completed_prompt(1);
                current_main_menu_selection = 0;
                return;
            }
        }
        else if (key == 27)
        {
            display_exit_confirmation();
            return;
        }
#endif
    }
}
/**
 * Complete workflow for initial loading of database
 */
void initial_loading_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // Step 1: Get number of records
    gotoxy(center_x - 20, 3);
    set_color(COLOR_CYAN);
    printf("INITIAL DATABASE CREATION");

    gotoxy(center_x - 30, 6);
    set_color(COLOR_WHITE);
    printf("============================================================");

    gotoxy(center_x - 20, 8);
    set_color(COLOR_YELLOW);
    printf("Enter number of students to generate: ");

    set_color(COLOR_CYAN);
    show_cursor();
    int N;
    scanf("%d", &N);
    getchar();
    hide_cursor();

    if (N <= 0)
    {
        gotoxy(center_x - 15, 12);
        set_color(COLOR_RED);
        printf("Number must be greater than 0!");

        gotoxy(center_x - 15, 14);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Step 2: Confirmation
    CLEAR_SCREEN();
    gotoxy(center_x - 20, 3);
    set_color(COLOR_MAGENTA);
    printf("CONFIRM DATABASE CREATION");

    gotoxy(center_x - 30, 6);
    set_color(COLOR_CYAN);
    printf("============================================================");

    int y = 8;

    // Display loading details
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("File to create: ");
    set_color(COLOR_CYAN);
    printf("STUDENTS_ESI.bin");

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Number of students: ");
    set_color(COLOR_CYAN);
    printf("%d", N);

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Records per block: ");
    set_color(COLOR_CYAN);
    printf("40");

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Estimated blocks: ");
    set_color(COLOR_CYAN);
    printf("%d", (N + 39) / 40);

    y += 1;

    gotoxy(center_x - 25, y++);
    set_color(COLOR_BLUE);
    printf("WARNING: Existing STUDENTS_ESI.bin will be overwritten!");

    y += 2;

    gotoxy(center_x - 20, y++);
    set_color(COLOR_WHITE);
    printf("Create database? (Y/N): ");

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        // Clear screen for loading
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 5);
        set_color(COLOR_CYAN);
        printf("GENERATING DATABASE...");

        gotoxy(center_x - 30, 8);
        set_color(COLOR_CYAN);
        printf("============================================================");

        // Show progress
        gotoxy(center_x - 25, 10);
        set_color(COLOR_WHITE);
        printf("Creating %d student records", N);

        gotoxy(center_x - 25, 12);
        printf("Progress: [");
        // Initial empty progress bar
        for (int i = 0; i < 20; i++)
            printf(".");
        printf("]");

        // Perform initial loading
        initial_loading(N, &C2);

        // Update progress bar
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        for (int i = 0; i < 20; i++)
            printf("#");
        set_color(COLOR_WHITE);

        // Success message
        gotoxy(center_x - 30, 15);
        set_color(COLOR_CYAN);
        printf("============================================================");

        gotoxy(center_x - 15, 17);
        set_color(COLOR_GREEN);
        printf("DATABASE CREATED SUCCESSFULLY!");

        gotoxy(center_x - 15, 19);
        set_color(COLOR_WHITE);
        printf("Creation cost C2: ");
        set_color(COLOR_CYAN);
        printf("%d", C2);

        gotoxy(center_x - 15, 20);
        set_color(COLOR_WHITE);
        printf("Index table size: ");
        set_color(COLOR_CYAN);
        printf("%d records", index_size);

        gotoxy(center_x - 15, 21);
        set_color(COLOR_WHITE);
        printf("Secondary indexes: ");
        set_color(COLOR_CYAN);
        printf("8 files created");
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Creation cancelled.");
    }

    // Wait for user
    gotoxy(center_x - 15, y + 6);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");
    getchar();
}

/**
 * Complete workflow for saving all index files
 */
void save_index_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // Check if index is empty
    if (index_size == 0)
    {
        gotoxy(center_x - 15, 8);
        set_color(COLOR_RED);
        printf("Index table is empty!");

        gotoxy(center_x - 20, 10);
        set_color(COLOR_YELLOW);
        printf("Please create or load an index first.");

        gotoxy(center_x - 15, 12);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Show current index status
    gotoxy(center_x - 20, 3);
    set_color(COLOR_CYAN);
    printf("SAVE ALL INDEX FILES");

    gotoxy(center_x - 30, 6);
    set_color(COLOR_CYAN);
    printf("============================================================");

    int y = 8;

    // Display index information
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Primary Index: ");
    set_color(COLOR_GREEN);
    printf("%d records", index_size);

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("File: ");
    set_color(COLOR_GREEN);
    printf("StudentID_INDEX.idx");

    y += 1;

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Secondary Indexes: 10 files total");

    if (index_size1 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- DateBirth_INDEX.idx: %d records", index_size1);
    }

    if (index_size2 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- BloodType_INDEX.idx: %d records", index_size2);
    }

    if (index_size3 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- YearBirth_INDEX.idx: %d records", index_size3);
    }

    if (index_size4 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- MonthBirth_INDEX.idx: %d records", index_size4);
    }

    if (index_size5 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- Gender_INDEX.idx: %d records", index_size5);
    }

    if (index_size6 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- YearStudy_INDEX.idx: %d records", index_size6);
    }

    if (index_size7 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- Speciality_INDEX.idx: %d records", index_size7);
    }

    if (index_size8 > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- WilayaBirth_INDEX.idx: %d records", index_size8);
    }

    if (index_size_CP > 0)
    {
        gotoxy(center_x - 22, y++);
        set_color(COLOR_CYAN);
        printf("- STUDENTS_CP_INDEX.idx: %d records (CP Students)", index_size_CP);
    }

    y += 2;

    gotoxy(center_x - 25, y++);
    set_color(COLOR_BLUE);
    printf("WARNING: Existing index files will be overwritten!");

    y += 2;

    gotoxy(center_x - 20, y++);
    set_color(COLOR_WHITE);
    printf("Save all 10 index files? (Y/N): ");

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        // Clear screen for saving
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 5);
        set_color(COLOR_CYAN);
        printf("SAVING INDEX FILES...");

        gotoxy(center_x - 30, 8);
        set_color(COLOR_CYAN);
        printf("============================================================");

        // Show progress
        gotoxy(center_x - 25, 10);
        set_color(COLOR_WHITE);
        printf("Saving 10 index files to disk");

        gotoxy(center_x - 25, 12);
        printf("Progress: [");
        // Initial empty progress bar
        for (int i = 0; i < 20; i++)
            printf(".");
        printf("]");

        // Save all indexes
        int cost;
        save_indextable(index_pr, index_size, fname1, &C31);

        // Update progress bar in steps
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        printf("####"); // 4/10 files
        set_color(COLOR_WHITE);

        // Save secondary indexes
        save_indextable(index_BooldType, index_size2, fname7, &cost);
        save_indextable(index_Datebirth, index_size1, fname3, &cost);
        save_indextable(index_Yearbirth, index_size3, fname2, &cost);
        save_indextable(index_Monthbirth, index_size4, fname4, &cost);

        gotoxy(center_x - 25 + 11 + 4, 12);
        set_color(COLOR_GREEN);
        printf("########"); // 8 more files
        set_color(COLOR_WHITE);

        save_indextable(index_Gender, index_size5, fname6, &cost);
        save_indextable(index_YearStudy, index_size6, fname8, &cost);
        save_indextable(index_Speciality, index_size7, fname9, &cost);
        save_indextable(index_Wilayabirth, index_size8, fname5, &cost);

        // Save CP index
        save_indextable(index_CP, index_size_CP, fname10, &cost);

        gotoxy(center_x - 25 + 11 + 12, 12);
        set_color(COLOR_GREEN);
        printf("########"); // Complete the bar
        set_color(COLOR_WHITE);

        // Success message
        gotoxy(center_x - 30, 15);
        set_color(COLOR_CYAN);
        printf("============================================================");

        gotoxy(center_x - 15, 17);
        set_color(COLOR_GREEN);
        printf("INDEX FILES SAVED SUCCESSFULLY!");

        gotoxy(center_x - 15, 19);
        set_color(COLOR_WHITE);
        printf("Save cost C31: ");
        set_color(COLOR_CYAN);
        printf("%d", C31);

        gotoxy(center_x - 15, 20);
        set_color(COLOR_WHITE);
        printf("Files saved: ");
        set_color(COLOR_CYAN);
        printf("10 files");
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Save cancelled.");
    }

    // Wait for user
    gotoxy(center_x - 15, y + 6);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");
    getchar();
}

/**
 * Complete workflow for loading all index files
 */
void load_index_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // Confirmation screen
    gotoxy(center_x - 20, 3);
    set_color(COLOR_CYAN);
    printf("LOAD ALL INDEX FILES");

    gotoxy(center_x - 30, 6);
    set_color(COLOR_CYAN);
    printf("============================================================");

    int y = 8;

    // Display what will be loaded
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Files to load (10 files total):");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("1. StudentID_INDEX.idx (Primary)");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("2. DateBirth_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("3. BloodType_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("4. YearBirth_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("5. MonthBirth_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("6. Gender_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("7. YearStudy_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("8. Speciality_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("9. WilayaBirth_INDEX.idx");

    gotoxy(center_x - 22, y++);
    set_color(COLOR_CYAN);
    printf("10. STUDENTS_CP_INDEX.idx (CP Students)");

    y += 2;

    gotoxy(center_x - 25, y++);
    set_color(COLOR_YELLOW);
    printf("WARNING: Current index data will be replaced!");

    y += 2;

    gotoxy(center_x - 20, y++);
    set_color(COLOR_WHITE);
    printf("Load all 10 index files? (Y/N): ");

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        // Clear screen for loading
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 5);
        set_color(COLOR_CYAN);
        printf("LOADING INDEX FILES...");

        gotoxy(center_x - 30, 8);
        set_color(COLOR_CYAN);
        printf("============================================================");

        // Show progress
        gotoxy(center_x - 25, 10);
        set_color(COLOR_WHITE);
        printf("Loading 10 index files from disk");

        gotoxy(center_x - 25, 12);
        printf("Progress: [");
        // Initial empty progress bar
        for (int i = 0; i < 20; i++)
            printf(".");
        printf("]");

        // Load all indexes
        int cost1;
        loadindextable(fname1, index_pr, &index_size, &C32);

        // Update progress bar in steps
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        printf("####"); // 4/10 files
        set_color(COLOR_WHITE);

        // Load secondary indexes
        loadindextable(fname3, index_Datebirth, &index_size1, &cost1);
        loadindextable(fname7, index_BooldType, &index_size2, &cost1);
        loadindextable(fname2, index_Yearbirth, &index_size3, &cost1);
        loadindextable(fname4, index_Monthbirth, &index_size4, &cost1);

        gotoxy(center_x - 25 + 11 + 4, 12);
        set_color(COLOR_GREEN);
        printf("########"); // 8 more files
        set_color(COLOR_WHITE);

        loadindextable(fname6, index_Gender, &index_size5, &cost1);
        loadindextable(fname8, index_YearStudy, &index_size6, &cost1);
        loadindextable(fname9, index_Speciality, &index_size7, &cost1);
        loadindextable(fname5, index_Wilayabirth, &index_size8, &cost1);

        // Load CP index
        loadindextable(fname10, index_CP, &index_size_CP, &cost1);

        gotoxy(center_x - 25 + 11 + 12, 12);
        set_color(COLOR_GREEN);
        printf("########"); // Complete the bar
        set_color(COLOR_WHITE);

        // Success message
        gotoxy(center_x - 30, 15);
        set_color(COLOR_CYAN);
        printf("============================================================");

        gotoxy(center_x - 15, 17);
        set_color(COLOR_GREEN);
        printf("INDEX FILES LOADED SUCCESSFULLY!");

        gotoxy(center_x - 15, 19);
        set_color(COLOR_WHITE);
        printf("Load cost C32: ");
        set_color(COLOR_CYAN);
        printf("%d", C32);

        gotoxy(center_x - 15, 20);
        set_color(COLOR_WHITE);
        printf("Primary index size: ");
        set_color(COLOR_CYAN);
        printf("%d records", index_size);

        gotoxy(center_x - 15, 21);
        set_color(COLOR_WHITE);
        printf("Secondary indexes: ");
        set_color(COLOR_CYAN);
        printf("9 files loaded");

        if (index_size_CP > 0)
        {
            gotoxy(center_x - 15, 22);
            set_color(COLOR_WHITE);
            printf("CP students index: ");
            set_color(COLOR_CYAN);
            printf("%d records", index_size_CP);
        }
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Load cancelled.");
    }

    // Wait for user
    gotoxy(center_x - 15, y + 6);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");
    getchar();
}

void display_file_operations_menu()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FIRST FRAME: "SELECT AN OPTION" ==========
    int title_frame_y = 7;
    int title_frame_width = 50;
    int title_frame_x = center_x - (title_frame_width / 2);

    // Draw top border of title frame
    gotoxy(title_frame_x, title_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of title frame
    gotoxy(title_frame_x, title_frame_y + 1);
    printf("|");
    gotoxy(title_frame_x + title_frame_width - 1, title_frame_y + 1);
    printf("|");

    // Draw title inside the frame
    gotoxy(center_x - 17, title_frame_y + 1);
    set_color(COLOR_CYAN);
    printf("FILE OPERATIONS - Choose an Option");

    // Draw bottom border of title frame
    gotoxy(title_frame_x, title_frame_y + 2);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SECOND FRAME: MENU OPTIONS ==========
    int menu_frame_y = title_frame_y + 4;
    int menu_frame_width = 70;
    int menu_frame_x = center_x - (menu_frame_width / 2);

    // Draw top border of menu frame
    gotoxy(menu_frame_x, menu_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of menu frame
    for (int i = 0; i < 13; i++)
    {
        gotoxy(menu_frame_x, menu_frame_y + 1 + i);
        printf("|");
        gotoxy(menu_frame_x + menu_frame_width - 1, menu_frame_y + 1 + i);
        printf("|");
    }

    // Draw bottom border of menu frame
    gotoxy(menu_frame_x, menu_frame_y + 14);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Menu options inside the second frame
    int start_y = menu_frame_y + 2;
    char *submenu_options[] = {
        "1. Create STUDENTS_ESI.BIN with N records",
        "2. Display creation cost (C2)",
        "3. Save index to StudentID_INDEX.idx",
        "4. Load index from StudentID_INDEX.idx",
        "5. Return to Main Menu"};

    int num_options = 5;
    current_submenu_selection = 0;

    while (1)
    {
        // Display all options
        for (int i = 0; i < num_options; i++)
        {
            int option_x = menu_frame_x + 6;

            // Clear the line first
            gotoxy(option_x - 2, start_y + i * 2);
            for (int j = 0; j < menu_frame_width - 10; j++)
                printf(" ");

            // Print the option with proper highlighting
            gotoxy(option_x, start_y + i * 2);

            if (i == current_submenu_selection)
            {
                if (i == num_options - 1)
                {
                    set_color(COLOR_BLUE);
                    printf("> ");
                    set_color(COLOR_BLUE);
                }
                else
                {
                    set_color(COLOR_YELLOW);
                    printf("> ");
                    set_color(COLOR_YELLOW);
                }

                printf("%s", submenu_options[i]);

                int option_length = strlen(submenu_options[i]);
                int padding_needed = menu_frame_width - 12 - option_length;
                for (int p = 0; p < padding_needed; p++)
                {
                    printf(" ");
                }
            }
            else
            {
                printf("  ");

                set_color(COLOR_WHITE);

                if (i == num_options - 1)
                {
                    set_color(COLOR_WHITE);
                }

                printf("%s", submenu_options[i]);
            }
        }

        // Footer
        gotoxy(center_x - 30, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < 60; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 10, height - 2);
        set_color(COLOR_WHITE);
        printf("Press ");
        set_color(COLOR_RED);
        printf("ESC");
        set_color(COLOR_WHITE);
        printf(" to go back");

        // Get key input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && current_submenu_selection > 0)
            {
                current_submenu_selection--;
            }
            else if (key == 80 && current_submenu_selection < num_options - 1)
            {
                current_submenu_selection++;
            }
        }
        else if (key == 13)
        {
            int selection = current_submenu_selection + 1;

            switch (selection)
            {
            case 1:
                initial_loading_workflow();
                operation_completed_prompt(0);
                display_file_operations_menu();
                return;

            case 2:
            {
                CLEAR_SCREEN();
                printf("\n=== CREATION COST (C2) ===\n\n");

                if (C2 == 0)
                {
                    printf("No file has been created yet.\n");
                    printf("Please create a file first using option 1.\n");
                }
                else
                {
                    printf("The creation cost C2 is: %d\n", C2);
                    printf("This represents the number of blocks written.\n");
                }

                operation_completed_prompt(0);
                display_file_operations_menu();
                return;
            }

            case 3:
                save_index_workflow();
                operation_completed_prompt(0);
                display_file_operations_menu();
                return;

            case 4:
                load_index_workflow();
                operation_completed_prompt(0);
                display_file_operations_menu();
                return;

            case 5:
                return;
            }
        }
        else if (key == 27)
        {
            return;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar();
            key = getchar();
            system("stty cooked");

            if (key == 65 && current_submenu_selection > 0)
            {
                current_submenu_selection--;
            }
            else if (key == 66 && current_submenu_selection < num_options - 1)
            {
                current_submenu_selection++;
            }
        }
        else if (key == 10)
        {
            int selection = current_submenu_selection + 1;

            switch (selection)
            {
            case 5:
                return;
            default:
                printf("\nOption %d selected...\n", selection);
                operation_completed_prompt(0);
                display_file_operations_menu();
                return;
            }
        }
        else if (key == 27)
        {
            return;
        }
#endif
    }
}

void display_search_operations_menu()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FIRST FRAME: "SELECT AN OPTION" ==========
    int title_frame_y = 7;
    int title_frame_width = 50;
    int title_frame_x = center_x - (title_frame_width / 2);

    // Draw top border of title frame
    gotoxy(title_frame_x, title_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of title frame
    gotoxy(title_frame_x, title_frame_y + 1);
    printf("|");
    gotoxy(title_frame_x + title_frame_width - 1, title_frame_y + 1);
    printf("|");

    // Draw title inside the frame
    gotoxy(center_x - 17, title_frame_y + 1);
    set_color(COLOR_CYAN);
    printf("SEARCH & ANALYSIS - Choose an Option");

    // Draw bottom border of title frame
    gotoxy(title_frame_x, title_frame_y + 2);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SECOND FRAME: MENU OPTIONS ==========
    int menu_frame_y = title_frame_y + 4;
    int menu_frame_width = 70;
    int menu_frame_x = center_x - (menu_frame_width / 2);

    // Draw top border of menu frame
    gotoxy(menu_frame_x, menu_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of menu frame
    for (int i = 0; i < 15; i++)
    {
        gotoxy(menu_frame_x, menu_frame_y + 1 + i);
        printf("|");
        gotoxy(menu_frame_x + menu_frame_width - 1, menu_frame_y + 1 + i);
        printf("|");
    }

    // Draw bottom border of menu frame
    gotoxy(menu_frame_x, menu_frame_y + 16);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Menu options inside the second frame
    int start_y = menu_frame_y + 2;
    char *submenu_options[] = {
        "1. Search Student by ID",
        "2. Insert New Student",
        "3. Display Index Table",
        "4. Delete Student ",
        "5. Modify Student's First Name",
        "6. Return to Main Menu"};

    int num_options = 6;
    current_submenu_selection = 0;

    while (1)
    {
        // Display options with highlighting
        for (int i = 0; i < num_options; i++)
        {
            int option_x = menu_frame_x + 6;

            gotoxy(option_x - 2, start_y + i * 2);
            for (int j = 0; j < menu_frame_width - 10; j++)
                printf(" ");

            gotoxy(option_x, start_y + i * 2);

            if (i == current_submenu_selection)
            {
                if (i == num_options - 1)
                {
                    set_color(COLOR_BLUE);
                    printf("> ");
                    set_color(COLOR_BLUE);
                }
                else
                {
                    set_color(COLOR_YELLOW);
                    printf("> ");
                    set_color(COLOR_YELLOW);
                }

                printf("%s", submenu_options[i]);

                int option_length = strlen(submenu_options[i]);
                int padding_needed = menu_frame_width - 12 - option_length;
                for (int p = 0; p < padding_needed; p++)
                {
                    printf(" ");
                }
            }
            else
            {
                printf("  ");

                set_color(COLOR_WHITE);

                if (i == num_options - 1)
                {
                    set_color(COLOR_WHITE);
                }

                printf("%s", submenu_options[i]);
            }
        }

        // Footer
        gotoxy(center_x - 30, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < 60; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 14, height - 2);
        set_color(COLOR_WHITE);
        printf("Costs: C33, C41, C42, C43, C44");

        // Get key input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && current_submenu_selection > 0)
            {
                current_submenu_selection--;
            }
            else if (key == 80 && current_submenu_selection < num_options - 1)
            {
                current_submenu_selection++;
            }
        }
        else if (key == 13)
        {
            int selection = current_submenu_selection + 1;

            switch (selection)
            {
            case 1:
            {
                CLEAR_SCREEN();
                printf("\n=== SEARCH STUDENT BY ID ===\n\n");

                int student_id;
                int C33, num_block, offset;
                bool found;
                printf("Enter student ID to search: ");
                scanf("%d", &student_id);
                getchar();

                printf("\nSearching for student ID: %d\n", student_id);
                Search_StuDentID(student_id, index_pr, &found, &num_block, &offset, &C33, index_size);
                printf("\nThe Cost of This Operation is : %d", C33);
                if (!found)
                {
                    printf("\n User not Found ");
                }
                else
                {
                    printf("\nThe User Information found in block number %d, and offset number %d", num_block, offset);
                }
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 2:
            {
                insert_student_workflow();
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 3:
            {
                CLEAR_SCREEN();
                displayPrIndexTableWithData(index_pr, index_size, fname, 4);
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 4:
            {
                CLEAR_SCREEN();
                delete_student_workflow();
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 5:
            {
                CLEAR_SCREEN();
                modify_student_workflow();
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 6:
                return;
            }
        }
        else if (key == 27)
        {
            return;
        }
#else
        return;
#endif
    }
}

void display_search_operations_menu_blood_year()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FIRST FRAME: "SELECT AN OPTION" ==========
    int title_frame_y = 7;
    int title_frame_width = 50;
    int title_frame_x = center_x - (title_frame_width / 2);

    // Draw top border of title frame
    gotoxy(title_frame_x, title_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of title frame
    gotoxy(title_frame_x, title_frame_y + 1);
    printf("|");
    gotoxy(title_frame_x + title_frame_width - 1, title_frame_y + 1);
    printf("|");

    // Draw title inside the frame
    gotoxy(center_x - 17, title_frame_y + 1);
    set_color(COLOR_CYAN);
    printf("FILTER STUDENTS - Choose an Option");

    // Draw bottom border of title frame
    gotoxy(title_frame_x, title_frame_y + 2);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SECOND FRAME: MENU OPTIONS ==========
    int menu_frame_y = title_frame_y + 4;
    int menu_frame_width = 70;
    int menu_frame_x = center_x - (menu_frame_width / 2);

    // Draw top border of menu frame
    gotoxy(menu_frame_x, menu_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of menu frame
    for (int i = 0; i < 13; i++)
    {
        gotoxy(menu_frame_x, menu_frame_y + 1 + i);
        printf("|");
        gotoxy(menu_frame_x + menu_frame_width - 1, menu_frame_y + 1 + i);
        printf("|");
    }

    // Draw bottom border of menu frame
    gotoxy(menu_frame_x, menu_frame_y + 14);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Menu options inside the second frame
    int start_y = menu_frame_y + 2; // Start inside the menu frame
    char *submenu_options[] = {
        "1. Display students by blood type (C41)",
        "2. Display students in a given speciality (C42)",
        "3. Display students under 20 in year range (C43)",
        "4. Display students by study year (C44)",
        "5. Return to Main Menu"};

    int num_options = 5;
    current_submenu_selection = 0;

    while (1)
    {
        // Display all options with ENHANCED highlighting
        for (int i = 0; i < num_options; i++)
        {
            // Calculate position to center within the menu frame
            int option_x = menu_frame_x + 6; // Padding from left border

            // Clear the line first (for highlighting)
            gotoxy(option_x - 2, start_y + i * 2);
            for (int j = 0; j < menu_frame_width - 10; j++)
                printf(" "); // Clear with spaces

            // Now print the option with proper highlighting
            gotoxy(option_x, start_y + i * 2);

            if (i == current_submenu_selection)
            {
                // ENHANCED: Selected option gets highlighted
                if (i == num_options - 1)
                {
                    // Return option when selected - BLUE
                    set_color(COLOR_BLUE);
                    printf("> ");
                    set_color(COLOR_BLUE);
                }
                else
                {
                    // Regular option when selected - YELLOW
                    set_color(COLOR_YELLOW);
                    printf("> ");
                    set_color(COLOR_YELLOW);
                }

                printf("%s", submenu_options[i]);

                // Add padding to fill the frame
                int option_length = strlen(submenu_options[i]);
                int padding_needed = menu_frame_width - 12 - option_length;
                for (int p = 0; p < padding_needed; p++)
                {
                    printf(" ");
                }
            }
            else
            {
                printf("  "); // Regular padding for non-selected

                // Default: All options are WHITE when not selected
                set_color(COLOR_WHITE);

                // Return option is also white when not selected
                if (i == num_options - 1)
                {
                    set_color(COLOR_WHITE);
                }

                printf("%s", submenu_options[i]);
            }
        }

        // Footer - Make it narrower too
        gotoxy(center_x - 30, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < 61; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 29, height - 2);
        set_color(COLOR_WHITE);
        printf("Use ");
        set_color(COLOR_GREEN);
        printf("ARROW KEYS");
        set_color(COLOR_WHITE);
        printf(" to navigate, ");
        set_color(COLOR_GREEN);
        printf("ENTER");
        set_color(COLOR_WHITE);
        printf(" to select, ");
        set_color(COLOR_RED);
        printf("ESC");
        set_color(COLOR_WHITE);
        printf(" to go back");

        // Get key input - Keep your original logic
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && current_submenu_selection > 0)
            {
                current_submenu_selection--;
            }
            else if (key == 80 && current_submenu_selection < num_options - 1)
            {
                current_submenu_selection++;
            }
        }
        else if (key == 13)
        {
            int selection = current_submenu_selection + 1;

            switch (selection)
            {
            case 1: // Display students by blood type (C41)
            {
                CLEAR_SCREEN();

                // Show blood type selection menu
                int blood_choice = select_blood_type(8, center_x);
                if (blood_choice == -1)
                {
                    display_search_operations_menu_blood_year();
                    return;
                }

                int C41 = 0;
                display_blood_type_ui(index_BooldType, index_size2, blood_choice + 1, fname, &C41);

                display_search_operations_menu_blood_year();
                return;
            }

            case 2: // Display students in a given speciality (C42)
            {
                int C42 = 0;
                display_speciality_ui(index_Speciality, index_size7, fname, &C42);

                display_search_operations_menu_blood_year();
                return;
            }

            case 3: // Display students under 20 in year range (C43)
            {
                CLEAR_SCREEN();

                // Get year range input
                gotoxy(center_x - 20, 8);
                set_color(COLOR_YELLOW);
                printf("Enter start year (2003-2008): ");
                show_cursor();
                int Y1;
                scanf("%d", &Y1);
                getchar();

                gotoxy(center_x - 20, 10);
                printf("Enter end year (2003-2008): ");
                int Y2;
                scanf("%d", &Y2);
                getchar();

                int C43 = 0;
                display_birth_interval_ui(index_Yearbirth, index_size3, Y1, Y2, fname, &C43);

                display_search_operations_menu_blood_year();
                return;
            }

            case 4: // Display students by study year (C44)
            {
                CLEAR_SCREEN();

                const char *year_options[] = {"1CP", "2CP", "1CS", "2CS", "3CS"};
                int year_choice = select_from_list("SELECT YEAR OF STUDY", year_options, 5, 8, center_x);
                if (year_choice == -1)
                {
                    display_search_operations_menu_blood_year();
                    return;
                }

                int C44 = 0;
                display_year_study_ui(index_YearStudy, index_size6, year_choice + 1, fname, &C44);

                display_search_operations_menu_blood_year();
                return;
            }

            case 5: // Return to Main Menu
                return;
            }
        }
        else if (key == 27) // ESC key
        {
            return;
        }
#else
        return;
#endif
    }
}

void display_exit_confirmation()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;
    int center_y = height / 2;

    // ========== FIRST FRAME: EXIT CONFIRMATION TITLE ==========
    int title_frame_y = center_y - 8;
    int title_frame_width = 50;
    int title_frame_x = center_x - (title_frame_width / 2);

    // Draw top border of title frame
    gotoxy(title_frame_x, title_frame_y);
    set_color(COLOR_YELLOW);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of title frame
    gotoxy(title_frame_x, title_frame_y + 1);
    printf("|");
    gotoxy(title_frame_x + title_frame_width - 1, title_frame_y + 1);
    printf("|");

    // Draw title inside the frame
    gotoxy(center_x - 10, title_frame_y + 1);
    set_color(COLOR_YELLOW);
    printf("EXIT CONFIRMATION");

    // Draw bottom border of title frame
    gotoxy(title_frame_x, title_frame_y + 2);
    set_color(COLOR_YELLOW);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SECOND FRAME: CONFIRMATION MESSAGE ==========
    int msg_frame_y = title_frame_y + 4;
    int msg_frame_width = 50;
    int msg_frame_x = center_x - (msg_frame_width / 2);

    // Draw top border of message frame
    gotoxy(msg_frame_x, msg_frame_y);
    set_color(COLOR_YELLOW);
    printf("+");
    for (int i = 0; i < msg_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of message frame
    for (int i = 0; i < 6; i++)
    {
        gotoxy(msg_frame_x, msg_frame_y + 1 + i);
        printf("|");
        gotoxy(msg_frame_x + msg_frame_width - 1, msg_frame_y + 1 + i);
        printf("|");
    }

    // Draw message inside the frame
    gotoxy(center_x - 20, msg_frame_y + 2);
    set_color(COLOR_WHITE);
    printf("Are you sure you want to exit the program?");

    // Warning message
    gotoxy(center_x - 15, msg_frame_y + 4);
    set_color(COLOR_BLUE);
    printf("All unsaved data will be lost!");

    // Draw bottom border of message frame
    gotoxy(msg_frame_x, msg_frame_y + 7);
    set_color(COLOR_YELLOW);
    printf("+");
    for (int i = 0; i < msg_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SELECTION OPTIONS ==========
    int selection_y = msg_frame_y + 9;
    int selection_width = 30;
    int selection_x = center_x - (selection_width / 2);

    // Draw selection box
    gotoxy(selection_x, selection_y);
    set_color(COLOR_YELLOW);
    printf("+");
    for (int i = 0; i < selection_width - 2; i++)
        printf("-");
    printf("+");

    gotoxy(selection_x, selection_y + 1);
    printf("|");
    gotoxy(selection_x + selection_width - 1, selection_y + 1);
    printf("|");

    gotoxy(selection_x, selection_y + 2);
    printf("+");
    for (int i = 0; i < selection_width - 2; i++)
        printf("-");
    printf("+");

    // Selection options
    int selection = 0; // 0 = NO, 1 = YES
    int selected = 0;

    while (!selected)
    {
        // Clear the selection line
        gotoxy(selection_x + 2, selection_y + 1);
        for (int i = 0; i < selection_width - 4; i++)
            printf(" ");

        // Display options based on selection
        gotoxy(selection_x + 8, selection_y + 1);

        if (selection == 1)
        {
            // NO is selected
            set_color(COLOR_GREEN);
            printf("[ ");
            set_color(COLOR_GREEN);
            printf("YES");
            set_color(COLOR_GREEN);
            printf(" ]");

            gotoxy(selection_x + 15, selection_y + 1);
            set_color(COLOR_WHITE);
            printf("[ NO ]");
        }
        else
        {
            // YES is selected
            set_color(COLOR_WHITE);
            printf("[ YES ]");

            gotoxy(selection_x + 15, selection_y + 1);
            set_color(COLOR_BLUE);
            printf("[ ");
            set_color(COLOR_BLUE);
            printf("NO");
            set_color(COLOR_BLUE);
            printf(" ]");
        }

        // ========== NAVIGATION INSTRUCTIONS ==========
        gotoxy(center_x - 15, selection_y + 4);
        set_color(COLOR_YELLOW);
        printf("Use ");
        set_color(COLOR_GREEN);
        printf("LEFT/RIGHT");
        set_color(COLOR_YELLOW);
        printf(" arrows to choose");

        gotoxy(center_x - 10, selection_y + 5);
        printf("Press ");
        set_color(COLOR_GREEN);
        printf("ENTER");
        set_color(COLOR_YELLOW);
        printf(" to confirm");

        // Get input
        show_cursor();
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 75 || key == 77) // Left or Right arrow
            {
                selection = !selection; // Toggle between NO and YES
            }
        }
        else if (key == 13) // Enter
        {
            selected = 1;
        }
        else if (key == 27) // ESC - same as NO
        {
            selection = 0;
            selected = 1;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar();
            key = getchar();
            system("stty cooked");

            if (key == 67 || key == 68) // Right or Left arrow
            {
                selection = !selection;
            }
        }
        else if (key == 10) // Enter
        {
            selected = 1;
        }
        else if (key == 27) // ESC
        {
            selection = 0;
            selected = 1;
        }
#endif
        hide_cursor();
    }

    // Process selection
    if (selection == 0) // NO
    {
        return; // Return to previous menu
    }
    else // YES
    {
        CLEAR_SCREEN();

        // Draw goodbye frame with basic ASCII
        int goodbye_width = 50;
        int goodbye_x = center_x - (goodbye_width / 2);

        gotoxy(goodbye_x, center_y - 2);
        set_color(COLOR_YELLOW);
        printf("+");
        for (int i = 0; i < goodbye_width - 2; i++)
            printf("-");
        printf("+");

        gotoxy(goodbye_x, center_y - 1);
        printf("|");
        gotoxy(goodbye_x + goodbye_width - 1, center_y - 1);
        printf("|");

        gotoxy(center_x - 16, center_y);
        set_color(COLOR_YELLOW);
        printf("THANK YOU FOR USING OUR SYSTEM!");

        gotoxy(goodbye_x, center_y);
        set_color(COLOR_YELLOW);
        printf("|");
        gotoxy(goodbye_x + goodbye_width - 1, center_y);
        printf("|");

        gotoxy(goodbye_x, center_y + 1);
        printf("+");
        for (int i = 0; i < goodbye_width - 2; i++)
            printf("-");
        printf("+");

        gotoxy(center_x - 10, center_y + 3);
        set_color(COLOR_WHITE);
        printf("Program exiting...");

        SLEEP_MS(2000);
        exit(0);
    }
}

void operation_completed_prompt(int return_to_main)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    printf("\n\n");
    set_color(COLOR_CYAN);
    for (int i = 0; i < width; i++)
        printf("-");
    printf("\n");

    gotoxy(center_x - 21, height - 8);
    set_color(COLOR_YELLOW);
    printf("Operation completed successfully!");

    gotoxy(center_x - 20, height - 6);
    set_color(COLOR_WHITE);
    printf("What would you like to do next?");

    gotoxy(center_x - 15, height - 4);
    set_color(COLOR_GREEN);
    printf("[R] Return");

    if (return_to_main)
    {
        gotoxy(center_x - 15, height - 3);
        set_color(COLOR_GRAY);
        printf("[M] Main Menu");
    }

    gotoxy(center_x - 15, height - 2);
    set_color(COLOR_BLUE);
    printf("[E] Exit Program");

#ifdef _WIN32
    int key;
    do
    {
        key = _getch();
        key = toupper(key);

        if (key == 'R')
        {
            return;
        }
        else if (key == 'M' && return_to_main)
        {
            return;
        }
        else if (key == 'E')
        {
            display_exit_confirmation();
        }
    } while (!((key == 'R') || (key == 'M' && return_to_main) || (key == 'E')));
#else
    system("stty raw");
    int key = getchar();
    system("stty cooked");

    key = toupper(key);
    if (key == 'R')
    {
        return;
    }
    else if (key == 'M' && return_to_main)
    {
        return;
    }
    else if (key == 'E')
    {
        display_exit_confirmation();
    }
#endif
}

void press_any_key_to_continue()
{
    int width = get_terminal_width();
    int height = get_terminal_height();

    gotoxy(width / 2 - 15, height - 1);
    set_color(COLOR_YELLOW);
    printf("Press any key to continue...");

#ifdef _WIN32
    _getch();
#else
    system("stty raw");
    getchar();
    system("stty cooked");
#endif
}

char *select_file_menu(int start_y, int center_x)
{
    static char selected_file[50];
    const char *file_options[] = {
        "STUDENTS_ESI.bin (Main Database)",
        "STUDENTS_CP.bin (Compressed/LOF File)"};

    int choice = select_from_list("SELECT FILE TO USE", file_options, 2, start_y, center_x);

    if (choice == -1)
    {
        return NULL; // User cancelled
    }

    if (choice == 0)
    {
        strcpy(selected_file, "STUDENTS_ESI.bin");
    }
    else
    {
        strcpy(selected_file, "STUDENTS_CP.bin");
    }

    return selected_file;
}

// ========== ENHANCED UI FUNCTIONS FOR LIBRARY OPERATIONS ==========

/**
 * Enhanced UI for displaying students by blood type with proper navigation
 */
void display_blood_type_ui(Pr_index index_table[], int size, int value, char *fname, int *C41)
{
    if (C41)
        *C41 = 0;

    int start = find_value_Upper_bound_index(index_table, size, value);
    if (start < 0)
    {
        int width = get_terminal_width();
        int height = get_terminal_height();
        int center_x = width / 2;

        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_RED);
        printf("No students found with this blood type!");

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Count students with this blood type
    int totalStudents = 0;
    for (int i = start; i < size && index_table[i].Identifier == value; i++)
    {
        totalStudents++;
    }

    if (totalStudents == 0)
    {
        int width = get_terminal_width();
        int height = get_terminal_height();
        int center_x = width / 2;

        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_RED);
        printf("No students found with this blood type!");

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // ========== MAIN DISPLAY LOOP ==========
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 10;
    int totalPages = (totalStudents + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    // Get blood type name for display
    char blood_type_name[20];
    strcpy(blood_type_name, "Unknown");
    if (value >= 1 && value <= 8)
    {
        strcpy(blood_type_name, Blood_Types[value - 1].name);
    }

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("STUDENTS BY BLOOD TYPE");

        gotoxy(center_x - 10, 3);
        set_color(COLOR_YELLOW);
        printf("Blood Type: %s | Total Students: %d", blood_type_name, totalStudents);

        // ========== OPEN FILE ==========
        t_LnOVS *F;
        Open(&F, fname, 'E');

        // Calculate current page indices
        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > totalStudents)
            endIdx = totalStudents;

        int y = 6;

        // ========== RECORDS TABLE ==========
        if (totalStudents > 0)
        {
            // Page info
            gotoxy(center_x - 20, y);
            set_color(COLOR_GREEN);
            printf("Page %d/%d - Students %d to %d of %d",
                   currentPage, totalPages, startIdx + 1, endIdx, totalStudents);
            y += 2;

            // Table header
            int tableWidth = 90;
            int tableStartX = center_x - (tableWidth / 2);

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_YELLOW);
            printf("| #  | Student ID |        Name         | Birth Date |   Wilaya   | Blood Type |\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            // Display records for current page
            int displayed = 0;
            rec selectedStudent;
            bool foundSelected = false;
            block buffer;

            // Find and display students for current page
            int studentCount = 0;
            for (int i = start; i < size && displayed < recordsPerPage; i++)
            {
                if (index_table[i].Identifier == value)
                {
                    if (studentCount >= startIdx && studentCount < endIdx)
                    {
                        // Get record position
                        Pr_cor coords = index_table[i].crdt;

                        // Read the block and get the record
                        ReadBlock(F, coords.block_number, &buffer);
                        if (C41 && studentCount == startIdx) // Count cost only once per page
                            (*C41)++;

                        rec student = buffer.tab[coords.offset];

                        gotoxy(tableStartX, y);

                        // Highlight cursor line
                        if (cursorIndex == displayed)
                        {
                            set_color(COLOR_GREEN);
                            printf("|>%-3d", studentCount + 1);
                            set_color(COLOR_WHITE);
                            // Store the selected student for details display
                            selectedStudent = student;
                            foundSelected = true;
                        }
                        else
                        {
                            printf("| %-3d", studentCount + 1);
                        }

                        // Student ID
                        printf("| %-10d ", student.Student_ID);

                        // Name (truncated)
                        char name[21];
                        snprintf(name, sizeof(name), "%s %c",
                                 student.Family_Name, student.First_Name[0]);
                        if (strlen(name) > 20)
                        {
                            strncpy(name, name, 17);
                            strcpy(name + 17, "...");
                        }
                        printf("| %-19s ", name);

                        // Birth date
                        char date[11];
                        snprintf(date, sizeof(date), "%02d/%02d/%04d",
                                 student.Date_Birth.day,
                                 student.Date_Birth.month,
                                 student.Date_Birth.year);
                        printf("| %-10s ", date);

                        // Wilaya (truncated)
                        char wil[11];
                        strncpy(wil, student.Wilaya_Birth, 10);
                        wil[10] = '\0';
                        if (strlen(student.Wilaya_Birth) > 10)
                        {
                            strcpy(wil + 7, "...");
                        }
                        printf("| %-10s ", wil);

                        // Blood type
                        printf("| %-10s |\n", student.Blood_Type);

                        y++;
                        displayed++;
                    }
                    studentCount++;
                }
            }

            // Table footer
            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y += 2;

            // ========== CURRENT RECORD DETAILS (VERTICAL FORMAT) ==========
            if (foundSelected)
            {
                gotoxy(center_x - 20, y);
                set_color(COLOR_MAGENTA);
                printf("SELECTED STUDENT DETAILS:");
                y++;

                // Create a vertical details table with consistent colors
                int detailsWidth = 50;
                int detailsStartX = center_x - (detailsWidth / 2);

                // Top border - Cyan color
                gotoxy(detailsStartX, y);
                set_color(COLOR_CYAN);
                printf("+--------------------------------------------------+\n");
                y++;

                // Title - Yellow color
                gotoxy(detailsStartX, y);
                printf("|");
                gotoxy(center_x - 10, y);
                set_color(COLOR_YELLOW);
                printf("STUDENT RECORD");
                gotoxy(detailsStartX + detailsWidth - 1, y);
                set_color(COLOR_CYAN);
                printf("|\n");
                y++;

                // Separator - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y++;

                // Student ID - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Student ID: ");
                set_color(COLOR_WHITE);
                printf("%-35d", selectedStudent.Student_ID);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Family Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Family Name: ");
                set_color(COLOR_WHITE);
                printf("%-33s", selectedStudent.Family_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // First Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("First Name: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.First_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Birth Date - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Birth Date: ");
                char date_str[20];
                snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
                         selectedStudent.Date_Birth.day,
                         selectedStudent.Date_Birth.month,
                         selectedStudent.Date_Birth.year);
                set_color(COLOR_WHITE);
                printf("%-34s", date_str);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Wilaya - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Wilaya: ");
                set_color(COLOR_WHITE);
                printf("%-38s", selectedStudent.Wilaya_Birth);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Gender - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Gender: ");
                set_color(COLOR_WHITE);
                printf("%-37s",
                       selectedStudent.Gender == 1 ? "Male" : "Female");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Blood Type - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Blood Type: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Blood_Type);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Year of Study - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Year of Study: ");
                set_color(COLOR_WHITE);
                printf("%-31s", selectedStudent.Year_Study);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Speciality - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Speciality: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Speciality);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Campus Resident - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Campus Resident: ");
                set_color(COLOR_WHITE);
                printf("%-29s", selectedStudent.Resident_UC ? "Yes" : "No");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Bottom border - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y += 2;
            }
        }
        else
        {
            gotoxy(center_x - 10, y);
            set_color(COLOR_YELLOW);
            printf("No records found.");
            y += 2;
        }

        // Close file
        Close(F);

        // ========== NAVIGATION INSTRUCTIONS ==========
        y += 2;
        gotoxy(center_x - 30, y);
        set_color(COLOR_CYAN);
        printf("Navigation: ");

        gotoxy(center_x - 30, y + 1);
        set_color(COLOR_WHITE);
        printf("UP/DOWN: Select record | LEFT/RIGHT: Change page");

        gotoxy(center_x - 30, y + 2);
        printf("N: Next Page | P: Previous Page | G: Go to Page");

        gotoxy(center_x - 30, y + 3);
        printf("R: Return to Menu | ESC: Exit");

        y += 5;

        // ========== INPUT HANDLING ==========
        show_cursor();
        int keyPressed = 0;

#ifdef _WIN32
        int key = _getch();

        // Handle arrow keys for navigation
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72: // Up arrow - move cursor up
                if (totalStudents > 0 && cursorIndex > 0)
                {
                    cursorIndex--;
                    keyPressed = 1;
                }
                break;

            case 80: // Down arrow - move cursor down
                if (totalStudents > 0 && cursorIndex < (endIdx - startIdx - 1))
                {
                    cursorIndex++;
                    keyPressed = 1;
                }
                break;

            case 75: // Left arrow - previous page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;

            case 77: // Right arrow - next page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;
            }
        }
        else
        {
            // Handle letter keys for commands
            key = toupper(key);

            switch (key)
            {
            case 'N': // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on last page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'P': // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on first page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'G': // Go to Page
            {
                int newPage;
                gotoxy(center_x - 15, y);
                set_color(COLOR_YELLOW);
                printf("Enter page number (1-%d): ", totalPages);
                show_cursor();
                scanf("%d", &newPage);
                getchar();

                if (newPage > 0 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show error message
                    gotoxy(center_x - 20, y + 1);
                    set_color(COLOR_RED);
                    printf("Error: Page %d is out of range!", newPage);
                    SLEEP_MS(1500);
                }
                break;
            }

            case 'R': // Return to Menu
                continueDisplay = 0;
                keyPressed = 1;
                break;

            case 27: // ESC - Exit
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux implementation would go here
        // Similar logic but with Linux terminal handling
#endif

        if (!keyPressed)
        {
            SLEEP_MS(100);
        }

        hide_cursor();
    }

    hide_cursor();
}

/**
 * Enhanced UI for displaying students by speciality with proper navigation
 */
void display_speciality_ui(Pr_index index_table[], int size, char *fname, int *C42)
{
    if (C42)
        *C42 = 0;

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Define the 4 main specialities
    const char *speciality_names[] = {
        "Information Systems and Technologies (SIT)",
        "Computer Systems (SIQ)",
        "Software and Computer Systems (SIL)",
        "Intelligent Systems and Data (SID)"};

    const int num_specialities = 4;

    // Let user select speciality
    CLEAR_SCREEN();
    gotoxy(center_x - 15, 3);
    set_color(COLOR_CYAN);
    printf("SELECT SPECIALITY");

    int spec_choice = select_from_list("SELECT SPECIALITY", speciality_names, num_specialities, 8, center_x);
    if (spec_choice == -1)
    {
        return; // User cancelled
    }

    // Speciality values in index table: SIT=1, SIQ=2, SIL=3, SID=4
    int speciality_value = spec_choice + 1; // 1-based indexing
    char speciality_name[60];
    strcpy(speciality_name, speciality_names[spec_choice]);

    // Find students with this speciality
    int start = find_value_Upper_bound_index(index_table, size, speciality_value);
    if (start < 0)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_YELLOW);
        printf("No students found in %s!", speciality_name);

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_WHITE);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Count students in this speciality
    int totalStudents = 0;
    for (int i = start; i < size && index_table[i].Identifier == speciality_value; i++)
    {
        totalStudents++;
    }

    if (totalStudents == 0)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_YELLOW);
        printf("No students found in %s!", speciality_name);

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_WHITE);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // ========== MAIN DISPLAY LOOP ==========
    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 10;
    int totalPages = (totalStudents + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("STUDENTS BY SPECIALITY");

        gotoxy(center_x - 10, 3);
        set_color(COLOR_YELLOW);
        printf("Speciality: %s | Total Students: %d", speciality_name, totalStudents);

        // ========== OPEN FILE ==========
        t_LnOVS *F;
        Open(&F, fname, 'E');

        // Calculate current page indices
        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > totalStudents)
            endIdx = totalStudents;

        int y = 6;

        // ========== RECORDS TABLE ==========
        if (totalStudents > 0)
        {
            // Page info
            gotoxy(center_x - 20, y);
            set_color(COLOR_GREEN);
            printf("Page %d/%d - Students %d to %d of %d",
                   currentPage, totalPages, startIdx + 1, endIdx, totalStudents);
            y += 2;

            // Table header
            int tableWidth = 90;
            int tableStartX = center_x - (tableWidth / 2);

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_YELLOW);
            printf("| #  | Student ID |        Name         | Birth Date |   Wilaya   | Blood Type |\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            // Display records for current page
            int displayed = 0;
            rec selectedStudent;
            bool foundSelected = false;
            block buffer;

            // Find and display students for current page
            int studentCount = 0;
            for (int i = start; i < size && displayed < recordsPerPage; i++)
            {
                if (index_table[i].Identifier == speciality_value)
                {
                    if (studentCount >= startIdx && studentCount < endIdx)
                    {
                        // Get record position
                        Pr_cor coords = index_table[i].crdt;

                        // Read the block and get the record
                        ReadBlock(F, coords.block_number, &buffer);
                        if (C42 && studentCount == startIdx) // Count cost only once per page
                            (*C42)++;

                        rec student = buffer.tab[coords.offset];

                        gotoxy(tableStartX, y);

                        // Highlight cursor line
                        if (cursorIndex == displayed)
                        {
                            set_color(COLOR_GREEN);
                            printf("|>%-3d", studentCount + 1);
                            set_color(COLOR_WHITE);
                            // Store the selected student for details display
                            selectedStudent = student;
                            foundSelected = true;
                        }
                        else
                        {
                            printf("| %-3d", studentCount + 1);
                        }

                        // Student ID
                        printf("| %-10d ", student.Student_ID);

                        // Name (truncated)
                        char name[21];
                        snprintf(name, sizeof(name), "%s %c",
                                 student.Family_Name, student.First_Name[0]);
                        if (strlen(name) > 20)
                        {
                            strncpy(name, name, 17);
                            strcpy(name + 17, "...");
                        }
                        printf("| %-19s ", name);

                        // Birth date
                        char date[11];
                        snprintf(date, sizeof(date), "%02d/%02d/%04d",
                                 student.Date_Birth.day,
                                 student.Date_Birth.month,
                                 student.Date_Birth.year);
                        printf("| %-10s ", date);

                        // Wilaya (truncated)
                        char wil[11];
                        strncpy(wil, student.Wilaya_Birth, 10);
                        wil[10] = '\0';
                        if (strlen(student.Wilaya_Birth) > 10)
                        {
                            strcpy(wil + 7, "...");
                        }
                        printf("| %-10s ", wil);

                        // Blood type
                        printf("| %-10s |\n", student.Blood_Type);

                        y++;
                        displayed++;
                    }
                    studentCount++;
                }
            }

            // Table footer
            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y += 2;

            // ========== CURRENT RECORD DETAILS (VERTICAL FORMAT) ==========
            if (foundSelected)
            {
                gotoxy(center_x - 20, y);
                set_color(COLOR_MAGENTA);
                printf("SELECTED STUDENT DETAILS:");
                y++;

                // Create a vertical details table with consistent colors
                int detailsWidth = 50;
                int detailsStartX = center_x - (detailsWidth / 2);

                // Top border - Cyan color
                gotoxy(detailsStartX, y);
                set_color(COLOR_CYAN);
                printf("+--------------------------------------------------+\n");
                y++;

                // Title - Yellow color
                gotoxy(detailsStartX, y);
                printf("|");
                gotoxy(center_x - 10, y);
                set_color(COLOR_YELLOW);
                printf("STUDENT RECORD");
                gotoxy(detailsStartX + detailsWidth - 1, y);
                set_color(COLOR_CYAN);
                printf("|\n");
                y++;

                // Separator - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y++;

                // Student ID - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Student ID: ");
                set_color(COLOR_WHITE);
                printf("%-35d", selectedStudent.Student_ID);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Family Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Family Name: ");
                set_color(COLOR_WHITE);
                printf("%-33s", selectedStudent.Family_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // First Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("First Name: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.First_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Birth Date - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Birth Date: ");
                char date_str[20];
                snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
                         selectedStudent.Date_Birth.day,
                         selectedStudent.Date_Birth.month,
                         selectedStudent.Date_Birth.year);
                set_color(COLOR_WHITE);
                printf("%-34s", date_str);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Wilaya - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Wilaya: ");
                set_color(COLOR_WHITE);
                printf("%-38s", selectedStudent.Wilaya_Birth);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Gender - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Gender: ");
                set_color(COLOR_WHITE);
                printf("%-37s",
                       selectedStudent.Gender == 1 ? "Male" : "Female");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Blood Type - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Blood Type: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Blood_Type);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Year of Study - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Year of Study: ");
                set_color(COLOR_WHITE);
                printf("%-31s", selectedStudent.Year_Study);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Speciality - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Speciality: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Speciality);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Campus Resident - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Campus Resident: ");
                set_color(COLOR_WHITE);
                printf("%-29s", selectedStudent.Resident_UC ? "Yes" : "No");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Bottom border - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y += 2;
            }
        }
        else
        {
            gotoxy(center_x - 10, y);
            set_color(COLOR_YELLOW);
            printf("No records found.");
            y += 2;
        }

        // Close file
        Close(F);

        // ========== NAVIGATION INSTRUCTIONS ==========
        y += 2;
        gotoxy(center_x - 30, y);
        set_color(COLOR_CYAN);
        printf("Navigation: ");

        gotoxy(center_x - 30, y + 1);
        set_color(COLOR_WHITE);
        printf("UP/DOWN: Select record | LEFT/RIGHT: Change page");

        gotoxy(center_x - 30, y + 2);
        printf("N: Next Page | P: Previous Page | G: Go to Page");

        gotoxy(center_x - 30, y + 3);
        printf("R: Return to Menu | ESC: Exit");

        y += 5;

        // ========== INPUT HANDLING ==========
        show_cursor();
        int keyPressed = 0;

#ifdef _WIN32
        int key = _getch();

        // Handle arrow keys for navigation
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72: // Up arrow - move cursor up
                if (totalStudents > 0 && cursorIndex > 0)
                {
                    cursorIndex--;
                    keyPressed = 1;
                }
                break;

            case 80: // Down arrow - move cursor down
                if (totalStudents > 0 && cursorIndex < (endIdx - startIdx - 1))
                {
                    cursorIndex++;
                    keyPressed = 1;
                }
                break;

            case 75: // Left arrow - previous page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;

            case 77: // Right arrow - next page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;
            }
        }
        else
        {
            // Handle letter keys for commands
            key = toupper(key);

            switch (key)
            {
            case 'N': // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on last page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'P': // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on first page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'G': // Go to Page
            {
                int newPage;
                gotoxy(center_x - 15, y);
                set_color(COLOR_YELLOW);
                printf("Enter page number (1-%d): ", totalPages);
                show_cursor();
                scanf("%d", &newPage);
                getchar();

                if (newPage > 0 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show error message
                    gotoxy(center_x - 20, y + 1);
                    set_color(COLOR_RED);
                    printf("Error: Page %d is out of range!", newPage);
                    SLEEP_MS(1500);
                }
                break;
            }

            case 'R': // Return to Menu
                continueDisplay = 0;
                keyPressed = 1;
                break;

            case 27: // ESC - Exit
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux implementation would go here
        // Similar logic but with Linux terminal handling
#endif

        if (!keyPressed)
        {
            SLEEP_MS(100);
        }

        hide_cursor();
    }

    hide_cursor();
}

/**
 * Enhanced UI for displaying students by year of study with proper navigation
 */
void display_year_study_ui(Pr_index index_table[], int size, int value, char *fname, int *C44)
{
    int start = find_value_Upper_bound_index(index_table, size, value);
    if (start < 0)
    {
        int width = get_terminal_width();
        int height = get_terminal_height();
        int center_x = width / 2;

        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_RED);
        printf("No students found in this year of study!");

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Count students
    int totalStudents = 0;
    for (int i = start; i < size && index_table[i].Identifier == value; i++)
    {
        totalStudents++;
    }

    if (totalStudents == 0)
    {
        int width = get_terminal_width();
        int height = get_terminal_height();
        int center_x = width / 2;

        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_RED);
        printf("No students found in this year of study!");

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Get year name
    char year_name[10] = "Unknown";
    if (value >= 1 && value <= 5)
    {
        strcpy(year_name, specs[value - 1].years);
    }

    // ========== MAIN DISPLAY LOOP ==========
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 10;
    int totalPages = (totalStudents + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("STUDENTS BY YEAR OF STUDY");

        gotoxy(center_x - 10, 3);
        set_color(COLOR_YELLOW);
        printf("Year: %s | Total Students: %d", year_name, totalStudents);

        // ========== OPEN FILE ==========
        t_LnOVS *F;
        Open(&F, fname, 'E');

        // Calculate current page indices
        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > totalStudents)
            endIdx = totalStudents;

        int y = 6;

        // ========== RECORDS TABLE ==========
        if (totalStudents > 0)
        {
            // Page info
            gotoxy(center_x - 20, y);
            set_color(COLOR_GREEN);
            printf("Page %d/%d - Students %d to %d of %d",
                   currentPage, totalPages, startIdx + 1, endIdx, totalStudents);
            y += 2;

            // Table header
            int tableWidth = 90;
            int tableStartX = center_x - (tableWidth / 2);

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_YELLOW);
            printf("| #  | Student ID |        Name         | Birth Date |   Wilaya   | Blood Type |\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            // Display records for current page
            int displayed = 0;
            rec selectedStudent;
            bool foundSelected = false;
            block buffer;

            // Find and display students for current page
            int studentCount = 0;
            for (int i = start; i < size && displayed < recordsPerPage; i++)
            {
                if (index_table[i].Identifier == value)
                {
                    if (studentCount >= startIdx && studentCount < endIdx)
                    {
                        // Get record position
                        Pr_cor coords = index_table[i].crdt;

                        // Read the block and get the record
                        ReadBlock(F, coords.block_number, &buffer);
                        if (C44 && studentCount == startIdx) // Count cost only once per page
                            (*C44)++;

                        rec student = buffer.tab[coords.offset];

                        gotoxy(tableStartX, y);

                        // Highlight cursor line
                        if (cursorIndex == displayed)
                        {
                            set_color(COLOR_GREEN);
                            printf("|>%-3d", studentCount + 1);
                            set_color(COLOR_WHITE);
                            // Store the selected student for details display
                            selectedStudent = student;
                            foundSelected = true;
                        }
                        else
                        {
                            printf("| %-3d", studentCount + 1);
                        }

                        // Student ID
                        printf("| %-10d ", student.Student_ID);

                        // Name (truncated)
                        char name[21];
                        snprintf(name, sizeof(name), "%s %c",
                                 student.Family_Name, student.First_Name[0]);
                        if (strlen(name) > 20)
                        {
                            strncpy(name, name, 17);
                            strcpy(name + 17, "...");
                        }
                        printf("| %-19s ", name);

                        // Birth date
                        char date[11];
                        snprintf(date, sizeof(date), "%02d/%02d/%04d",
                                 student.Date_Birth.day,
                                 student.Date_Birth.month,
                                 student.Date_Birth.year);
                        printf("| %-10s ", date);

                        // Wilaya (truncated)
                        char wil[11];
                        strncpy(wil, student.Wilaya_Birth, 10);
                        wil[10] = '\0';
                        if (strlen(student.Wilaya_Birth) > 10)
                        {
                            strcpy(wil + 7, "...");
                        }
                        printf("| %-10s ", wil);

                        // Blood type
                        printf("| %-10s |\n", student.Blood_Type);

                        y++;
                        displayed++;
                    }
                    studentCount++;
                }
            }

            // Table footer
            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y += 2;

            // ========== CURRENT RECORD DETAILS (VERTICAL FORMAT) ==========
            if (foundSelected)
            {
                gotoxy(center_x - 20, y);
                set_color(COLOR_MAGENTA);
                printf("SELECTED STUDENT DETAILS:");
                y++;

                // Create a vertical details table with consistent colors
                int detailsWidth = 50;
                int detailsStartX = center_x - (detailsWidth / 2);

                // Top border - Cyan color
                gotoxy(detailsStartX, y);
                set_color(COLOR_CYAN);
                printf("+--------------------------------------------------+\n");
                y++;

                // Title - Yellow color
                gotoxy(detailsStartX, y);
                printf("|");
                gotoxy(center_x - 10, y);
                set_color(COLOR_YELLOW);
                printf("STUDENT RECORD");
                gotoxy(detailsStartX + detailsWidth - 1, y);
                set_color(COLOR_CYAN);
                printf("|\n");
                y++;

                // Separator - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y++;

                // Student ID - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Student ID: ");
                set_color(COLOR_WHITE);
                printf("%-35d", selectedStudent.Student_ID);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Family Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Family Name: ");
                set_color(COLOR_WHITE);
                printf("%-33s", selectedStudent.Family_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // First Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("First Name: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.First_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Birth Date - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Birth Date: ");
                char date_str[20];
                snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
                         selectedStudent.Date_Birth.day,
                         selectedStudent.Date_Birth.month,
                         selectedStudent.Date_Birth.year);
                set_color(COLOR_WHITE);
                printf("%-34s", date_str);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Wilaya - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Wilaya: ");
                set_color(COLOR_WHITE);
                printf("%-38s", selectedStudent.Wilaya_Birth);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Gender - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Gender: ");
                set_color(COLOR_WHITE);
                printf("%-37s",
                       selectedStudent.Gender == 1 ? "Male" : "Female");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Blood Type - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Blood Type: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Blood_Type);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Year of Study - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Year of Study: ");
                set_color(COLOR_WHITE);
                printf("%-31s", selectedStudent.Year_Study);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Speciality - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Speciality: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Speciality);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Campus Resident - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Campus Resident: ");
                set_color(COLOR_WHITE);
                printf("%-29s", selectedStudent.Resident_UC ? "Yes" : "No");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Bottom border - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y += 2;
            }
        }
        else
        {
            gotoxy(center_x - 10, y);
            set_color(COLOR_YELLOW);
            printf("No records found.");
            y += 2;
        }

        // Close file
        Close(F);

        // ========== NAVIGATION INSTRUCTIONS ==========
        y += 2;
        gotoxy(center_x - 30, y);
        set_color(COLOR_CYAN);
        printf("Navigation: ");

        gotoxy(center_x - 30, y + 1);
        set_color(COLOR_WHITE);
        printf("UP/DOWN: Select record | LEFT/RIGHT: Change page");

        gotoxy(center_x - 30, y + 2);
        printf("N: Next Page | P: Previous Page | G: Go to Page");

        gotoxy(center_x - 30, y + 3);
        printf("R: Return to Menu | ESC: Exit");

        y += 5;

        // ========== INPUT HANDLING ==========
        show_cursor();
        int keyPressed = 0;

#ifdef _WIN32
        int key = _getch();

        // Handle arrow keys for navigation
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72: // Up arrow - move cursor up
                if (totalStudents > 0 && cursorIndex > 0)
                {
                    cursorIndex--;
                    keyPressed = 1;
                }
                break;

            case 80: // Down arrow - move cursor down
                if (totalStudents > 0 && cursorIndex < (endIdx - startIdx - 1))
                {
                    cursorIndex++;
                    keyPressed = 1;
                }
                break;

            case 75: // Left arrow - previous page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;

            case 77: // Right arrow - next page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;
            }
        }
        else
        {
            // Handle letter keys for commands
            key = toupper(key);

            switch (key)
            {
            case 'N': // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on last page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'P': // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on first page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'G': // Go to Page
            {
                int newPage;
                gotoxy(center_x - 15, y);
                set_color(COLOR_YELLOW);
                printf("Enter page number (1-%d): ", totalPages);
                show_cursor();
                scanf("%d", &newPage);
                getchar();

                if (newPage > 0 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show error message
                    gotoxy(center_x - 20, y + 1);
                    set_color(COLOR_RED);
                    printf("Error: Page %d is out of range!", newPage);
                    SLEEP_MS(1500);
                }
                break;
            }

            case 'R': // Return to Menu
                continueDisplay = 0;
                keyPressed = 1;
                break;

            case 27: // ESC - Exit
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux implementation would go here
        // Similar logic but with Linux terminal handling
#endif

        if (!keyPressed)
        {
            SLEEP_MS(100);
        }

        hide_cursor();
    }

    hide_cursor();
}

/**
 * Enhanced UI for displaying students by birth interval with proper navigation
 */
void display_birth_interval_ui(Pr_index index_table[], int size, int Y1, int Y2, char *fname, int *C43)
{
    // Validate years
    if (Y1 < 2003 || Y1 > 2008 || Y2 < 2003 || Y2 > 2008 || Y1 > Y2)
    {
        int width = get_terminal_width();
        int height = get_terminal_height();
        int center_x = width / 2;

        CLEAR_SCREEN();
        gotoxy(center_x - 25, height / 2 - 2);
        set_color(COLOR_RED);
        printf("Invalid year range! Must be 2003-2008 and Y1 <= Y2");

        gotoxy(center_x - 15, height / 2 + 1);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Current year for age calculation
    int current_year = 2025;
    int max_birth_year = current_year - 20; // Students must be born in 2005 or later to be 20 or under

    // Count students in interval who are 20 or under
    int totalStudents = 0;
    for (int i = 0; i < size; i++)
    {
        if (index_table[i].Identifier >= Y1 &&
            index_table[i].Identifier <= Y2 &&
            index_table[i].Identifier >= max_birth_year)
        {
            totalStudents++;
        }
    }

    if (totalStudents == 0)
    {
        int width = get_terminal_width();
        int height = get_terminal_height();
        int center_x = width / 2;

        CLEAR_SCREEN();
        gotoxy(center_x - 20, height / 2 - 2);
        set_color(COLOR_YELLOW);
        printf("No students under 20 found in birth year interval!");

        gotoxy(center_x - 15, height / 2 + 1);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // ========== MAIN DISPLAY LOOP ==========
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 10;
    int totalPages = (totalStudents + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    // Create title for display
    char title[100];
    snprintf(title, sizeof(title), "STUDENTS BORN %d-%d (20 OR UNDER)", Y1, Y2);

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - strlen(title) / 2, 2);
        set_color(COLOR_MAGENTA);
        printf("%s", title);

        gotoxy(center_x - 10, 3);
        set_color(COLOR_YELLOW);
        printf("Total Students: %d (Born 2005 or later)", totalStudents);

        // ========== OPEN FILE ==========
        t_LnOVS *F;
        Open(&F, fname, 'E');

        // Calculate current page indices
        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > totalStudents)
            endIdx = totalStudents;

        int y = 6;

        // ========== RECORDS TABLE ==========
        if (totalStudents > 0)
        {
            // Page info
            gotoxy(center_x - 20, y);
            set_color(COLOR_GREEN);
            printf("Page %d/%d - Students %d to %d of %d",
                   currentPage, totalPages, startIdx + 1, endIdx, totalStudents);
            y += 2;

            // Table header
            int tableWidth = 90;
            int tableStartX = center_x - (tableWidth / 2);

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_YELLOW);
            printf("| #  | Student ID |        Name         | Birth Date |   Wilaya   | Blood Type |\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            // Display records for current page
            int displayed = 0;
            rec selectedStudent;
            bool foundSelected = false;
            block buffer;

            // Find and display students for current page
            int studentCount = 0;
            for (int i = 0; i < size && displayed < recordsPerPage; i++)
            {
                if (index_table[i].Identifier >= Y1 &&
                    index_table[i].Identifier <= Y2 &&
                    index_table[i].Identifier >= max_birth_year)
                {
                    if (studentCount >= startIdx && studentCount < endIdx)
                    {
                        // Get record position
                        Pr_cor coords = index_table[i].crdt;

                        // Read the block and get the record
                        ReadBlock(F, coords.block_number, &buffer);
                        if (C43 && studentCount == startIdx) // Count cost only once per page
                            (*C43)++;

                        rec student = buffer.tab[coords.offset];

                        gotoxy(tableStartX, y);

                        // Highlight cursor line
                        if (cursorIndex == displayed)
                        {
                            set_color(COLOR_GREEN);
                            printf("|>%-3d", studentCount + 1);
                            set_color(COLOR_WHITE);
                            // Store the selected student for details display
                            selectedStudent = student;
                            foundSelected = true;
                        }
                        else
                        {
                            printf("| %-3d", studentCount + 1);
                        }

                        // Student ID
                        printf("| %-10d ", student.Student_ID);

                        // Name (truncated)
                        char name[21];
                        snprintf(name, sizeof(name), "%s %c",
                                 student.Family_Name, student.First_Name[0]);
                        if (strlen(name) > 20)
                        {
                            strncpy(name, name, 17);
                            strcpy(name + 17, "...");
                        }
                        printf("| %-19s ", name);

                        // Birth date
                        char date[11];
                        snprintf(date, sizeof(date), "%02d/%02d/%04d",
                                 student.Date_Birth.day,
                                 student.Date_Birth.month,
                                 student.Date_Birth.year);
                        printf("| %-10s ", date);

                        // Wilaya (truncated)
                        char wil[11];
                        strncpy(wil, student.Wilaya_Birth, 10);
                        wil[10] = '\0';
                        if (strlen(student.Wilaya_Birth) > 10)
                        {
                            strcpy(wil + 7, "...");
                        }
                        printf("| %-10s ", wil);

                        // Blood type
                        printf("| %-10s |\n", student.Blood_Type);

                        y++;
                        displayed++;
                    }
                    studentCount++;
                }
            }

            // Table footer
            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y += 2;

            // ========== CURRENT RECORD DETAILS (VERTICAL FORMAT) ==========
            if (foundSelected)
            {
                gotoxy(center_x - 20, y);
                set_color(COLOR_MAGENTA);
                printf("SELECTED STUDENT DETAILS:");
                y++;

                // Create a vertical details table with consistent colors
                int detailsWidth = 50;
                int detailsStartX = center_x - (detailsWidth / 2);

                // Top border - Cyan color
                gotoxy(detailsStartX, y);
                set_color(COLOR_CYAN);
                printf("+--------------------------------------------------+\n");
                y++;

                // Title - Yellow color
                gotoxy(detailsStartX, y);
                printf("|");
                gotoxy(center_x - 10, y);
                set_color(COLOR_YELLOW);
                printf("STUDENT RECORD");
                gotoxy(detailsStartX + detailsWidth - 1, y);
                set_color(COLOR_CYAN);
                printf("|\n");
                y++;

                // Separator - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y++;

                // Student ID - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Student ID: ");
                set_color(COLOR_WHITE);
                printf("%-35d", selectedStudent.Student_ID);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Family Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Family Name: ");
                set_color(COLOR_WHITE);
                printf("%-33s", selectedStudent.Family_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // First Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("First Name: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.First_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Birth Date - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Birth Date: ");
                char date_str[20];
                snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
                         selectedStudent.Date_Birth.day,
                         selectedStudent.Date_Birth.month,
                         selectedStudent.Date_Birth.year);
                set_color(COLOR_WHITE);
                printf("%-34s", date_str);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Wilaya - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Wilaya: ");
                set_color(COLOR_WHITE);
                printf("%-38s", selectedStudent.Wilaya_Birth);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Gender - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Gender: ");
                set_color(COLOR_WHITE);
                printf("%-37s",
                       selectedStudent.Gender == 1 ? "Male" : "Female");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Blood Type - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Blood Type: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Blood_Type);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Year of Study - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Year of Study: ");
                set_color(COLOR_WHITE);
                printf("%-31s", selectedStudent.Year_Study);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Speciality - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Speciality: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Speciality);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Campus Resident - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Campus Resident: ");
                set_color(COLOR_WHITE);
                printf("%-29s", selectedStudent.Resident_UC ? "Yes" : "No");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Age calculation - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Age: ");
                int age = current_year - selectedStudent.Date_Birth.year;
                set_color(COLOR_WHITE);
                printf("%-41d", age);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Bottom border - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y += 2;
            }
        }
        else
        {
            gotoxy(center_x - 10, y);
            set_color(COLOR_YELLOW);
            printf("No records found.");
            y += 2;
        }

        // Close file
        Close(F);

        // ========== NAVIGATION INSTRUCTIONS ==========
        y += 2;
        gotoxy(center_x - 30, y);
        set_color(COLOR_CYAN);
        printf("Navigation: ");

        gotoxy(center_x - 30, y + 1);
        set_color(COLOR_WHITE);
        printf("UP/DOWN: Select record | LEFT/RIGHT: Change page");

        gotoxy(center_x - 30, y + 2);
        printf("N: Next Page | P: Previous Page | G: Go to Page");

        gotoxy(center_x - 30, y + 3);
        printf("R: Return to Menu | ESC: Exit");

        y += 5;

        // ========== INPUT HANDLING ==========
        show_cursor();
        int keyPressed = 0;

#ifdef _WIN32
        int key = _getch();

        // Handle arrow keys for navigation
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72: // Up arrow - move cursor up
                if (totalStudents > 0 && cursorIndex > 0)
                {
                    cursorIndex--;
                    keyPressed = 1;
                }
                break;

            case 80: // Down arrow - move cursor down
                if (totalStudents > 0 && cursorIndex < (endIdx - startIdx - 1))
                {
                    cursorIndex++;
                    keyPressed = 1;
                }
                break;

            case 75: // Left arrow - previous page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;

            case 77: // Right arrow - next page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;
            }
        }
        else
        {
            // Handle letter keys for commands
            key = toupper(key);

            switch (key)
            {
            case 'N': // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on last page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'P': // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on first page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'G': // Go to Page
            {
                int newPage;
                gotoxy(center_x - 15, y);
                set_color(COLOR_YELLOW);
                printf("Enter page number (1-%d): ", totalPages);
                show_cursor();
                scanf("%d", &newPage);
                getchar();

                if (newPage > 0 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show error message
                    gotoxy(center_x - 20, y + 1);
                    set_color(COLOR_RED);
                    printf("Error: Page %d is out of range!", newPage);
                    SLEEP_MS(1500);
                }
                break;
            }

            case 'R': // Return to Menu
                continueDisplay = 0;
                keyPressed = 1;
                break;

            case 27: // ESC - Exit
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux implementation would go here
        // Similar logic but with Linux terminal handling
#endif

        if (!keyPressed)
        {
            SLEEP_MS(100);
        }

        hide_cursor();
    }

    hide_cursor();
}

/**
 * Generic function to display students list with paginated UI
 */

void display_students_list_ui(const char *title, const char *subtitle,
                              Pr_index index_table[], int size, int filter_value,
                              char *fname, int totalStudents, int start_index, int *cost)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 15;
    int totalPages = (totalStudents + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - strlen(title) / 2, 2);
        set_color(COLOR_MAGENTA);
        printf("%s", title);

        gotoxy(center_x - strlen(subtitle) / 2, 3);
        set_color(COLOR_YELLOW);
        printf("%s", subtitle);

        gotoxy(center_x - 30, 5);
        set_color(COLOR_GREEN);
        printf("+--------------------------------------------------------------+\n");

        gotoxy(center_x - 30, 6);
        printf("|  Total Students: %-6d | Page: %-3d/%-3d | Cost: %-4d       |\n",
               totalStudents, currentPage, totalPages, totalStudents);

        gotoxy(center_x - 30, 7);
        printf("+--------------------------------------------------------------+\n");

        // ========== TABLE SECTION ==========
        t_LnOVS *F;
        Open(&F, fname, 'E');
        block buffer;

        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > totalStudents)
            endIdx = totalStudents;

        int tableWidth = 90;
        int tableStartX = center_x - (tableWidth / 2);

        // Table header
        gotoxy(tableStartX, 9);
        set_color(COLOR_BLUE);
        printf("+----+------------+---------------------+------------+------------+------------+------------+------------+------------------+\n");

        gotoxy(tableStartX, 10);
        set_color(COLOR_YELLOW);
        printf("| #  | Student ID |        Name         |  Birth Date |   Wilaya   | Blood Type | Year Study |  Speciality  | Campus Resident |\n");

        gotoxy(tableStartX, 11);
        set_color(COLOR_BLUE);
        printf("+----+------------+---------------------+------------+------------+------------+------------+------------+------------------+\n");

        // Find and display records for current page
        int lineY = 12;
        int displayed = 0;
        int studentCount = 0;

        for (int i = start_index; i < size && displayed < recordsPerPage; i++)
        {
            if (index_table[i].Identifier == filter_value)
            {
                if (studentCount >= startIdx && studentCount < endIdx)
                {
                    Pr_cor coords = index_table[i].crdt;
                    ReadBlock(F, coords.block_number, &buffer);
                    if (cost)
                        (*cost)++;

                    rec student = buffer.tab[coords.offset];

                    gotoxy(tableStartX, lineY);

                    // Highlight cursor line
                    if (cursorIndex == displayed)
                    {
                        set_color(COLOR_GREEN);
                        printf("|>%-3d", studentCount + 1);
                        set_color(COLOR_WHITE);
                    }
                    else
                    {
                        printf("| %-3d", studentCount + 1);
                    }

                    // Student ID
                    printf("| %-10d ", student.Student_ID);

                    // Name (truncated)
                    char name[21];
                    snprintf(name, sizeof(name), "%s %c", student.Family_Name, student.First_Name[0]);
                    if (strlen(name) > 20)
                    {
                        strncpy(name, name, 17);
                        strcpy(name + 17, "...");
                    }
                    printf("| %-19s ", name);

                    // Birth date
                    printf("| %02d/%02d/%04d ",
                           student.Date_Birth.day,
                           student.Date_Birth.month,
                           student.Date_Birth.year);

                    // Wilaya (truncated)
                    char wil[12];
                    strncpy(wil, student.Wilaya_Birth, 11);
                    wil[11] = '\0';
                    if (strlen(student.Wilaya_Birth) > 11)
                    {
                        strcpy(wil + 8, "...");
                    }
                    printf("| %-10s ", wil);

                    // Blood type
                    char blood[12];
                    strncpy(blood, student.Blood_Type, 11);
                    blood[11] = '\0';
                    printf("| %-10s ", blood);

                    // Year study
                    printf("| %-8s ", student.Year_Study);

                    // Speciality (truncated)
                    char spec[12];
                    strncpy(spec, student.Speciality, 11);
                    spec[11] = '\0';
                    if (strlen(student.Speciality) > 11)
                    {
                        strcpy(spec + 8, "...");
                    }
                    printf("| %-10s ", spec);

                    // Resident
                    printf("|      %-3s      |\n", student.Resident_UC ? "Yes" : "No");

                    lineY++;
                    displayed++;
                }
                studentCount++;
            }
        }

        // Table footer
        gotoxy(tableStartX, lineY);
        set_color(COLOR_BLUE);
        printf("+----+------------+---------------------+------------+------------+------------+------------+------------+------------------+\n");

        Close(F);

        // ========== NAVIGATION SECTION ==========
        int navStartY = lineY + 4;
        if (navStartY > height - 12)
            navStartY = height - 12;

        // Clear navigation area
        for (int i = navStartY; i < height - 1; i++)
        {
            gotoxy(0, i);
            printf("%-*s", width, "");
        }

        // Navigation menu
        char *navOptions[] = {
            " Next Page        ",
            " Previous Page    ",
            " Go to Page       ",
            " View Details     ",
            " Export Results   ",
            " Return           "};

        int navCursor = 0;
        int navSelected = -1;

        while (navSelected == -1 && continueDisplay)
        {
            // Draw navigation box
            gotoxy(center_x - 25, navStartY);
            set_color(COLOR_MAGENTA);
            printf("+----------------------------------------------------+\n");

            gotoxy(center_x - 25, navStartY + 1);
            printf("|                 NAVIGATION MENU                   |\n");

            gotoxy(center_x - 25, navStartY + 2);
            printf("+----------------------------------------------------+\n");

            // Draw menu options
            for (int i = 0; i < 6; i++)
            {
                gotoxy(center_x - 25, navStartY + 3 + i);

                if (i == navCursor)
                {
                    set_color(COLOR_GREEN);
                    printf("| > %-46s |\n", navOptions[i]);
                }
                else
                {
                    set_color(COLOR_WHITE);
                    printf("|   %-46s |\n", navOptions[i]);
                }
            }

            gotoxy(center_x - 25, navStartY + 9);
            set_color(COLOR_MAGENTA);
            printf("+----------------------------------------------------+\n");

            // Instructions
            gotoxy(center_x - 25, navStartY + 11);
            set_color(COLOR_YELLOW);
            printf("ARROW KEYS: Navigate | ENTER: Select | ESC: Return");

            // Handle input
            show_cursor();
#ifdef _WIN32
            int key = _getch();
            if (key == 224 || key == 0)
            {
                key = _getch();
                if (key == 72 && navCursor > 0)
                {
                    navCursor--;
                }
                else if (key == 80 && navCursor < 5)
                {
                    navCursor++;
                }
            }
            else if (key == 13)
            {
                navSelected = navCursor;
            }
            else if (key == 27)
            {
                continueDisplay = 0;
                break;
            }
#else
            system("stty raw");
            int key = getchar();
            system("stty cooked");

            if (key == 27)
            {
                system("stty raw");
                getchar();
                key = getchar();
                system("stty cooked");

                if (key == 65 && navCursor > 0)
                {
                    navCursor--;
                }
                else if (key == 66 && navCursor < 5)
                {
                    navCursor++;
                }
            }
            else if (key == 10)
            {
                navSelected = navCursor;
            }
            else if (key == 27)
            {
                continueDisplay = 0;
                break;
            }
#endif
            hide_cursor();
        }

        // Process selection
        if (navSelected != -1)
        {
            switch (navSelected)
            {
            case 0: // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                }
                break;

            case 1: // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                }
                break;

            case 2: // Go to Page
            {
                gotoxy(center_x - 15, navStartY + 13);
                set_color(COLOR_YELLOW);
                printf("Enter page (1-%d): ", totalPages);
                show_cursor();
                int newPage;
                scanf("%d", &newPage);
                getchar();
                if (newPage >= 1 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                }
                // Clear input area
                gotoxy(center_x - 15, navStartY + 13);
                printf("                                             ");
                break;
            }

            case 3: // View Details
                if (cursorIndex >= 0 && cursorIndex < displayed)
                {
                    // Find the actual student index
                    int studentIdx = start_index;
                    int count = 0;
                    for (int i = start_index; i < size; i++)
                    {
                        if (index_table[i].Identifier == filter_value)
                        {
                            if (count == startIdx + cursorIndex)
                            {
                                studentIdx = i;
                                break;
                            }
                            count++;
                        }
                    }

                    // Display student details
                    t_LnOVS *F2;
                    Open(&F2, fname, 'E');
                    block buf2;
                    Pr_cor coords = index_table[studentIdx].crdt;
                    ReadBlock(F2, coords.block_number, &buf2);
                    rec student = buf2.tab[coords.offset];

                    CLEAR_SCREEN();
                    display_record_detail_ui(student, center_x);

                    Close(F2);
                }
                break;

            case 5: // Return
                continueDisplay = 0;
                break;
            }
        }
    }
}

/**
 * Display students list for interval (different filtering logic)
 */
void display_students_list_interval_ui(const char *title,
                                       Pr_index index_table[], int size, int Y1, int Y2,
                                       char *fname, int totalStudentsInRange, int *cost)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 15;

    // Changed to include students who are 20 OR UNDER
    // Current year is 2025, so students born in 2005 or later are 20 or under
    int current_year = 2025;
    int max_birth_year = current_year - 20; // Students must be born in 2005 or later to be 20 or under

    // Add cost tracking variables
    static int finalCostValue = 0;

    // Create a unique ID for this search
    int currentSearchId = Y1 * 1000 + Y2;

    // If this is a new search, reset the flags

    // COUNT ACTUAL STUDENTS 20 OR UNDER for THIS DISPLAY
    int actualStudents20OrUnder = 0;
    for (int i = 0; i < size; i++)
    {
        if (index_table[i].Identifier >= Y1 &&
            index_table[i].Identifier <= Y2 &&
            index_table[i].Identifier >= max_birth_year)
        {
            actualStudents20OrUnder++;
        }
    }

    // Use ONLY 20-or-under count for pagination
    int totalPages = (actualStudents20OrUnder + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - strlen(title) / 2, 2);
        set_color(COLOR_MAGENTA);
        printf("%s", title);

        gotoxy(center_x - 30, 4);
        set_color(COLOR_YELLOW);
        printf("+--------------------------------------------------------------+\n");

        gotoxy(center_x - 30, 5);
        printf("|  In Range: %-5d | 20 or Under: %-5d | Page: %-3d/%-3d |\n",
               totalStudentsInRange, actualStudents20OrUnder, currentPage, totalPages);

        gotoxy(center_x - 30, 6);
        printf("|  Cost: %-55d |\n", totalStudentsInRange);

        gotoxy(center_x - 30, 7);
        printf("+--------------------------------------------------------------+\n");

        // ========== TABLE SECTION ==========
        t_LnOVS *F;
        Open(&F, fname, 'E');
        block buffer;

        // Calculate start/end indices based on 20-or-under students
        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > actualStudents20OrUnder)
            endIdx = actualStudents20OrUnder;

        int tableWidth = 90;
        int tableStartX = center_x - (tableWidth / 2);

        // Table header
        gotoxy(tableStartX, 9);
        set_color(COLOR_BLUE);
        printf("+----+------------+---------------------+------------+------------+------------+------------+------------+------------------+\n");

        gotoxy(tableStartX, 10);
        set_color(COLOR_YELLOW);
        printf("| #  | Student ID |        Name         |  Birth Date |   Wilaya   | Blood Type | Year Study |  Speciality  | Campus Resident |\n");

        gotoxy(tableStartX, 11);
        set_color(COLOR_BLUE);
        printf("+----+------------+---------------------+------------+------------+------------+------------+------------+------------------+\n");

        // Find and display records for current page
        int lineY = 12;
        int displayed = 0;
        int studentCount = 0; // This counts ONLY students 20 or under

        for (int i = 0; i < size && displayed < recordsPerPage; i++)
        {
            // Check if student is in year range AND 20 years old or younger
            if (index_table[i].Identifier >= Y1 &&
                index_table[i].Identifier <= Y2 &&
                index_table[i].Identifier >= max_birth_year)
            {
                // studentCount is now the count of ONLY 20-or-under students
                if (studentCount >= startIdx && studentCount < endIdx)
                {
                    Pr_cor coords = index_table[i].crdt;
                    ReadBlock(F, coords.block_number, &buffer);

                    // Count cost only on first page
                    if (cost && currentPage == 1)
                    {
                        (*cost)++;
                        finalCostValue = *cost; // Store the value
                    }

                    rec student = buffer.tab[coords.offset];

                    gotoxy(tableStartX, lineY);

                    // Highlight cursor line
                    if (cursorIndex == displayed)
                    {
                        set_color(COLOR_GREEN);
                        printf("|>%-3d", studentCount + 1); // Display position among 20-or-under students
                        set_color(COLOR_WHITE);
                    }
                    else
                    {
                        printf("| %-3d", studentCount + 1); // Display position among 20-or-under students
                    }

                    // Student ID
                    printf("| %-10d ", student.Student_ID);

                    // Name (truncated)
                    char name[21];
                    snprintf(name, sizeof(name), "%s %c", student.Family_Name, student.First_Name[0]);
                    if (strlen(name) > 20)
                    {
                        strncpy(name, name, 17);
                        strcpy(name + 17, "...");
                    }
                    printf("| %-19s ", name);

                    // Birth date
                    printf("| %02d/%02d/%04d ",
                           student.Date_Birth.day,
                           student.Date_Birth.month,
                           student.Date_Birth.year);

                    // Wilaya (truncated)
                    char wil[12];
                    strncpy(wil, student.Wilaya_Birth, 11);
                    wil[11] = '\0';
                    if (strlen(student.Wilaya_Birth) > 11)
                    {
                        strcpy(wil + 8, "...");
                    }
                    printf("| %-10s ", wil);

                    // Blood type
                    char blood[12];
                    strncpy(blood, student.Blood_Type, 11);
                    blood[11] = '\0';
                    printf("| %-10s ", blood);

                    // Year study
                    printf("| %-8s ", student.Year_Study);

                    // Speciality (truncated)
                    char spec[12];
                    strncpy(spec, student.Speciality, 11);
                    spec[11] = '\0';
                    if (strlen(student.Speciality) > 11)
                    {
                        strcpy(spec + 8, "...");
                    }
                    printf("| %-10s ", spec);

                    // Resident
                    printf("|      %-3s      |\n", student.Resident_UC ? "Yes" : "No");

                    lineY++;
                    displayed++;
                }
                studentCount++; // Increment ONLY for 20-or-under students
            }
        }

        // If no students found on this page but page number is valid, show empty message
        if (displayed == 0 && currentPage <= totalPages)
        {
            gotoxy(tableStartX + 10, lineY);
            set_color(COLOR_YELLOW);
            printf("No students found on this page.");
            lineY++;
        }

        // Table footer
        gotoxy(tableStartX, lineY);
        set_color(COLOR_BLUE);
        printf("+----+------------+---------------------+------------+------------+------------+------------+------------+------------------+\n");

        Close(F);

        // ========== NAVIGATION SECTION ==========
        int navStartY = lineY + 4;
        if (navStartY > height - 12)
            navStartY = height - 12;

        // Clear navigation area
        for (int i = navStartY; i < height - 1; i++)
        {
            gotoxy(0, i);
            printf("%-*s", width, "");
        }

        // Navigation menu
        char *navOptions[] = {
            " Next Page        ",
            " Previous Page    ",
            " Go to Page       ",
            " View Details     ",
            " Export Results   ",
            " Return           "};

        int navCursor = 0;
        int navSelected = -1;

        while (navSelected == -1 && continueDisplay)
        {
            // Draw navigation box
            gotoxy(center_x - 25, navStartY);
            set_color(COLOR_MAGENTA);
            printf("+----------------------------------------------------+\n");

            gotoxy(center_x - 25, navStartY + 1);
            printf("|                 NAVIGATION MENU                   |\n");

            gotoxy(center_x - 25, navStartY + 2);
            printf("+----------------------------------------------------+\n");

            // Draw menu options
            for (int i = 0; i < 6; i++)
            {
                gotoxy(center_x - 25, navStartY + 3 + i);

                if (i == navCursor)
                {
                    set_color(COLOR_GREEN);
                    printf("| > %-46s |\n", navOptions[i]);
                }
                else
                {
                    set_color(COLOR_WHITE);
                    printf("|   %-46s |\n", navOptions[i]);
                }
            }

            gotoxy(center_x - 25, navStartY + 9);
            set_color(COLOR_MAGENTA);
            printf("+----------------------------------------------------+\n");

            // Instructions
            gotoxy(center_x - 25, navStartY + 11);
            set_color(COLOR_YELLOW);
            printf("ARROW KEYS: Navigate | ENTER: Select | ESC: Return");

            // Handle input
            show_cursor();
#ifdef _WIN32
            int key = _getch();
            if (key == 224 || key == 0)
            {
                key = _getch();
                if (key == 72 && navCursor > 0)
                {
                    navCursor--;
                }
                else if (key == 80 && navCursor < 5)
                {
                    navCursor++;
                }
            }
            else if (key == 13)
            {
                navSelected = navCursor;
            }
            else if (key == 27)
            {
                continueDisplay = 0;
                break;
            }
#else
            system("stty raw");
            int key = getchar();
            system("stty cooked");

            if (key == 27)
            {
                system("stty raw");
                getchar();
                key = getchar();
                system("stty cooked");

                if (key == 65 && navCursor > 0)
                {
                    navCursor--;
                }
                else if (key == 66 && navCursor < 5)
                {
                    navCursor++;
                }
            }
            else if (key == 10)
            {
                navSelected = navCursor;
            }
            else if (key == 27)
            {
                continueDisplay = 0;
                break;
            }
#endif
            hide_cursor();
        }

        // Process selection
        if (navSelected != -1)
        {
            switch (navSelected)
            {
            case 0: // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                }
                break;

            case 1: // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                }
                break;

            case 2: // Go to Page
            {
                gotoxy(center_x - 15, navStartY + 13);
                set_color(COLOR_YELLOW);
                printf("Enter page (1-%d): ", totalPages);
                show_cursor();
                int newPage;
                scanf("%d", &newPage);
                getchar();
                if (newPage >= 1 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                }
                // Clear input area
                gotoxy(center_x - 15, navStartY + 13);
                printf("                                             ");
                break;
            }

            case 3: // View Details
                if (cursorIndex >= 0 && cursorIndex < displayed)
                {
                    // Find the actual student index
                    int studentIdx = -1;
                    int count = 0;
                    for (int i = 0; i < size; i++)
                    {
                        if (index_table[i].Identifier >= Y1 &&
                            index_table[i].Identifier <= Y2 &&
                            index_table[i].Identifier >= max_birth_year)
                        {
                            if (count == startIdx + cursorIndex)
                            {
                                studentIdx = i;
                                break;
                            }
                            count++;
                        }
                    }

                    if (studentIdx != -1)
                    {
                        // Display student details
                        t_LnOVS *F2;
                        Open(&F2, fname, 'E');
                        block buf2;
                        Pr_cor coords = index_table[studentIdx].crdt;
                        ReadBlock(F2, coords.block_number, &buf2);
                        rec student = buf2.tab[coords.offset];

                        CLEAR_SCREEN();
                        display_record_detail_ui(student, center_x);

                        Close(F2);
                    }
                }
                break;

            case 5: // Return
                continueDisplay = 0;
                break;
            }
        }
    }
}

/**
 * Display student record details in enhanced UI
 */
void display_record_detail_ui(rec student, int center_x)
{
    int y = 5;

    // Header
    gotoxy(center_x - 20, 2);
    set_color(COLOR_MAGENTA);
    printf("STUDENT DETAILED INFORMATION");

    // Top border
    gotoxy(center_x - 40, y);
    set_color(COLOR_CYAN);
    printf("+------------------------------------------------------------------------+\n");
    y++;

    // Title
    gotoxy(center_x - 40, y);
    printf("|");
    gotoxy(center_x - 18, y);
    set_color(COLOR_YELLOW);
    printf("STUDENT RECORD");
    gotoxy(center_x + 22, y);
    set_color(COLOR_CYAN);
    printf("|\n");
    y++;

    // Separator
    gotoxy(center_x - 40, y);
    printf("+------------------------------------------------------------------------+\n");
    y++;

    // Display fields
    gotoxy(center_x - 40, y);
    printf("|  Student ID: %-55d |\n", student.Student_ID);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Family Name: %-53s |\n", student.Family_Name);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  First Name: %-54s |\n", student.First_Name);
    y++;

    char date_str[20];
    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
             student.Date_Birth.day, student.Date_Birth.month, student.Date_Birth.year);
    gotoxy(center_x - 40, y);
    printf("|  Birth Date: %-54s |\n", date_str);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Wilaya: %-58s |\n", student.Wilaya_Birth);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Gender: %-57s |\n", student.Gender == 1 ? "Male" : "Female");
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Blood Type: %-54s |\n", student.Blood_Type);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Year of Study: %-51s |\n", student.Year_Study);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Speciality: %-54s |\n", student.Speciality);
    y++;

    gotoxy(center_x - 40, y);
    printf("|  Campus Resident: %-50s |\n", student.Resident_UC ? "Yes" : "No");
    y++;

    // Bottom border
    gotoxy(center_x - 40, y);
    set_color(COLOR_CYAN);
    printf("+------------------------------------------------------------------------+\n");
    y += 2;

    // Continue prompt
    gotoxy(center_x - 15, y);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");

    show_cursor();
    getchar();
    hide_cursor();
}

/**
 * Simplified UI for displaying block contents with file selection
 * Shows only records table with student details below
 */
void display_block_ui(t_LnOVS *F, int blkN, char *Fname)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FILE SELECTION ==========
    CLEAR_SCREEN();
    hide_cursor();

    gotoxy(center_x - 20, 1);
    set_color(COLOR_MAGENTA);
    printf("VIEW BLOCK CONTENTS");

    gotoxy(center_x - 25, 2);
    set_color(COLOR_CYAN);
    for (int i = 0; i < 50; i++)
        printf("=");
    printf("\n\n");

    // Let user select which file to analyze
    gotoxy(center_x - 25, 5);
    set_color(COLOR_YELLOW);
    printf("Select file to view blocks:");

    int file_choice = select_file_for_analysis(8, center_x);

    if (file_choice == -1)
    {
        return; // User cancelled
    }

    // Determine the filename based on selection
    char filename[50];
    if (file_choice == 0)
    {
        strcpy(filename, "STUDENTS_ESI.bin");
    }
    else
    {
        strcpy(filename, "STUDENTS_CP.bin");
    }

    // ========== GET MAX BLOCKS ==========
    // Get maximum blocks in file
    int maxBlocks = 0;
    t_LnOVS *tempF;
    Open(&tempF, filename, 'E');
    if (tempF == NULL)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 10);
        set_color(COLOR_RED);
        printf("Error: Could not open file %s!", filename);
        gotoxy(center_x - 15, 12);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Find maximum block number by traversing the chain
    long head = getHeader(tempF, "head");
    long current = head;
    maxBlocks = 0;

    while (current != -1 && current != 0)
    {
        block buf;
        ReadBlock(tempF, current, &buf);
        if (current > maxBlocks)
            maxBlocks = current;
        current = buf.next;
    }

    Close(tempF);

    if (maxBlocks <= 0)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 25, 10);
        set_color(COLOR_RED);
        printf("Error: File %s has no blocks!", filename);
        gotoxy(center_x - 15, 12);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // ========== BLOCK NUMBER SELECTION ==========
    int currentBlock = 1; // Default to block 1

    // Ask user for block number with validation
    CLEAR_SCREEN();
    gotoxy(center_x - 20, 1);
    set_color(COLOR_MAGENTA);
    printf("SELECT BLOCK TO VIEW");

    gotoxy(center_x - 25, 2);
    set_color(COLOR_CYAN);
    for (int i = 0; i < 50; i++)
        printf("=");
    printf("\n\n");

    gotoxy(center_x - 20, 5);
    set_color(COLOR_WHITE);
    printf("File: ");
    set_color(COLOR_GREEN);
    printf("%s", filename);

    gotoxy(center_x - 20, 6);
    set_color(COLOR_WHITE);
    printf("Available blocks: ");
    set_color(COLOR_CYAN);
    printf("1 to %d", maxBlocks);

    gotoxy(center_x - 20, 8);
    set_color(COLOR_YELLOW);
    printf("Enter block number (1-%d): ", maxBlocks);

    show_cursor();
    int blockInput;
    scanf("%d", &blockInput);
    getchar(); // Clear newline
    hide_cursor();

    // Validate block number
    if (blockInput < 1 || blockInput > maxBlocks)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 20, 10);
        set_color(COLOR_RED);
        printf("Error: Block %d is out of range!", blockInput);

        gotoxy(center_x - 25, 11);
        set_color(COLOR_YELLOW);
        printf("Valid blocks are 1 to %d", maxBlocks);

        gotoxy(center_x - 15, 13);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    currentBlock = blockInput;

    int continueDisplay = 1;
    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 10;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("BLOCK %d CONTENTS", currentBlock);

        gotoxy(center_x - 10, 3);
        set_color(COLOR_YELLOW);
        printf("File: %s | Block: %d/%d", filename, currentBlock, maxBlocks);

        // ========== OPEN FILE AND READ BLOCK ==========
        Open(&F, filename, 'E');
        block buff;

        // Check if block exists before reading
        long head_check = getHeader(F, "head");
        if (head_check == -1 || head_check == 0)
        {
            gotoxy(center_x - 20, 10);
            set_color(COLOR_RED);
            printf("Error: File header corrupted!");
            Close(F);

            gotoxy(center_x - 15, 12);
            set_color(COLOR_YELLOW);
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        // Read the block - ReadBlock doesn't return a value
        ReadBlock(F, currentBlock, &buff);

        // Check if the block data appears valid
        if (buff.Nb < 0 || buff.Nb > 40)
        {
            gotoxy(center_x - 20, 10);
            set_color(COLOR_YELLOW);
            printf("Warning: Block %d has invalid record count (%d)", currentBlock, buff.Nb);

            // Try to recover by limiting to reasonable values
            if (buff.Nb < 0)
                buff.Nb = 0;
            if (buff.Nb > 40)
                buff.Nb = 40;

            gotoxy(center_x - 20, 11);
            printf("Displaying %d records...", buff.Nb);

            // Don't return - try to display what we have
        }

        int y = 6;

        // ========== RECORDS TABLE ==========
        if (buff.Nb > 0)
        {
            // Calculate pagination
            int totalPages = (buff.Nb + recordsPerPage - 1) / recordsPerPage;
            if (totalPages == 0)
                totalPages = 1;

            // Ensure currentPage is valid
            if (currentPage > totalPages)
                currentPage = totalPages;
            if (currentPage < 1)
                currentPage = 1;

            int startIdx = (currentPage - 1) * recordsPerPage;
            int endIdx = startIdx + recordsPerPage;
            if (endIdx > buff.Nb)
                endIdx = buff.Nb;

            // Page info
            gotoxy(center_x - 20, y);
            set_color(COLOR_GREEN);
            printf("Page %d/%d - Records %d to %d of %d",
                   currentPage, totalPages, startIdx + 1, endIdx, buff.Nb);
            y += 2;

            // Table header
            int tableWidth = 90;
            int tableStartX = center_x - (tableWidth / 2);

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_YELLOW);
            printf("| #  | Student ID |        Name         | Birth Date |   Wilaya   | Blood Type |\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            // Display records for current page
            int displayed = 0;
            for (int i = startIdx; i < endIdx && i < buff.Nb; i++)
            {
                rec student = buff.tab[i];

                gotoxy(tableStartX, y);

                // Highlight cursor line
                if (i == cursorIndex)
                {
                    set_color(COLOR_GREEN);
                    printf("|>%-3d", i + 1);
                    set_color(COLOR_WHITE);
                }
                else
                {
                    printf("| %-3d", i + 1);
                }

                if (student.Student_ID == -1)
                {
                    // Deleted record
                    printf("| %-10s | %-19s | %-10s | %-10s | %-10s |\n",
                           "DELETED", "", "", "", "");
                }
                else
                {
                    // Student ID
                    printf("| %-10d ", student.Student_ID);

                    // Name (truncated)
                    char name[21];
                    snprintf(name, sizeof(name), "%s %c",
                             student.Family_Name, student.First_Name[0]);
                    if (strlen(name) > 20)
                    {
                        strncpy(name, name, 17);
                        strcpy(name + 17, "...");
                    }
                    printf("| %-19s ", name);

                    // Birth date
                    char date[11];
                    snprintf(date, sizeof(date), "%02d/%02d/%04d",
                             student.Date_Birth.day,
                             student.Date_Birth.month,
                             student.Date_Birth.year);
                    printf("| %-10s ", date);

                    // Wilaya (truncated)
                    char wil[11];
                    strncpy(wil, student.Wilaya_Birth, 10);
                    wil[10] = '\0';
                    if (strlen(student.Wilaya_Birth) > 10)
                    {
                        strcpy(wil + 7, "...");
                    }
                    printf("| %-10s ", wil);

                    // Blood type
                    printf("| %-10s |\n", student.Blood_Type);
                }
                y++;
                displayed++;
            }

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y += 2;

            // ========== CURRENT RECORD DETAILS (VERTICAL FORMAT) ==========
            if (cursorIndex >= 0 && cursorIndex < buff.Nb)
            {
                rec currentStudent = buff.tab[cursorIndex];

                if (currentStudent.Student_ID != -1)
                {
                    gotoxy(center_x - 20, y);
                    set_color(COLOR_MAGENTA);
                    printf("SELECTED STUDENT DETAILS:");
                    y++;

                    // Create a vertical details table with consistent colors
                    int detailsWidth = 50;
                    int detailsStartX = center_x - (detailsWidth / 2);

                    // Top border - Cyan color
                    gotoxy(detailsStartX, y);
                    set_color(COLOR_CYAN);
                    printf("+--------------------------------------------------+\n");
                    y++;

                    // Title - Yellow color
                    gotoxy(detailsStartX, y);
                    printf("|");
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_YELLOW);
                    printf("STUDENT RECORD");
                    gotoxy(detailsStartX + detailsWidth - 1, y);
                    set_color(COLOR_CYAN);
                    printf("|\n");
                    y++;

                    // Separator - Cyan color
                    gotoxy(detailsStartX, y);
                    printf("+--------------------------------------------------+\n");
                    y++;

                    // Student ID - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Student ID: ");
                    set_color(COLOR_WHITE);
                    printf("%-35d", currentStudent.Student_ID);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Family Name - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Family Name: ");
                    set_color(COLOR_WHITE);
                    printf("%-33s", currentStudent.Family_Name);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // First Name - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("First Name: ");
                    set_color(COLOR_WHITE);
                    printf("%-34s", currentStudent.First_Name);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Birth Date - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Birth Date: ");
                    char date_str[20];
                    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
                             currentStudent.Date_Birth.day,
                             currentStudent.Date_Birth.month,
                             currentStudent.Date_Birth.year);
                    set_color(COLOR_WHITE);
                    printf("%-34s", date_str);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Wilaya - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Wilaya: ");
                    set_color(COLOR_WHITE);
                    printf("%-38s", currentStudent.Wilaya_Birth);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Gender - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Gender: ");
                    set_color(COLOR_WHITE);
                    printf("%-37s",
                           currentStudent.Gender == 1 ? "Male" : "Female");
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Blood Type - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Blood Type: ");
                    set_color(COLOR_WHITE);
                    printf("%-34s", currentStudent.Blood_Type);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Year of Study - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Year of Study: ");
                    set_color(COLOR_WHITE);
                    printf("%-31s", currentStudent.Year_Study);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Speciality - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Speciality: ");
                    set_color(COLOR_WHITE);
                    printf("%-34s", currentStudent.Speciality);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Campus Resident - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Campus Resident: ");
                    set_color(COLOR_WHITE);
                    printf("%-29s", currentStudent.Resident_UC ? "Yes" : "No");
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Location - Yellow label, White value
                    gotoxy(detailsStartX, y);
                    printf("|  ");
                    set_color(COLOR_YELLOW);
                    printf("Location: ");
                    set_color(COLOR_WHITE);
                    printf("Block %-31d", currentBlock);
                    set_color(COLOR_CYAN);
                    printf(" |\n");
                    y++;

                    // Bottom border - Cyan color
                    gotoxy(detailsStartX, y);
                    printf("+--------------------------------------------------+\n");
                    y += 2;
                }
            }
        }
        else
        {
            gotoxy(center_x - 10, y);
            set_color(COLOR_YELLOW);
            printf("Block %d is empty or has no valid records.", currentBlock);
            y += 2;
        }

        // Close file
        Close(F);

        // ========== NAVIGATION INSTRUCTIONS ==========
        y += 2;
        gotoxy(center_x - 30, y);
        set_color(COLOR_CYAN);
        printf("Navigation: ");

        gotoxy(center_x - 30, y + 1);
        set_color(COLOR_WHITE);
        printf("UP/DOWN: Select student | LEFT/RIGHT: Change page");

        gotoxy(center_x - 30, y + 2);
        printf("N: Next Block | P: Previous Block | B: Go to Block");

        gotoxy(center_x - 30, y + 3);
        printf("R: Return to Menu | ESC: Exit");

        y += 5;

        // ========== INPUT HANDLING ==========
        show_cursor();
        int keyPressed = 0;

#ifdef _WIN32
        int key = _getch();

        // Handle arrow keys for navigation
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72: // Up arrow - move cursor up
                if (buff.Nb > 0 && cursorIndex > 0)
                {
                    cursorIndex--;
                    if (cursorIndex < (currentPage - 1) * recordsPerPage)
                    {
                        currentPage--;
                        if (currentPage < 1)
                            currentPage = 1;
                    }
                    keyPressed = 1;
                }
                break;

            case 80: // Down arrow - move cursor down
                if (buff.Nb > 0 && cursorIndex < buff.Nb - 1)
                {
                    cursorIndex++;
                    if (cursorIndex >= currentPage * recordsPerPage)
                    {
                        currentPage++;
                        int totalPages = (buff.Nb + recordsPerPage - 1) / recordsPerPage;
                        if (currentPage > totalPages)
                            currentPage = totalPages;
                    }
                    keyPressed = 1;
                }
                break;

            case 75: // Left arrow - previous page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                    keyPressed = 1;
                }
                break;

            case 77: // Right arrow - next page
                if (currentPage < ((buff.Nb + recordsPerPage - 1) / recordsPerPage))
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                    if (cursorIndex >= buff.Nb)
                        cursorIndex = buff.Nb - 1;
                    keyPressed = 1;
                }
                break;
            }
        }
        else
        {
            // Handle letter keys for commands
            key = toupper(key);

            switch (key)
            {
            case 'N': // Next Block
                if (currentBlock < maxBlocks)
                {
                    currentBlock++;
                    currentPage = 1;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("No next block available! Max: %d", maxBlocks);
                    SLEEP_MS(1000);
                }
                break;

            case 'P': // Previous Block
                if (currentBlock > 1)
                {
                    currentBlock--;
                    currentPage = 1;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("No previous block available! Already at block 1");
                    SLEEP_MS(1000);
                }
                break;

            case 'B': // Go to Block
            {
                int newBlock;
                gotoxy(center_x - 15, y);
                set_color(COLOR_YELLOW);
                printf("Enter block number (1-%d): ", maxBlocks);
                show_cursor();
                scanf("%d", &newBlock);
                getchar();

                if (newBlock > 0 && newBlock <= maxBlocks)
                {
                    currentBlock = newBlock;
                    currentPage = 1;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show error message
                    gotoxy(center_x - 20, y + 1);
                    set_color(COLOR_RED);
                    printf("Error: Block %d is out of range (1-%d)!", newBlock, maxBlocks);
                    SLEEP_MS(1500);
                }
                break;
            }

            case 'R': // Return to Menu
                continueDisplay = 0;
                keyPressed = 1;
                break;

            case 27: // ESC - Exit
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux implementation would go here
        // Similar logic but with Linux terminal handling
#endif

        if (!keyPressed)
        {
            SLEEP_MS(100);
        }

        hide_cursor();
    }

    hide_cursor();
}

/**
 * Enhanced UI for displaying all file contents with same UI as block display
 */
void display_all_contents_ui(t_LnOVS *F, char *default_filename)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FILE SELECTION ==========
    CLEAR_SCREEN();
    hide_cursor();

    // File selection prompt
    gotoxy(center_x - 25, 5);
    set_color(COLOR_YELLOW);
    printf("Select file to view all contents:");

    int file_choice = select_file_for_analysis(8, center_x);

    if (file_choice == -1)
    {
        return; // User cancelled
    }

    // Determine the filename based on selection
    char *filename;
    if (file_choice == 0)
    {
        filename = "STUDENTS_ESI.bin";
    }
    else
    {
        filename = "STUDENTS_CP.bin";
    }

    // ========== COUNT TOTAL RECORDS AND STORE POSITIONS ==========
    t_LnOVS *tempF;
    Open(&tempF, filename, 'E');
    if (tempF == NULL)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 10);
        set_color(COLOR_RED);
        printf("Error: Could not open file %s!", filename);
        gotoxy(center_x - 15, 12);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Get header info
    long head = getHeader(tempF, "head");
    long tail = getHeader(tempF, "tail");
    long freepos = getHeader(tempF, "freepos");

    // Count total records and store their positions
    int totalRecords = 0;
    long currentBlock = head;

    // First pass: count total non-deleted records
    while (currentBlock != -1 && currentBlock != 0)
    {
        block buf;
        ReadBlock(tempF, currentBlock, &buf);

        int maxRecords = buf.Nb;
        if (currentBlock == tail && freepos > 0 && freepos <= buf.Nb)
        {
            maxRecords = freepos;
        }

        for (int i = 0; i < maxRecords; i++)
        {
            if (buf.tab[i].Student_ID != -1)
            {
                totalRecords++;
            }
        }

        currentBlock = buf.next;
    }

    if (totalRecords == 0)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 10);
        set_color(COLOR_YELLOW);
        printf("File is empty!");
        gotoxy(center_x - 15, 12);
        set_color(COLOR_CYAN);
        printf("Press Enter to continue...");
        getchar();
        Close(tempF);
        return;
    }

    // Allocate array to store record positions
    typedef struct
    {
        long blockNum;
        int offset;
    } RecordPosition;

    RecordPosition *recordPositions = malloc(totalRecords * sizeof(RecordPosition));
    if (recordPositions == NULL)
    {
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 10);
        set_color(COLOR_RED);
        printf("Error: Memory allocation failed!");
        gotoxy(center_x - 15, 12);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        Close(tempF);
        return;
    }

    // Second pass: store positions of all non-deleted records
    currentBlock = head;
    int recordIndex = 0;

    while (currentBlock != -1 && currentBlock != 0 && recordIndex < totalRecords)
    {
        block buf;
        ReadBlock(tempF, currentBlock, &buf);

        int maxRecords = buf.Nb;
        if (currentBlock == tail && freepos > 0 && freepos <= buf.Nb)
        {
            maxRecords = freepos;
        }

        for (int i = 0; i < maxRecords && recordIndex < totalRecords; i++)
        {
            if (buf.tab[i].Student_ID != -1)
            {
                recordPositions[recordIndex].blockNum = currentBlock;
                recordPositions[recordIndex].offset = i;
                recordIndex++;
            }
        }

        currentBlock = buf.next;
    }

    Close(tempF);

    // Verify we found all records
    if (recordIndex != totalRecords)
    {
        totalRecords = recordIndex; // Update with actual count
    }

    // ========== MAIN DISPLAY LOOP ==========
    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 10;
    int totalPages = (totalRecords + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0)
        totalPages = 1;

    int continueDisplay = 1;

    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();

        // ========== HEADER SECTION ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++)
            printf("=");
        printf("\n");

        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("ALL FILE CONTENTS");

        gotoxy(center_x - 10, 3);
        set_color(COLOR_YELLOW);
        printf("File: %s | Total Records: %d", filename, totalRecords);

        // ========== OPEN FILE ==========
        Open(&F, filename, 'E');

        // Calculate current page indices
        int startIdx = (currentPage - 1) * recordsPerPage;
        int endIdx = startIdx + recordsPerPage;
        if (endIdx > totalRecords)
            endIdx = totalRecords;

        int y = 6;

        // ========== RECORDS TABLE ==========
        if (totalRecords > 0)
        {
            // Page info
            gotoxy(center_x - 20, y);
            set_color(COLOR_GREEN);
            printf("Page %d/%d - Records %d to %d of %d",
                   currentPage, totalPages, startIdx + 1, endIdx, totalRecords);
            y += 2;

            // Table header
            int tableWidth = 90;
            int tableStartX = center_x - (tableWidth / 2);

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_YELLOW);
            printf("| #  | Student ID |        Name         | Birth Date |   Wilaya   | Blood Type |\n");
            y++;

            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y++;

            // Display records for current page using stored positions
            int displayed = 0;
            rec selectedStudent;
            bool foundSelected = false;
            block buffer;

            for (int i = startIdx; i < endIdx && i < totalRecords; i++)
            {
                // Get record position from stored array
                RecordPosition pos = recordPositions[i];

                // Read the block and get the record
                ReadBlock(F, pos.blockNum, &buffer);
                rec student = buffer.tab[pos.offset];

                gotoxy(tableStartX, y);

                // Highlight cursor line
                if (i == startIdx + cursorIndex)
                {
                    set_color(COLOR_GREEN);
                    printf("|>%-3d", i + 1);
                    set_color(COLOR_WHITE);
                    // Store the selected student for details display
                    selectedStudent = student;
                    foundSelected = true;
                }
                else
                {
                    printf("| %-3d", i + 1);
                }

                // Student ID
                printf("| %-10d ", student.Student_ID);

                // Name (truncated)
                char name[21];
                snprintf(name, sizeof(name), "%s %c",
                         student.Family_Name, student.First_Name[0]);
                if (strlen(name) > 20)
                {
                    strncpy(name, name, 17);
                    strcpy(name + 17, "...");
                }
                printf("| %-19s ", name);

                // Birth date
                char date[11];
                snprintf(date, sizeof(date), "%02d/%02d/%04d",
                         student.Date_Birth.day,
                         student.Date_Birth.month,
                         student.Date_Birth.year);
                printf("| %-10s ", date);

                // Wilaya (truncated)
                char wil[11];
                strncpy(wil, student.Wilaya_Birth, 10);
                wil[10] = '\0';
                if (strlen(student.Wilaya_Birth) > 10)
                {
                    strcpy(wil + 7, "...");
                }
                printf("| %-10s ", wil);

                // Blood type
                printf("| %-10s |\n", student.Blood_Type);

                y++;
                displayed++;
            }

            // Table footer
            gotoxy(tableStartX, y);
            set_color(COLOR_CYAN);
            printf("+----+------------+---------------------+------------+------------+------------+\n");
            y += 2;

            // ========== CURRENT RECORD DETAILS (VERTICAL FORMAT) ==========
            if (foundSelected)
            {
                gotoxy(center_x - 20, y);
                set_color(COLOR_MAGENTA);
                printf("SELECTED STUDENT DETAILS:");
                y++;

                // Create a vertical details table with consistent colors
                int detailsWidth = 50;
                int detailsStartX = center_x - (detailsWidth / 2);

                // Top border - Cyan color
                gotoxy(detailsStartX, y);
                set_color(COLOR_CYAN);
                printf("+--------------------------------------------------+\n");
                y++;

                // Title - Yellow color
                gotoxy(detailsStartX, y);
                printf("|");
                gotoxy(center_x - 10, y);
                set_color(COLOR_YELLOW);
                printf("STUDENT RECORD");
                gotoxy(detailsStartX + detailsWidth - 1, y);
                set_color(COLOR_CYAN);
                printf("|\n");
                y++;

                // Separator - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y++;

                // Student ID - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Student ID: ");
                set_color(COLOR_WHITE);
                printf("%-35d", selectedStudent.Student_ID);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Family Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Family Name: ");
                set_color(COLOR_WHITE);
                printf("%-33s", selectedStudent.Family_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // First Name - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("First Name: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.First_Name);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Birth Date - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Birth Date: ");
                char date_str[20];
                snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
                         selectedStudent.Date_Birth.day,
                         selectedStudent.Date_Birth.month,
                         selectedStudent.Date_Birth.year);
                set_color(COLOR_WHITE);
                printf("%-34s", date_str);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Wilaya - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Wilaya: ");
                set_color(COLOR_WHITE);
                printf("%-38s", selectedStudent.Wilaya_Birth);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Gender - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Gender: ");
                set_color(COLOR_WHITE);
                printf("%-37s",
                       selectedStudent.Gender == 1 ? "Male" : "Female");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Blood Type - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Blood Type: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Blood_Type);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Year of Study - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Year of Study: ");
                set_color(COLOR_WHITE);
                printf("%-31s", selectedStudent.Year_Study);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Speciality - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Speciality: ");
                set_color(COLOR_WHITE);
                printf("%-34s", selectedStudent.Speciality);
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Campus Resident - Yellow label, White value
                gotoxy(detailsStartX, y);
                printf("|  ");
                set_color(COLOR_YELLOW);
                printf("Campus Resident: ");
                set_color(COLOR_WHITE);
                printf("%-29s", selectedStudent.Resident_UC ? "Yes" : "No");
                set_color(COLOR_CYAN);
                printf(" |\n");
                y++;

                // Bottom border - Cyan color
                gotoxy(detailsStartX, y);
                printf("+--------------------------------------------------+\n");
                y += 2;
            }
        }
        else
        {
            gotoxy(center_x - 10, y);
            set_color(COLOR_YELLOW);
            printf("No records found.");
            y += 2;
        }

        // Close file
        Close(F);

        // ========== NAVIGATION INSTRUCTIONS ==========
        y += 2;
        gotoxy(center_x - 30, y);
        set_color(COLOR_CYAN);
        printf("Navigation: ");

        gotoxy(center_x - 30, y + 1);
        set_color(COLOR_WHITE);
        printf("UP/DOWN: Select record | LEFT/RIGHT: Change page");

        gotoxy(center_x - 30, y + 2);
        printf("N: Next Page | P: Previous Page | G: Go to Page");

        gotoxy(center_x - 30, y + 3);
        printf("R: Return to Menu | ESC: Exit");

        y += 5;

        // ========== INPUT HANDLING ==========
        show_cursor();
        int keyPressed = 0;

#ifdef _WIN32
        int key = _getch();

        // Handle arrow keys for navigation
        if (key == 224 || key == 0)
        {
            key = _getch();
            switch (key)
            {
            case 72: // Up arrow - move cursor up
                if (totalRecords > 0 && cursorIndex > 0)
                {
                    cursorIndex--;
                    keyPressed = 1;
                }
                break;

            case 80: // Down arrow - move cursor down
                if (totalRecords > 0 && cursorIndex < (endIdx - startIdx - 1))
                {
                    cursorIndex++;
                    keyPressed = 1;
                }
                break;

            case 75: // Left arrow - previous page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;

            case 77: // Right arrow - next page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                break;
            }
        }
        else
        {
            // Handle letter keys for commands
            key = toupper(key);

            switch (key)
            {
            case 'N': // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on last page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'P': // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show message
                    gotoxy(center_x - 10, y);
                    set_color(COLOR_RED);
                    printf("Already on first page!");
                    SLEEP_MS(1000);
                }
                break;

            case 'G': // Go to Page
            {
                int newPage;
                gotoxy(center_x - 15, y);
                set_color(COLOR_YELLOW);
                printf("Enter page number (1-%d): ", totalPages);
                show_cursor();
                scanf("%d", &newPage);
                getchar();

                if (newPage > 0 && newPage <= totalPages)
                {
                    currentPage = newPage;
                    cursorIndex = 0;
                    keyPressed = 1;
                }
                else
                {
                    // Show error message
                    gotoxy(center_x - 20, y + 1);
                    set_color(COLOR_RED);
                    printf("Error: Page %d is out of range!", newPage);
                    SLEEP_MS(1500);
                }
                break;
            }

            case 'R': // Return to Menu
                continueDisplay = 0;
                keyPressed = 1;
                break;

            case 27: // ESC - Exit
                continueDisplay = 0;
                keyPressed = 1;
                break;
            }
        }
#else
        // Linux implementation would go here
        // Similar logic but with Linux terminal handling
#endif

        if (!keyPressed)
        {
            SLEEP_MS(100);
        }

        hide_cursor();
    }

    // ========== CLEANUP ==========
    // Free allocated memory
    if (recordPositions != NULL)
    {
        free(recordPositions);
    }

    hide_cursor();
}

/**
 * Select file with UI similar to blood type selection
 */
int select_file_for_analysis(int start_y, int center_x)
{
    const char *file_options[] = {
        "STUDENTS_ESI.bin - Main Database File",
        "STUDENTS_CP.bin  - CP Database File"};

    int cursor = 0;
    int selected = -1;

    while (selected == -1)
    {
        // Clear the selection area
        for (int i = 0; i < 10; i++)
        {
            gotoxy(center_x - 35, start_y + i);
            printf("%-70s", " ");
        }

        // ========== FILE SELECTION BOX ==========
        gotoxy(center_x - 35, start_y);
        set_color(COLOR_BLUE);
        printf("+==================================================================+\n");

        gotoxy(center_x - 35, start_y + 1);
        printf("|");
        gotoxy(center_x + 33, start_y + 1);
        printf("|");

        // Title - centered
        char title[] = "SELECT DATABASE FILE";
        int title_len = strlen(title);
        int title_start = center_x - (title_len / 2);

        gotoxy(title_start, start_y + 1);
        set_color(COLOR_YELLOW);
        printf("%s", title);

        gotoxy(center_x - 35, start_y + 2);
        set_color(COLOR_BLUE);
        printf("+==================================================================+\n");

        // Display file options
        for (int i = 0; i < 2; i++)
        {
            gotoxy(center_x - 35, start_y + 3 + i);
            set_color(COLOR_BLUE);
            printf("|");

            gotoxy(center_x - 32, start_y + 3 + i);
            if (i == cursor)
            {
                set_color(COLOR_GREEN);
                printf("> ");
                set_color(COLOR_WHITE);
            }
            else
            {
                set_color(COLOR_GRAY);
                printf("  ");
                set_color(COLOR_WHITE);
            }

            // Display file info with formatting
            printf("%-60s", file_options[i]);

            gotoxy(center_x + 33, start_y + 3 + i);
            set_color(COLOR_BLUE);
            printf("|");
        }

        gotoxy(center_x - 35, start_y + 5);
        set_color(COLOR_BLUE);
        printf("+==================================================================+\n");

        // ========== FILE INFORMATION ==========
        gotoxy(center_x - 35, start_y + 7);
        set_color(COLOR_CYAN);
        printf("File Information:");

        // Show info about selected file (simplified as requested)
        if (cursor == 0)
        {
            gotoxy(center_x - 35, start_y + 8);
            set_color(COLOR_WHITE);
            printf("- Contains complete student records (all years)");
        }
        else
        {
            gotoxy(center_x - 35, start_y + 8);
            set_color(COLOR_WHITE);
            printf("- Contains only 1CP and 2CP student records");
        }

        // ========== INSTRUCTIONS ==========
        gotoxy(center_x - 35, start_y + 10);
        set_color(COLOR_YELLOW);
        printf("UP/DOWN: Navigate   ENTER: Select   ESC: Cancel");

        // Get input
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && cursor > 0)
            {
                cursor--;
            }
            else if (key == 80 && cursor < 1)
            {
                cursor++;
            }
        }
        else if (key == 13)
        {
            selected = cursor;
        }
        else if (key == 27)
        {
            return -1;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");

            if (key == 65 && cursor > 0)
            {
                cursor--;
            }
            else if (key == 66 && cursor < 1)
            {
                cursor++;
            }
        }
        else if (key == 10)
        {
            selected = cursor;
        }
        else if (key == 27)
        {
            return -1;
        }
#endif
    }

    return selected;
}
/**
 * Get block number with nice UI
 */
int get_block_number_ui(int start_y, int center_x, char *filename)
{
    int block_num = 1;
    int confirmed = 0;

    // Try to get max blocks from file
    int max_blocks = 100; // Default
    t_LnOVS *F;
    Open(&F, filename, 'E');
    if (F != NULL)
    {
        // Try to find last block
        long head = getHeader(F, "head");
        long current = head;
        max_blocks = 0;

        while (current != -1)
        {
            block buf;
            ReadBlock(F, current, &buf);
            max_blocks++;
            current = buf.next;
        }

        if (max_blocks == 0)
            max_blocks = 10; // Fallback
        Close(F);
    }

    while (!confirmed)
    {
        // Clear area
        for (int i = 0; i < 15; i++)
        {
            gotoxy(center_x - 35, start_y + i);
            printf("%-70s", " ");
        }

        // ========== BLOCK SELECTION BOX ==========
        gotoxy(center_x - 35, start_y);
        set_color(COLOR_MAGENTA);
        printf("+==================================================================+\n");

        gotoxy(center_x - 35, start_y + 1);
        printf("|");
        gotoxy(center_x + 33, start_y + 1);
        printf("|");

        char title[100];
        snprintf(title, sizeof(title), "ENTER BLOCK NUMBER FOR %s", filename);
        int title_len = strlen(title);
        int title_start = center_x - (title_len / 2);

        gotoxy(title_start, start_y + 1);
        set_color(COLOR_YELLOW);
        printf("%s", title);

        gotoxy(center_x - 35, start_y + 2);
        set_color(COLOR_MAGENTA);
        printf("+==================================================================+\n");

        // Display current block number
        gotoxy(center_x - 35, start_y + 4);
        set_color(COLOR_BLUE);
        printf("|");
        gotoxy(center_x - 25, start_y + 4);
        set_color(COLOR_WHITE);
        printf("Block Number: ");
        set_color(COLOR_CYAN);
        printf("[ %3d ]", block_num);
        gotoxy(center_x + 33, start_y + 4);
        set_color(COLOR_BLUE);
        printf("|");

        // Range info
        gotoxy(center_x - 35, start_y + 5);
        printf("|");
        gotoxy(center_x - 30, start_y + 5);
        set_color(COLOR_GRAY);
        printf("Valid range: 1 to %d", max_blocks);
        gotoxy(center_x + 33, start_y + 5);
        set_color(COLOR_BLUE);
        printf("|");

        gotoxy(center_x - 35, start_y + 6);
        printf("+==================================================================+\n");

        // ========== CONTROLS ==========
        gotoxy(center_x - 35, start_y + 8);
        set_color(COLOR_CYAN);
        printf("Controls:");

        gotoxy(center_x - 35, start_y + 9);
        set_color(COLOR_WHITE);
        printf("↑/↓  : Increase/Decrease block number");

        gotoxy(center_x - 35, start_y + 10);
        printf("+/-  : Increase/Decrease by 10");

        gotoxy(center_x - 35, start_y + 11);
        printf("ENTER: Confirm selection");

        gotoxy(center_x - 35, start_y + 12);
        printf("ESC  : Cancel");

        // ========== FILE INFO ==========
        gotoxy(center_x - 35, start_y + 14);
        set_color(COLOR_YELLOW);
        printf("Selected file: %s", filename);

        // Get input
        show_cursor();
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && block_num < max_blocks)
            { // Up
                block_num++;
            }
            else if (key == 80 && block_num > 1)
            { // Down
                block_num--;
            }
        }
        else if (key == 43 && block_num + 10 <= max_blocks)
        { // '+'
            block_num += 10;
        }
        else if (key == 45 && block_num - 10 >= 1)
        { // '-'
            block_num -= 10;
        }
        else if (key == 13)
        {
            confirmed = 1;
        }
        else if (key == 27)
        {
            return -1;
        }
#else
        // Linux implementation
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");

            if (key == 65 && block_num < max_blocks)
            { // Up
                block_num++;
            }
            else if (key == 66 && block_num > 1)
            { // Down
                block_num--;
            }
        }
        else if (key == 43 && block_num + 10 <= max_blocks)
        { // '+'
            block_num += 10;
        }
        else if (key == 45 && block_num - 10 >= 1)
        { // '-'
            block_num -= 10;
        }
        else if (key == 10)
        {
            confirmed = 1;
        }
        else if (key == 27)
        {
            return -1;
        }
#endif
        hide_cursor();
    }

    return block_num;
}

/**
 * Enhanced file header display with option to view blocks
 */
void display_header_ui(char *default_filename)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // ========== FILE SELECTION SECTION ==========
    gotoxy(center_x - 20, 1);
    set_color(COLOR_MAGENTA);
    printf("FILE HEADER ANALYSIS");

    gotoxy(center_x - 25, 2);
    set_color(COLOR_CYAN);
    for (int i = 0; i < 50; i++)
        printf("=");
    printf("\n\n");

    // Let user select which file to analyze
    gotoxy(center_x - 25, 5);
    set_color(COLOR_YELLOW);
    printf("Select file to analyze header:");

    int file_choice = select_file_for_analysis(8, center_x);

    if (file_choice == -1)
    {
        // User cancelled
        return;
    }

    // Determine the filename based on selection
    char *filename;
    char filename_buffer[50];

    if (file_choice == 0)
    {
        filename = "STUDENTS_ESI.bin";
    }
    else
    {
        filename = "STUDENTS_CP.bin";
    }

    // Store the filename in buffer for display
    strcpy(filename_buffer, filename);

    // ========== HEADER ==========
    CLEAR_SCREEN();
    gotoxy(center_x - 20, 1);
    set_color(COLOR_MAGENTA);
    printf("FILE HEADER ANALYSIS");

    gotoxy(center_x - 25, 2);
    set_color(COLOR_CYAN);
    for (int i = 0; i < 50; i++)
        printf("=");
    printf("\n");

    // File info
    gotoxy(center_x - 15, 4);
    set_color(COLOR_YELLOW);
    printf("Analyzing: %s", filename_buffer);

    // Display file type
    gotoxy(center_x - 15, 5);
    if (file_choice == 0)
    {
        set_color(COLOR_GREEN);
        printf("(Main Database File)");
    }
    else
    {
        set_color(COLOR_BLUE);
        printf("(CP Database File)");
    }

    gotoxy(center_x - 25, 6);
    set_color(COLOR_CYAN);
    for (int i = 0; i < 50; i++)
        printf("-");
    printf("\n\n");

    // Open file
    t_LnOVS *F;
    Open(&F, filename, 'E');
    if (F == NULL)
    {
        gotoxy(center_x - 15, 10);
        set_color(COLOR_RED);
        printf("Error: Could not open file!");

        gotoxy(center_x - 20, 12);
        printf("File '%s' may not exist.", filename_buffer);

        gotoxy(center_x - 15, 14);
        set_color(COLOR_YELLOW);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // Get header values
    long head = getHeader(F, "head");
    long tail = getHeader(F, "tail");
    long freepos = getHeader(F, "freepos");
    long lostspace = getHeader(F, "lostspace");
    long newblck = getHeader(F, "newblck");
    long freeblck = getHeader(F, "freeblck");

    // ========== HEADER VALUES TABLE ==========
    int tableWidth = 50;
    int tableStartX = center_x - (tableWidth / 2);
    int tableStartY = 8;

    // First table - HEADER VALUES
    gotoxy(tableStartX, tableStartY);
    set_color(COLOR_CYAN);
    printf("+--------------------------------------------------+\n");

    gotoxy(tableStartX, tableStartY + 1);
    printf("|");
    gotoxy(center_x - 10, tableStartY + 1);
    set_color(COLOR_YELLOW);
    printf("HEADER VALUES");
    gotoxy(tableStartX + tableWidth - 1, tableStartY + 1);
    set_color(COLOR_CYAN);
    printf("|\n");

    gotoxy(tableStartX, tableStartY + 2);
    printf("+--------------------------------------------------+\n");

    // Display header values
    int lineY = tableStartY + 3;

    // Head Block
    gotoxy(tableStartX, lineY);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Head Block: ");
    set_color(COLOR_WHITE);
    printf("%-35ld", head);
    set_color(COLOR_CYAN);
    printf(" |\n");
    lineY++;

    // Tail Block
    gotoxy(tableStartX, lineY);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Tail Block: ");
    set_color(COLOR_WHITE);
    printf("%-35ld", tail);
    set_color(COLOR_CYAN);
    printf(" |\n");
    lineY++;

    // Free Position
    gotoxy(tableStartX, lineY);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Free Position: ");
    set_color(COLOR_WHITE);
    printf("%-32ld", freepos);
    set_color(COLOR_CYAN);
    printf(" |\n");
    lineY++;

    // Lost Space
    gotoxy(tableStartX, lineY);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Lost Space: ");
    set_color(COLOR_WHITE);
    printf("%-35ld", lostspace);
    set_color(COLOR_CYAN);
    printf(" |\n");
    lineY++;

    // New Block Border
    gotoxy(tableStartX, lineY);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("New Block Border: ");
    set_color(COLOR_WHITE);
    printf("%-29ld", newblck);
    set_color(COLOR_CYAN);
    printf(" |\n");
    lineY++;

    // Free Block List
    gotoxy(tableStartX, lineY);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Free Block List: ");
    set_color(COLOR_WHITE);
    printf("%-30ld", freeblck);
    set_color(COLOR_CYAN);
    printf(" |\n");
    lineY++;

    // Close first table
    gotoxy(tableStartX, lineY);
    printf("+--------------------------------------------------+\n");

    // ========== HEADER VALUE EXPLANATIONS TABLE ==========
    lineY += 3; // Add some space between tables

    // Start second table at the new lineY position
    gotoxy(tableStartX, lineY);
    set_color(COLOR_CYAN);
    printf("+--------------------------------------------------+\n");

    gotoxy(tableStartX, lineY + 1);
    printf("|");
    gotoxy(center_x - 12, lineY + 1);
    set_color(COLOR_YELLOW);
    printf("HEADER EXPLANATIONS");
    gotoxy(tableStartX + tableWidth - 1, lineY + 1);
    set_color(COLOR_CYAN);
    printf("|\n");

    gotoxy(tableStartX, lineY + 2);
    printf("+--------------------------------------------------+\n");

    // Explanations with proper right borders
    int currentLine = lineY + 3;

    // Head Block explanation
    gotoxy(tableStartX, currentLine);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Head Block: ");
    set_color(COLOR_WHITE);
    printf("First block in chain");
    // Add spaces to fill exactly to the border
    int remainingSpaces = tableWidth - 3 - 11 - 19 - 1; // 3 for "|  ", 11 for "Head Block: ", 19 for text, 1 for border
    for (int i = 0; i < remainingSpaces; i++)
        printf(" ");
    set_color(COLOR_CYAN);
    printf("|\n");
    currentLine++;

    // Tail Block explanation
    gotoxy(tableStartX, currentLine);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Tail Block: ");
    set_color(COLOR_WHITE);
    printf("Last block in chain");
    remainingSpaces = tableWidth - 3 - 11 - 18 - 1;
    for (int i = 0; i < remainingSpaces; i++)
        printf(" ");
    set_color(COLOR_CYAN);
    printf("|\n");
    currentLine++;

    // Free Position explanation
    gotoxy(tableStartX, currentLine);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Free Position: ");
    set_color(COLOR_WHITE);
    printf("Available spots");
    remainingSpaces = tableWidth - 3 - 15 - 15 - 1;
    for (int i = 0; i < remainingSpaces; i++)
        printf(" ");
    set_color(COLOR_CYAN);
    printf("|\n");
    currentLine++;

    // Lost Space explanation
    gotoxy(tableStartX, currentLine);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Lost Space: ");
    set_color(COLOR_WHITE);
    printf("Deleted records space");
    remainingSpaces = tableWidth - 3 - 11 - 21 - 1;
    for (int i = 0; i < remainingSpaces; i++)
        printf(" ");
    set_color(COLOR_CYAN);
    printf("|\n");
    currentLine++;

    // New Block Border explanation
    gotoxy(tableStartX, currentLine);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("New Block Border: ");
    set_color(COLOR_WHITE);
    printf("Next block number");
    remainingSpaces = tableWidth - 3 - 18 - 17 - 1;
    for (int i = 0; i < remainingSpaces; i++)
        printf(" ");
    set_color(COLOR_CYAN);
    printf("|\n");
    currentLine++;

    // Free Block List explanation
    gotoxy(tableStartX, currentLine);
    printf("|  ");
    set_color(COLOR_YELLOW);
    printf("Free Block List: ");
    set_color(COLOR_WHITE);
    printf("Empty blocks list");
    remainingSpaces = tableWidth - 3 - 17 - 17 - 1;
    for (int i = 0; i < remainingSpaces; i++)
        printf(" ");
    set_color(COLOR_CYAN);
    printf("|\n");

    // Close second table with bottom border
    gotoxy(tableStartX, currentLine + 1);
    printf("+--------------------------------------------------+\n");

    // ========== NAVIGATION ==========
    currentLine += 4;
    gotoxy(center_x - 15, currentLine);
    set_color(COLOR_CYAN);
    printf("Press Enter to continue... ");

    // Close file
    Close(F);

    show_cursor();
    getchar();
    hide_cursor();
}

/**
 * Complete workflow for creating LOF file (STUDENTS_CP.BIN)
 */
void create_lof_file_workflow()
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    CLEAR_SCREEN();
    hide_cursor();

    // Step 1: Confirmation screen
    gotoxy(center_x - 20, 3);
    set_color(COLOR_CYAN);
    printf("CREATE LOF FILE - STUDENTS_CP.BIN");

    gotoxy(center_x - 30, 6);
    set_color(COLOR_CYAN);
    printf("============================================================");

    int y = 8;

    // Display information about the operation
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Operation: ");
    set_color(COLOR_CYAN);
    printf("Create STUDENTS_CP.bin File (LOF)");

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Source File: ");
    set_color(COLOR_CYAN);
    printf("STUDENTS_ESI.bin");

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Destination File: ");
    set_color(COLOR_CYAN);
    printf("STUDENTS_CP.bin");

    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Content Filter: ");
    set_color(COLOR_CYAN);
    printf("Only 1CP and 2CP students");

    y += 1;

    // Show statistics if index is loaded
    if (index_size > 0)
    {
        // Count 1CP and 2CP students
        int cp_student_count = 0;
        t_LnOVS *F;
        Open(&F, fname, 'E');
        block buffer;

        for (int i = 0; i < index_size; i++)
        {
            Pr_cor coords = index_pr[i].crdt;
            ReadBlock(F, coords.block_number, &buffer);
            rec student = buffer.tab[coords.offset];

            if (strcmp(student.Year_Study, "1CP") == 0 ||
                strcmp(student.Year_Study, "2CP") == 0)
            {
                cp_student_count++;
            }
        }
        Close(F);

        gotoxy(center_x - 25, y++);
        set_color(COLOR_WHITE);
        printf("Total CP Students Found: ");
        set_color(COLOR_CYAN);
        printf("%d", cp_student_count);

        gotoxy(center_x - 25, y++);
        set_color(COLOR_WHITE);
        printf("Total Blocks Estimated: ");
        set_color(COLOR_CYAN);
        printf("%d", (cp_student_count + 29) / 30); // 75% load factor of 40

        y += 1;
    }
    else
    {
        gotoxy(center_x - 25, y++);
        set_color(COLOR_YELLOW);
        printf("Note: Please load index first for accurate statistics");
        y += 1;
    }

    gotoxy(center_x - 25, y++);
    set_color(COLOR_YELLOW);
    printf("This file will contain only 1CP and 2CP students with 75%% load factor");

    y += 2;

    gotoxy(center_x - 25, y++);
    set_color(COLOR_BLUE);
    printf("WARNING: Existing STUDENTS_CP.bin will be overwritten!");

    y += 2;

    gotoxy(center_x - 20, y++);
    set_color(COLOR_WHITE);
    printf("Create STUDENTS_CP.bin file? (Y/N): ");

    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (confirm == 'Y' || confirm == 'y')
    {
        // Step 2: Check if index is loaded
        if (index_size == 0)
        {
            CLEAR_SCREEN();
            gotoxy(center_x - 20, 10);
            set_color(COLOR_RED);
            printf("Error: Index table is empty!");

            gotoxy(center_x - 25, 12);
            set_color(COLOR_YELLOW);
            printf("Please load or create index first.");

            gotoxy(center_x - 15, 14);
            printf("Press Enter to continue...");
            getchar();
            return;
        }

        // Clear screen for creation
        CLEAR_SCREEN();
        gotoxy(center_x - 15, 5);
        set_color(COLOR_CYAN);
        printf("CREATING STUDENTS_CP.BIN...");

        gotoxy(center_x - 30, 8);
        set_color(COLOR_CYAN);
        printf("============================================================");

        // Show progress
        gotoxy(center_x - 25, 10);
        set_color(COLOR_WHITE);
        printf("Creating LOF file with 75%% load factor");

        gotoxy(center_x - 25, 12);
        printf("Progress: [");
        // Initial empty progress bar
        for (int i = 0; i < 20; i++)
            printf(".");
        printf("]");

        // Count CP students for progress tracking
        int cp_student_count = 0;
        t_LnOVS *F;
        Open(&F, fname, 'E');
        block buffer;

        for (int i = 0; i < index_size; i++)
        {
            Pr_cor coords = index_pr[i].crdt;
            ReadBlock(F, coords.block_number, &buffer);
            rec student = buffer.tab[coords.offset];

            if (strcmp(student.Year_Study, "1CP") == 0 ||
                strcmp(student.Year_Study, "2CP") == 0)
            {
                cp_student_count++;
            }
        }
        Close(F);

        // Create CP index table
        Pr_index index_cp[MAX_INDEX];
        int index_cp_size = 0;

        // Filter and create CP index
        t_LnOVS *F2;
        Open(&F2, fname, 'E');

        for (int i = 0; i < index_size; i++)
        {
            Pr_cor coords = index_pr[i].crdt;
            ReadBlock(F2, coords.block_number, &buffer);
            rec student = buffer.tab[coords.offset];

            if (strcmp(student.Year_Study, "1CP") == 0 ||
                strcmp(student.Year_Study, "2CP") == 0)
            {
                // Add to CP index
                Insert_index_CP(index_cp, &index_cp_size, student.Student_ID,
                                student, coords.block_number, coords.offset);
            }
        }
        Close(F2);

        // Update progress bar
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        printf("##########"); // 50% progress
        set_color(COLOR_WHITE);

        // Now create the STUDENTS_CP.bin file
        t_LnOVS *student_file;
        Open(&student_file, fname, 'E');

        // Call the create_CP_STUDENT function
        create_CP_STUDENT(index_cp, index_cp_size, student_file);

        Close(student_file);

        // Update progress bar to complete
        gotoxy(center_x - 25 + 21, 12);
        set_color(COLOR_GREEN);
        printf("##########"); // 100% progress
        set_color(COLOR_WHITE);

        // Success message
        gotoxy(center_x - 30, 15);
        set_color(COLOR_CYAN);
        printf("============================================================");

        gotoxy(center_x - 15, 17);
        set_color(COLOR_GREEN);
        printf("STUDENTS_CP.BIN CREATED SUCCESSFULLY!");

        // Show statistics
        gotoxy(center_x - 15, 19);
        set_color(COLOR_WHITE);
        printf("CP Students transferred: ");
        set_color(COLOR_CYAN);
        printf("%d", index_cp_size);

        gotoxy(center_x - 15, 20);
        set_color(COLOR_WHITE);
        printf("Load factor: ");
        set_color(COLOR_CYAN);
        printf("75%%");

        gotoxy(center_x - 15, 21);
        set_color(COLOR_WHITE);
        printf("File created: ");
        set_color(COLOR_CYAN);
        printf("STUDENTS_CP.bin");
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Creation cancelled.");
    }

    // Wait for user
    gotoxy(center_x - 15, y + 6);
    set_color(COLOR_YELLOW);
    printf("Press Enter to continue...");
    getchar();
}

int main()
{
    // Initialize
    srand(time(NULL));
    memset(index_pr, 0, MAX_INDEX * sizeof(Pr_index));
    // Display welcome screen
    display_welcome_screen();

    // Main program loop
    while (1)
    {
        display_main_menu();
    }

    return 0;
}

void display_optional_operations_menu()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // ========== FIRST FRAME: "SELECT AN OPTION" ==========
    int title_frame_y = 7;
    int title_frame_width = 50;
    int title_frame_x = center_x - (title_frame_width / 2);

    // Draw top border of title frame
    gotoxy(title_frame_x, title_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of title frame
    gotoxy(title_frame_x, title_frame_y + 1);
    printf("|");
    gotoxy(title_frame_x + title_frame_width - 1, title_frame_y + 1);
    printf("|");

    // Draw title inside the frame
    gotoxy(center_x - 18, title_frame_y + 1);
    set_color(COLOR_CYAN);
    printf("OPTIONAL OPERATIONS - Choose an Option");

    // Draw bottom border of title frame
    gotoxy(title_frame_x, title_frame_y + 2);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < title_frame_width - 2; i++)
        printf("-");
    printf("+");

    // ========== SECOND FRAME: MENU OPTIONS ==========
    int menu_frame_y = title_frame_y + 4;
    int menu_frame_width = 70;
    int menu_frame_x = center_x - (menu_frame_width / 2);

    // Draw top border of menu frame
    gotoxy(menu_frame_x, menu_frame_y);
    set_color(COLOR_CYAN);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Draw side borders of menu frame
    for (int i = 0; i < 11; i++)
    {
        gotoxy(menu_frame_x, menu_frame_y + 1 + i);
        printf("|");
        gotoxy(menu_frame_x + menu_frame_width - 1, menu_frame_y + 1 + i);
        printf("|");
    }

    // Draw bottom border of menu frame
    gotoxy(menu_frame_x, menu_frame_y + 12);
    printf("+");
    for (int i = 0; i < menu_frame_width - 2; i++)
        printf("-");
    printf("+");

    // Menu options inside the second frame
    int start_y = menu_frame_y + 2; // Start inside the menu frame
    char *submenu_options[] = {
        "1. Display File Header",
        "2. Display Specific Block",
        "3. Display All File Contents",
        "4. Return to Main Menu"};

    int num_options = 4;
    current_submenu_selection = 0;

    while (1)
    {
        // Display all options with ENHANCED highlighting
        for (int i = 0; i < num_options; i++)
        {
            // Calculate position to center within the menu frame
            int option_x = menu_frame_x + 6; // Padding from left border

            // Clear the line first (for highlighting)
            gotoxy(option_x - 2, start_y + i * 2);
            for (int j = 0; j < menu_frame_width - 10; j++)
                printf(" "); // Clear with spaces

            // Now print the option with proper highlighting
            gotoxy(option_x, start_y + i * 2);

            if (i == current_submenu_selection)
            {
                // ENHANCED: Selected option gets highlighted
                if (i == num_options - 1)
                {
                    // Return option when selected - BLUE
                    set_color(COLOR_BLUE);
                    printf("> ");
                    set_color(COLOR_BLUE);
                }
                else
                {
                    // Regular option when selected - YELLOW
                    set_color(COLOR_YELLOW);
                    printf("> ");
                    set_color(COLOR_YELLOW);
                }

                printf("%s", submenu_options[i]);

                // Add padding to fill the frame
                int option_length = strlen(submenu_options[i]);
                int padding_needed = menu_frame_width - 12 - option_length;
                for (int p = 0; p < padding_needed; p++)
                {
                    printf(" ");
                }
            }
            else
            {
                printf("  "); // Regular padding for non-selected

                // Default: All options are WHITE when not selected
                set_color(COLOR_WHITE);

                // Return option is also white when not selected
                if (i == num_options - 1)
                {
                    set_color(COLOR_WHITE);
                }

                printf("%s", submenu_options[i]);
            }
        }

        // Footer - Make it narrower too
        gotoxy(center_x - 30, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < 61; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 29, height - 2);
        set_color(COLOR_WHITE);
        printf("Use ");
        set_color(COLOR_GREEN);
        printf("ARROW KEYS");
        set_color(COLOR_WHITE);
        printf(" to navigate, ");
        set_color(COLOR_GREEN);
        printf("ENTER");
        set_color(COLOR_WHITE);
        printf(" to select, ");
        set_color(COLOR_RED);
        printf("ESC");
        set_color(COLOR_WHITE);
        printf(" to go back");

        // Get key input - Keep your original logic
#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 72 && current_submenu_selection > 0)
            {
                current_submenu_selection--;
            }
            else if (key == 80 && current_submenu_selection < num_options - 1)
            {
                current_submenu_selection++;
            }
        }
        else if (key == 13)
        {
            int selection = current_submenu_selection + 1;

            switch (selection)
            {
            case 1: // Display File Header
            {
                CLEAR_SCREEN();
                display_header_ui("STUDENTS_ESI.bin");
                operation_completed_prompt(0);
                display_optional_operations_menu();
                return;
            }

            case 2: // Display Specific Block
            {
                CLEAR_SCREEN();
                t_LnOVS *F;
                display_block_ui(F, 0, NULL);
                display_optional_operations_menu();
                return;
            }

            case 3: // Display All File Contents
            {
                CLEAR_SCREEN();
                t_LnOVS *F;
                display_all_contents_ui(F, "STUDENTS_ESI.bin");
                display_optional_operations_menu();
                return;
            }

            case 4: // Return to Main Menu
                return;
            }
        }
        else if (key == 27) // ESC key
        {
            return;
        }
#else
        // Linux implementation
        system("stty raw");
        int key = getchar();
        system("stty cooked");

        if (key == 27)
        {
            system("stty raw");
            getchar();
            key = getchar();
            system("stty cooked");

            if (key == 65 && current_submenu_selection > 0)
            {
                current_submenu_selection--;
            }
            else if (key == 66 && current_submenu_selection < num_options - 1)
            {
                current_submenu_selection++;
            }
        }
        else if (key == 10)
        {
            int selection = current_submenu_selection + 1;

            switch (selection)
            {
            case 1:
                CLEAR_SCREEN();
                display_header_ui("STUDENTS_ESI.bin");
                operation_completed_prompt(0);
                display_optional_operations_menu();
                return;

            case 2:
            {
                CLEAR_SCREEN();
                t_LnOVS *F;
                display_block_ui(F, 0, NULL);
                display_optional_operations_menu();
                return;
            }

            case 3:
                CLEAR_SCREEN();
                t_LnOVS *F;
                display_all_contents_ui(F, "STUDENTS_ESI.bin");
                display_optional_operations_menu();
                return;

            case 4: // Return to Main Menu
                return;
            }
        }
        else if (key == 27)
        {
            return;
        }
#endif
    }
}