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

int index_size = 0;
int index_size1 = 0;
int index_size2 = 0;
int index_size3 = 0;
int index_size4 = 0;
int index_size5 = 0;
int index_size6 = 0;
int index_size7 = 0;
int index_size8 = 0;
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

int current_main_menu_selection = 0;
int current_submenu_selection = 0;
// Add this helper function near the top
char* format(const char* format, ...)
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
 * Display TOF_ip index file with organized paging - ASCII version
 */
/**
 * Display TOF_ip index file with organized paging - Enhanced version
 */
/**
 * Display Pr_index table with organized layout - Final corrected version
 */
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

/**
 * Complete workflow for deleting a student
 */
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
        
        // Delete from all secondary indexes
        Delete_from_index(index_pr, &index_size, studentID);
        Delete_from_index1(index_Wilayabirth, &index_size8, coords.block_number, coords.offset);
        Delete_from_index1(index_Speciality, &index_size7, coords.block_number, coords.offset);
        Delete_from_index1(index_YearStudy, &index_size6, coords.block_number, coords.offset);
        Delete_from_index1(index_Gender, &index_size5, coords.block_number, coords.offset);
        Delete_from_index1(index_Monthbirth, &index_size4, coords.block_number, coords.offset);
        Delete_from_index1(index_Yearbirth, &index_size3, coords.block_number, coords.offset);
        Delete_from_index1(index_BooldType, &index_size2, coords.block_number, coords.offset);
        Delete_from_index1(index_Datebirth, &index_size1, coords.block_number, coords.offset);
        
        // Save updated indexes
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
    gotoxy(center_x - 15, y + 5);
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

        // Draw the fixed box frame
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
            // Fixed: array index should be 0-3, not 1-4
            strcpy(buf.tab[buf.Nb].Speciality, specs[rand_year - 1].subspec[rand_spec - 1].name);
        }

        buf.tab[buf.Nb].Resident_UC = Resident_UC;

        // Insert into primary index
        Insert_Pr_index(index_pr, &index_size, Student_ID, current_block, buf.Nb);
        Insert_index(index_Datebirth, &index_size1, day, current_block, buf.Nb);
        Insert_index(index_BooldType, &index_size2, rand_blood, current_block, buf.Nb);
        Insert_index(index_Yearbirth, &index_size3, year, current_block, buf.Nb);
        Insert_index(index_Monthbirth, &index_size4, month, current_block, buf.Nb);
        Insert_index(index_Gender, &index_size5, Gender, current_block, buf.Nb);
        //  Insert_Pr_index(index_Resident, &index_size, Resident_UC, current_block, buf.Nb);
        Insert_index(index_YearStudy, &index_size6, rand_year, current_block, buf.Nb);
        Insert_index(index_Speciality, &index_size7, rand_spec, current_block, buf.Nb);
        Insert_index(index_Wilayabirth, &index_size8, wilya_num, current_block, buf.Nb);

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
    printf("\t\t\tIndex table size: %d\n", index_size);
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
    getchar();
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

    gotoxy(center_x - 30, y);
    char date_str[20];
    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d",
             Date_Birth.day, Date_Birth.month, Date_Birth.year);
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
        // Load index table
       
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

        // Insert the new student
        int C34 = 0;
        insert_newStudent("STUDENTS_ESI.bin", index_pr, &index_size, Gender,
                          Family_Name, First_Name, Wilaya_Birth, Blood_Type,
                          Year_Study, Speciality_name, Date_Birth, Resident_UC, &C34);

        // Save updated index table
        int C31 = 0;
        save_indextable(index_pr, index_size, fname1, &C31);
        int cost;
        save_indextable(index_BooldType, index_size2, fname7, &cost);
        save_indextable(index_Datebirth, index_size1, fname3, &cost);
        save_indextable(index_Yearbirth, index_size3, fname2, &cost );      
        save_indextable(index_Monthbirth, index_size4, fname4, &cost);
        save_indextable(index_Gender, index_size5, fname6, &cost);
        save_indextable(index_YearStudy, index_size6, fname8, &cost);
        save_indextable(index_Speciality, index_size7, fname9, &cost);  
        save_indextable(index_Wilayabirth, index_size8, fname5, &cost);
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

// Main display functions
void display_welcome_screen()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Top border
    set_color(COLOR_CYAN);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // School name
    gotoxy(center_x - 18, 5);
    set_color(COLOR_MAGENTA);
    printf("ECOLE NATIONALE SUPERIEURE D'INFORMATIQUE");

    gotoxy(center_x - 2, 6);
    set_color(COLOR_YELLOW);
    printf("(ESI)");

    // Project title
    gotoxy(center_x - 30, 10);
    set_color(COLOR_GREEN);
    printf("Practical Work - FSDS: LOF Data File Management System");

    // Decorative line
    gotoxy(center_x - 20, 11);
    set_color(COLOR_CYAN);
    for (int i = 0; i < 40; i++)
        printf("~");
    printf("\n");

    // Teacher information
    gotoxy(center_x - 15, 15);
    set_color(COLOR_BLUE);
    printf("Supervised by: ");
    set_color(COLOR_YELLOW);
    printf("Mr. KERMI Adel");

    // Students information
    gotoxy(center_x - 15, 17);
    set_color(COLOR_BLUE);
    printf("Developed by:  ");
    set_color(COLOR_GREEN);
    printf("SIFI Mohamed");

    gotoxy(center_x - 15, 18);
    set_color(COLOR_GREEN);
    printf("               BOUDEHANE Mohamed Adem");

    // Bottom border
    gotoxy(0, height - 5);
    set_color(COLOR_CYAN);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // Continue prompt
    gotoxy(center_x - 15, height - 3);
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

    // Header
    set_color(COLOR_MAGENTA);
    for (int i = 0; i < width; i++)
        printf("*");
    printf("\n");

    gotoxy(center_x - 10, 2);
    set_color(COLOR_CYAN);
    printf("MAIN MENU");

    gotoxy(center_x - 20, 3);
    set_color(COLOR_GRAY);
    printf("LOF Data File Management System");

    set_color(COLOR_MAGENTA);
    gotoxy(0, 4);
    for (int i = 0; i < width; i++)
        printf("*");
    printf("\n");

    // Menu options with numbers
    int start_y = 8;
    char *menu_options[] = {
        "1. File Creation & Initial Loading",
        "2. Index File Operations",
        "3. Student Record Operations",
        "4. Search & Analysis Operations",
        "5. Create LOF File (STUDENTS_CP.BIN)",
        "6. Optional Operations",
        "7. Exit Program"};

    int num_options = 7;

    while (1)
    {
        // Display all options with proper highlighting
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 23, start_y + i * 2);

            if (i == current_main_menu_selection)
            {
                set_color(COLOR_GREEN);
                printf("> ");
            }
            else
            {
                printf("  ");
            }

            // Display option text with appropriate color
            if (i == num_options - 1)
            {
                set_color(COLOR_RED);
            }
            else if (i == 0)
            {
                set_color(COLOR_YELLOW);
            }
            else
            {
                set_color(COLOR_WHITE);
            }

            printf("%s", menu_options[i]);
        }

        // Footer
        gotoxy(0, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < width; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 15, height - 2);
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

        // Get key input
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
                printf("\nIndex File Operations selected...\n");
                printf("\nThis would call your index operations functions.\n");

                t_LnOVS *F;
                Open(&F, "STUDENTS_ESI.bin", 'e');
                DisplayAllContentsTable(F);

                Close(F);
                operation_completed_prompt(1);
                current_main_menu_selection = 0;
                return;
            case 3:
                CLEAR_SCREEN();
                printf("\nStudent Record Operations selected...\n");
                printf("\nThis would call your student operations functions.\n");
                operation_completed_prompt(1);
                current_main_menu_selection = 0;
                return;
            case 4:
                display_search_operations_menu();
                current_main_menu_selection = 0;
                return;
            case 5:
                CLEAR_SCREEN();
                printf("\nCreate LOF File (STUDENTS_CP.BIN) selected...\n");
                printf("\nThis would create STUDENTS_CP.BIN file.\n");
                operation_completed_prompt(1);
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
            case 1:
                display_file_operations_menu();
                current_main_menu_selection = 0;
                return;
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
    set_color(COLOR_GREEN);
    printf("STUDENTS_ESI.bin");
    
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Number of students: ");
    set_color(COLOR_BLUE);
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
    set_color(COLOR_YELLOW);
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
        for (int i = 0; i < 20; i++) printf(".");
        printf("]");
        
        // Perform initial loading
        initial_loading(N, &C2);
        
        // Update progress bar
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        for (int i = 0; i < 20; i++) printf("#");
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
    gotoxy(center_x - 15, y + 5);
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
    set_color(COLOR_BLUE);
    printf("%d records", index_size);
    
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("File: ");
    set_color(COLOR_GREEN);
    printf("StudentID_INDEX.idx");
    
    y += 1;
    
    gotoxy(center_x - 25, y++);
    set_color(COLOR_WHITE);
    printf("Secondary Indexes:");
    
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
    
    y += 2;
    
    gotoxy(center_x - 25, y++);
    set_color(COLOR_YELLOW);
    printf("WARNING: Existing index files will be overwritten!");
    
    y += 2;
    
    gotoxy(center_x - 20, y++);
    set_color(COLOR_WHITE);
    printf("Save all 9 index files? (Y/N): ");
    
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
        printf("Saving 9 index files to disk");
        
        gotoxy(center_x - 25, 12);
        printf("Progress: [");
        // Initial empty progress bar
        for (int i = 0; i < 20; i++) printf(".");
        printf("]");
        
        // Save all indexes
        int cost;
        save_indextable(index_pr, index_size, fname1, &C31);
        
        // Update progress bar in steps
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        printf("####"); // 4/9 files
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
        printf("9 files");
        
        // Show file list
        gotoxy(center_x - 25, 22);
        set_color(COLOR_WHITE);
        printf("Saved files:");
        gotoxy(center_x - 23, 23);
        set_color(COLOR_GRAY);
        printf("StudentID_INDEX.idx, DateBirth_INDEX.idx, ...");
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Save cancelled.");
    }
    
    // Wait for user
    gotoxy(center_x - 15, y + 5);
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
    printf("Files to load:");
    
    gotoxy(center_x - 22, y++);
    set_color(COLOR_GREEN);
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
    
    y += 2;
    
    gotoxy(center_x - 25, y++);
    set_color(COLOR_YELLOW);
    printf("WARNING: Current index data will be replaced!");
    
    y += 2;
    
    gotoxy(center_x - 20, y++);
    set_color(COLOR_WHITE);
    printf("Load all 9 index files? (Y/N): ");
    
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
        printf("Loading 9 index files from disk");
        
        gotoxy(center_x - 25, 12);
        printf("Progress: [");
        // Initial empty progress bar
        for (int i = 0; i < 20; i++) printf(".");
        printf("]");
        
        // Load all indexes
        int cost1;
        loadindextable(fname1, index_pr, &index_size, &C32);
        
        // Update progress bar in steps
        gotoxy(center_x - 25 + 11, 12); // Position after "Progress: ["
        set_color(COLOR_GREEN);
        printf("####"); // 4/9 files
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
        
        if (index_size1 > 0)
        {
            gotoxy(center_x - 15, 21);
            set_color(COLOR_WHITE);
            printf("Secondary indexes: ");
            set_color(COLOR_CYAN);
            printf("8 files loaded");
        }
    }
    else
    {
        gotoxy(center_x - 10, y + 2);
        set_color(COLOR_RED);
        printf("Load cancelled.");
    }
    
    // Wait for user
    gotoxy(center_x - 15, y + 5);
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

    // Header
    set_color(COLOR_GREEN);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    gotoxy(center_x - 15, 2);
    set_color(COLOR_CYAN);
    printf("FILE OPERATIONS MENU");

    set_color(COLOR_GREEN);
    gotoxy(0, 3);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // Submenu options
    int start_y = 8;
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
            gotoxy(center_x - 25, start_y + i * 2);

            if (i == current_submenu_selection)
            {
                set_color(COLOR_GREEN);
                printf("> ");
            }
            else
            {
                printf("  ");
            }

            if (i == num_options - 1)
            {
                set_color(COLOR_BLUE);
            }
            else if (i == 0)
            {
                set_color(COLOR_YELLOW);
            }
            else
            {
                set_color(COLOR_WHITE);
            }
            printf("%s", submenu_options[i]);
        }

        // Footer
        gotoxy(0, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < width; i++)
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

    // Header
    set_color(COLOR_BLUE);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    gotoxy(center_x - 15, 2);
    set_color(COLOR_CYAN);
    printf("SEARCH & ANALYSIS MENU");

    set_color(COLOR_BLUE);
    gotoxy(0, 3);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // Submenu options
    int start_y = 8;
    char *submenu_options[] = {
        "1. Search student by ID",
        "2. Insert new student",
        "3. display table ta3 index ",
        "4. Delete Student ",
        "5. modification of first name of student",
        "6. Return to Main Menu"};

    int num_options = 6;
    current_submenu_selection = 0;

    while (1)
    {
        // Display options with highlighting
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 25, start_y + i * 2);

            if (i == current_submenu_selection)
            {
                set_color(COLOR_GREEN);
                printf("> ");
            }
            else
            {
                printf("  ");
            }

            if (i == num_options - 1)
            {
                set_color(COLOR_BLUE);
            }
            else if (i == 0)
            {
                set_color(COLOR_YELLOW);
            }
            else
            {
                set_color(COLOR_WHITE);
            }
            printf("%s", submenu_options[i]);
        }

        // Footer
        gotoxy(0, height - 3);
        set_color(COLOR_GRAY);
        for (int i = 0; i < width; i++)
            printf("-");
        printf("\n");

        gotoxy(center_x - 10, height - 2);
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
                    printf("\n The User Information found in block number %d, and offset number %d", num_block, offset);
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
                //    displayPrIndexTableWithData(index_pr,index_size,fname,10);
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

void display_optional_operations_menu()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    // Header
    set_color(COLOR_MAGENTA);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    gotoxy(center_x - 12, 2);
    set_color(COLOR_CYAN);
    printf("OPTIONAL OPERATIONS");

    set_color(COLOR_MAGENTA);
    gotoxy(0, 3);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // Submenu options
    int start_y = 8;
    char *submenu_options[] = {
        "1. Display file header",
        "2. Display block contents",
        "3. Display entire file block by block",
        "4. Return to Main Menu"};

    int num_options = 4;
    current_submenu_selection = 0;

    while (1)
    {
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 23, start_y + i * 2);
            if (i == current_submenu_selection)
            {
                set_color(COLOR_GREEN);
                printf("> ");
            }
            else
            {
                printf("  ");
            }

            if (i == num_options - 1)
            {
                set_color(COLOR_BLUE);
            }
            else
            {
                set_color(COLOR_YELLOW);
            }
            printf("%s", submenu_options[i]);
        }

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
                printf("\n=== DISPLAY FILE HEADER ===\n\n");

                printf("Displaying file header...\n");
                printf("Header information would be displayed here.\n");

                operation_completed_prompt(0);
                display_optional_operations_menu();
                return;
            }
            case 2:
            {
                CLEAR_SCREEN();
                printf("\n=== DISPLAY BLOCK CONTENTS ===\n\n");

                int block_num;
                printf("Enter block number to display: ");
                scanf("%d", &block_num);
                getchar();

                printf("Displaying contents of block %d...\n", block_num);

                operation_completed_prompt(0);
                display_optional_operations_menu();
                return;
            }
            case 3:
            {
                CLEAR_SCREEN();
                printf("\n=== DISPLAY ENTIRE FILE ===\n\n");

                printf("Displaying entire file block by block...\n");

                operation_completed_prompt(0);
                display_optional_operations_menu();
                return;
            }
            case 4:
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

void display_exit_confirmation()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;
    int center_y = height / 2;

    // Draw confirmation box
    set_color(COLOR_RED);

    // Top border
    gotoxy(center_x - 20, center_y - 5);
    printf("+--------------------------------------+");

    // Empty lines
    gotoxy(center_x - 20, center_y - 4);
    printf("|                                      |");
    gotoxy(center_x - 20, center_y - 3);
    printf("|                                      |");
    gotoxy(center_x - 20, center_y - 2);
    printf("|                                      |");
    gotoxy(center_x - 20, center_y - 1);
    printf("|                                      |");
    gotoxy(center_x - 20, center_y);
    printf("|                                      |");
    gotoxy(center_x - 20, center_y + 1);
    printf("|                                      |");
    gotoxy(center_x - 20, center_y + 2);
    printf("+--------------------------------------+");

    // Title
    gotoxy(center_x - 6, center_y - 3);
    set_color(COLOR_YELLOW);
    printf("EXIT CONFIRMATION");

    // Message
    gotoxy(center_x - 16, center_y - 1);
    set_color(COLOR_WHITE);
    printf("Are you sure you want to exit?");

    // Selection logic
    int selection = 0;
    int selected = 0;

    while (!selected)
    {
        if (selection == 0)
        {
            gotoxy(center_x - 8, center_y + 1);
            set_color(COLOR_GREEN);
            printf("[ YES ]");

            gotoxy(center_x + 4, center_y + 1);
            set_color(COLOR_WHITE);
            printf("[ NO ]");
        }
        else
        {
            gotoxy(center_x - 8, center_y + 1);
            set_color(COLOR_WHITE);
            printf("[ YES ]");

            gotoxy(center_x + 4, center_y + 1);
            set_color(COLOR_RED);
            printf("[ NO ]");
        }

#ifdef _WIN32
        int key = _getch();
        if (key == 224 || key == 0)
        {
            key = _getch();
            if (key == 75 || key == 77)
            {
                selection = !selection;
            }
        }
        else if (key == 13)
        {
            selected = 1;
            if (selection == 0)
            {
                CLEAR_SCREEN();
                gotoxy(center_x - 15, center_y);
                set_color(COLOR_GREEN);
                printf("Thank you for using ESI File Manager!");
                set_color(COLOR_RESET);
                SLEEP_MS(2000);
                exit(0);
            }
            else
            {
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

    gotoxy(center_x - 15, height - 8);
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
    set_color(COLOR_RED);
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