#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "mainLibrary.h"
int get_terminal_width();
int get_terminal_height();
void gotoxy(int x, int y);
void set_color(int color);
void hide_cursor();
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
#define COLOR_GRAY 8  // Add this line

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
#define ANSI_GRAY "\033[90m"  // Add this lin
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
void displayTOFIndexFile(char *filename, int recordsPerPage)
{
    TOF_ip *F;
    int currentPage = 1;
    int totalRecords = 0;
    int totalBlocks = 0;
    int cursorLine = 1;
    
    // Open the TOF_ip file
    Open_TOF(&F, filename, 'E');
    
    // Get total blocks
    totalBlocks = getHeader_ip(F, "nblk");
    
    // Count total records
    block_ip buf;
    for (int i = 1; i <= totalBlocks; i++)
    {
        ReadBlock_ip(F, i, &buf);
        totalRecords += buf.Nb;
    }
    
    // Calculate total pages
    int totalPages = (totalRecords + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0) totalPages = 1;
    
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;
    
    int continueDisplay = 1;
    
    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();
        
        // ========== TOP SECTION: HEADER AND STATS ==========
        // Top border
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++) printf("=");
        printf("\n");
        
        // Title
        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("TOF INDEX FILE VIEWER");
        
        // File info box
        gotoxy(center_x - 25, 4);
        set_color(COLOR_YELLOW);
        printf("+----------------------------------------------------+\n");
        
        gotoxy(center_x - 25, 5);
        printf("|  File: %-40s  |\n", filename);
        
        gotoxy(center_x - 25, 6);
        printf("+----------------------------------------------------+\n");
        
        // Statistics
        gotoxy(center_x - 25, 7);
        set_color(COLOR_CYAN);
        printf("|  Blocks: %-4d | Records: %-6d | Page: %-3d/%-3d  |\n",
               totalBlocks, totalRecords, currentPage, totalPages);
        
        gotoxy(center_x - 25, 8);
        printf("+----------------------------------------------------+\n");
        
        // ========== MIDDLE SECTION: DATA TABLE ==========
        // Calculate start and end positions for current page
        int recordsDisplayed = 0;
        int currentRecord = 0;
        int startRecord = (currentPage - 1) * recordsPerPage;
        int endRecord = startRecord + recordsPerPage;
        
        // Table header
        int tableWidth = 68;
        int tableStartX = center_x - (tableWidth / 2);
        
        gotoxy(tableStartX, 10);
        set_color(COLOR_GREEN);
        printf("+------+------------+--------------+----------------+------------------+\n");
        
        gotoxy(tableStartX, 11);
        set_color(COLOR_YELLOW);
        printf("|  #   |    ID      |    Block     |    Offset      |  File Position   |\n");
        
        gotoxy(tableStartX, 12);
        set_color(COLOR_GREEN);
        printf("+------+------------+--------------+----------------+------------------+\n");
        
        // Display records
        int lineY = 13;
        for (int blockNum = 1; blockNum <= totalBlocks && recordsDisplayed < recordsPerPage; blockNum++)
        {
            ReadBlock_ip(F, blockNum, &buf);
            
            for (int offset = 0; offset < buf.Nb && recordsDisplayed < recordsPerPage; offset++)
            {
                currentRecord++;
                
                if (currentRecord > startRecord && currentRecord <= endRecord)
                {
                    int isCursorLine = (cursorLine == currentRecord);
                    long filePosition = sizeof(header_ip) + (blockNum - 1) * sizeof(block_ip) + offset * sizeof(rec_ip);
                    
                    gotoxy(tableStartX, lineY);
                    
                    if (isCursorLine)
                    {
                        set_color(COLOR_GREEN);
                        printf("| >%-4d", currentRecord);
                        set_color(COLOR_WHITE);
                        printf("| %-10d | %-12d | %-14d | %-16ld |\n",
                               buf.tab[offset].Identifier,
                               buf.tab[offset].crdt.block_number,
                               buf.tab[offset].crdt.offset,
                               filePosition);
                    }
                    else
                    {
                        printf("|  %-4d| %-10d | %-12d | %-14d | %-16ld |\n",
                               currentRecord,
                               buf.tab[offset].Identifier,
                               buf.tab[offset].crdt.block_number,
                               buf.tab[offset].crdt.offset,
                               filePosition);
                    }
                    
                    lineY++;
                    recordsDisplayed++;
                }
            }
        }
        
        // Table footer
        gotoxy(tableStartX, lineY);
        set_color(COLOR_GREEN);
        printf("+------+------------+--------------+----------------+------------------+\n");
        
        // Cursor position info
        if (cursorLine > 0 && cursorLine <= totalRecords)
        {
            gotoxy(center_x - 20, lineY + 2);
            set_color(COLOR_CYAN);
            printf("Current Cursor: Record %d of %d", cursorLine, totalRecords);
        }
        
        // ========== BOTTOM SECTION: NAVIGATION ==========
        int navStartY = lineY + 4;
        
        // Clear navigation area
        for (int i = navStartY; i < height - 1; i++)
        {
            gotoxy(0, i);
            printf("%-*s", width, "");
        }
        
        // Navigation box
        gotoxy(center_x - 30, navStartY);
        set_color(COLOR_BLUE);
        printf("+--------------------------------------------------------------+\n");
        
        gotoxy(center_x - 30, navStartY + 1);
        printf("|                     NAVIGATION MENU                          |\n");
        
        gotoxy(center_x - 30, navStartY + 2);
        printf("+--------------------------------------------------------------+\n");
        
        // Navigation options with cursor selection
        char *navOptions[] = {
            " Next Page        ",
            " Previous Page    ",
            " Go to Page       ",
            " Set Cursor Line  ",
            " Move Cursor Up   ",
            " Move Cursor Down ",
            " View Details     ",
            " Quit             "
        };
        
        int navCursor = 0;
        int navSelected = -1;
        
        while (navSelected == -1)
        {
            for (int i = 0; i < 8; i++)
            {
                gotoxy(center_x - 30, navStartY + 3 + i);
                
                if (i == navCursor)
                {
                    set_color(COLOR_GREEN);
                    printf("| > %-56s |\n", navOptions[i]);
                }
                else
                {
                    set_color(COLOR_WHITE);
                    printf("|   %-56s |\n", navOptions[i]);
                }
            }
            
            gotoxy(center_x - 30, navStartY + 11);
            set_color(COLOR_BLUE);
            printf("+--------------------------------------------------------------+\n");
            
            gotoxy(center_x - 25, navStartY + 12);
            set_color(COLOR_YELLOW);
            printf("Use ARROW KEYS to navigate, ENTER to select");
            
            // Get navigation input
            show_cursor();
            #ifdef _WIN32
            int key = _getch();
            if (key == 224 || key == 0)
            {
                key = _getch();
                if (key == 72 && navCursor > 0) // Up
                {
                    navCursor--;
                }
                else if (key == 80 && navCursor < 7) // Down
                {
                    navCursor++;
                }
            }
            else if (key == 13) // Enter
            {
                navSelected = navCursor;
            }
            else if (key == 27) // ESC
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
                getchar(); // Skip [
                key = getchar();
                system("stty cooked");
                
                if (key == 65 && navCursor > 0) // Up
                {
                    navCursor--;
                }
                else if (key == 66 && navCursor < 7) // Down
                {
                    navCursor++;
                }
            }
            else if (key == 10) // Enter
            {
                navSelected = navCursor;
            }
            #endif
            hide_cursor();
        }
        
        // Process selected navigation option
        if (navSelected != -1)
        {
            switch (navSelected)
            {
            case 0: // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorLine = (currentPage - 1) * recordsPerPage + 1;
                }
                break;
                
            case 1: // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorLine = (currentPage - 1) * recordsPerPage + 1;
                }
                break;
                
            case 2: // Go to Page
                {
                    // Show input prompt
                    gotoxy(center_x - 15, navStartY + 14);
                    set_color(COLOR_YELLOW);
                    printf("Enter page (1-%d): ", totalPages);
                    show_cursor();
                    int newPage;
                    scanf("%d", &newPage);
                    if (newPage >= 1 && newPage <= totalPages)
                    {
                        currentPage = newPage;
                        cursorLine = (currentPage - 1) * recordsPerPage + 1;
                    }
                }
                break;
                
            case 3: // Set Cursor Line
                {
                    gotoxy(center_x - 20, navStartY + 14);
                    set_color(COLOR_YELLOW);
                    printf("Enter line (1-%d): ", totalRecords);
                    show_cursor();
                    int newCursor;
                    scanf("%d", &newCursor);
                    if (newCursor >= 1 && newCursor <= totalRecords)
                    {
                        cursorLine = newCursor;
                        currentPage = (cursorLine - 1) / recordsPerPage + 1;
                    }
                }
                break;
                
            case 4: // Move Cursor Up
                if (cursorLine > 1)
                {
                    cursorLine--;
                    currentPage = (cursorLine - 1) / recordsPerPage + 1;
                }
                break;
                
            case 5: // Move Cursor Down
                if (cursorLine < totalRecords)
                {
                    cursorLine++;
                    currentPage = (cursorLine - 1) / recordsPerPage + 1;
                }
                break;
                
            case 6: // View Details
                if (cursorLine > 0 && cursorLine <= totalRecords)
                {
                    // Find the record
                    int tempRecord = 0;
                    int foundBlock = 0, foundOffset = 0;
                    rec_ip foundRec;
                    
                    for (int blockNum = 1; blockNum <= totalBlocks && !foundBlock; blockNum++)
                    {
                        ReadBlock_ip(F, blockNum, &buf);
                        for (int offset = 0; offset < buf.Nb && !foundBlock; offset++)
                        {
                            tempRecord++;
                            if (tempRecord == cursorLine)
                            {
                                foundBlock = blockNum;
                                foundOffset = offset;
                                foundRec = buf.tab[offset];
                                break;
                            }
                        }
                    }
                    
                    if (foundBlock)
                    {
                        CLEAR_SCREEN();
                        gotoxy(center_x - 20, 5);
                        set_color(COLOR_MAGENTA);
                        printf("INDEX ENTRY DETAILS\n");
                        
                        gotoxy(center_x - 25, 7);
                        set_color(COLOR_CYAN);
                        printf("+----------------------------------------------------+\n");
                        
                        gotoxy(center_x - 25, 8);
                        printf("|  Record: %-42d |\n", cursorLine);
                        
                        gotoxy(center_x - 25, 9);
                        printf("|  ID: %-45d |\n", foundRec.Identifier);
                        
                        gotoxy(center_x - 25, 10);
                        printf("|  Block: %-43d |\n", foundRec.crdt.block_number);
                        
                        gotoxy(center_x - 25, 11);
                        printf("|  Offset: %-42d |\n", foundRec.crdt.offset);
                        
                        gotoxy(center_x - 25, 12);
                        printf("+----------------------------------------------------+\n");
                        
                        gotoxy(center_x - 15, 15);
                        set_color(COLOR_YELLOW);
                        printf("Press Enter to continue...");
                        getchar();
                        getchar();
                    }
                }
                break;
                
            case 7: // Quit
                continueDisplay = 0;
                break;
            }
        }
    }
    
    // Close file
    Close_TOF(F);
    
    // Clear and show exit message
    CLEAR_SCREEN();
    gotoxy(center_x - 20, height / 2);
    set_color(COLOR_GREEN);
    printf("Index File Display Completed!\n");
    SLEEP_MS(1000);
}

/**
 * Display Pr_index table with enhanced UI and cursor control
 */
void displayPrIndexTableWithData(Pr_index *indexTable, int tableSize,
                                 char *dataFilename, int recordsPerPage)
{
    int currentPage = 1;
    int cursorIndex = 0;
    int totalPages = (tableSize + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0) totalPages = 1;
    
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
        
        // ========== TOP SECTION: HEADER AND STATS ==========
        // Top border
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++) printf("=");
        printf("\n");
        
        // Title
        gotoxy(center_x - 20, 2);
        set_color(COLOR_MAGENTA);
        printf("PRIMARY INDEX TABLE WITH RECORD DATA");
        
        // Stats box
        gotoxy(center_x - 30, 4);
        set_color(COLOR_YELLOW);
        printf("+--------------------------------------------------------------+\n");
        
        gotoxy(center_x - 30, 5);
        printf("|                         STATISTICS                           |\n");
        
        gotoxy(center_x - 30, 6);
        printf("+--------------------------------------------------------------+\n");
        
        gotoxy(center_x - 30, 7);
        set_color(COLOR_CYAN);
        printf("|  Records: %-6d | Page: %-3d/%-3d | Cursor: %-4d                |\n",
               tableSize, currentPage, totalPages, cursorIndex + 1);
        
        gotoxy(center_x - 30, 8);
        printf("|  Data File: %-52s |\n", dataFilename ? dataFilename : "None");
        
        gotoxy(center_x - 30, 9);
        printf("+--------------------------------------------------------------+\n");
        
        // ========== MIDDLE SECTION: CURSOR RECORD DETAILS ==========
        int detailStartY = 11;
        
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
                    gotoxy(center_x - 35, detailStartY);
                    set_color(COLOR_GREEN);
                    printf("+----------------------------------------------------------------------+\n");
                    
                    gotoxy(center_x - 35, detailStartY + 1);
                    printf("|                    CURSOR RECORD DETAILS                           |\n");
                    
                    gotoxy(center_x - 35, detailStartY + 2);
                    printf("+----------------------------------------------------------------------+\n");
                    
                    gotoxy(center_x - 35, detailStartY + 3);
                    printf("|  ID: %-5d  Name: %s %-30s |\n", 
                           studentRecord.Student_ID,
                           studentRecord.Family_Name,
                           studentRecord.First_Name);
                    
                    gotoxy(center_x - 35, detailStartY + 4);
                    printf("|  Birth: %02d/%02d/%04d  Gender: %-6s  Wilaya: %-15s |\n", 
                           studentRecord.Date_Birth.day,
                           studentRecord.Date_Birth.month,
                           studentRecord.Date_Birth.year,
                           studentRecord.Gender == 1 ? "Male" : "Female",
                           studentRecord.Wilaya_Birth);
                    
                    gotoxy(center_x - 35, detailStartY + 5);
                    printf("|  Year: %-5s  Speciality: %-35s |\n", 
                           studentRecord.Year_Study, studentRecord.Speciality);
                    
                    gotoxy(center_x - 35, detailStartY + 6);
                    printf("|  Blood: %-6s  Resident: %-3s  Block: %-3d  Offset: %-3d    |\n",
                           studentRecord.Blood_Type,
                           studentRecord.Resident_UC ? "Yes" : "No",
                           coords.block_number, coords.offset);
                    
                    gotoxy(center_x - 35, detailStartY + 7);
                    printf("+----------------------------------------------------------------------+\n");
                    
                    detailStartY += 9; // Move down for table
                }
            }
        }
        else
        {
            detailStartY = 11;
        }
        
        // ========== MIDDLE SECTION: DATA TABLE ==========
        int startIndex = (currentPage - 1) * recordsPerPage;
        int endIndex = startIndex + recordsPerPage;
        if (endIndex > tableSize) endIndex = tableSize;
        
        // Table header
        int tableWidth = 60;
        int tableStartX = center_x - (tableWidth / 2);
        
        gotoxy(tableStartX, detailStartY);
        set_color(COLOR_MAGENTA);
        printf("+----+------------+----------+----------+--------------+--------------------+\n");
        
        gotoxy(tableStartX, detailStartY + 1);
        set_color(COLOR_YELLOW);
        printf("| #  |    ID      |  Block   |  Offset  |    Status    |   Record Preview   |\n");
        
        gotoxy(tableStartX, detailStartY + 2);
        set_color(COLOR_MAGENTA);
        printf("+----+------------+----------+----------+--------------+--------------------+\n");
        
        // Display records
        int lineY = detailStartY + 3;
        for (int i = startIndex; i < endIndex; i++)
        {
            gotoxy(tableStartX, lineY);
            
            int isCursorIndex = (i == cursorIndex);
            char status[20] = "Unknown";
            char namePreview[20] = "N/A";
            
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
                    }
                    else
                    {
                        strcpy(status, "Valid");
                        
                        // Create name preview
                        if (strlen(studentRecord.Family_Name) > 0)
                        {
                            strncpy(namePreview, studentRecord.Family_Name, 6);
                            namePreview[6] = '\0';
                            strcat(namePreview, ".");
                            if (strlen(studentRecord.First_Name) > 0)
                            {
                                strncat(namePreview, studentRecord.First_Name, 1);
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
                if (strcmp(status, "Valid") == 0) set_color(COLOR_GREEN);
                else if (strcmp(status, "Deleted") == 0) set_color(COLOR_RED);
                else if (strcmp(status, "ID Mismatch") == 0) set_color(COLOR_YELLOW);
                else set_color(COLOR_WHITE);
                
                printf("%-12s | %-18s |\n", status, namePreview);
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
                if (strcmp(status, "Valid") == 0) set_color(COLOR_GREEN);
                else if (strcmp(status, "Deleted") == 0) set_color(COLOR_RED);
                else if (strcmp(status, "ID Mismatch") == 0) set_color(COLOR_YELLOW);
                else set_color(COLOR_GRAY);
                
                printf("%-12s | ", status);
                set_color(COLOR_WHITE);
                printf("%-18s |\n", namePreview);
            }
            
            lineY++;
        }
        
        // Table footer
        gotoxy(tableStartX, lineY);
        set_color(COLOR_MAGENTA);
        printf("+----+------------+----------+----------+--------------+--------------------+\n");
        
        // Page info
        if (tableSize > 0)
        {
            gotoxy(center_x - 20, lineY + 2);
            set_color(COLOR_CYAN);
            printf("Page %d/%d: Records %d to %d of %d",
                   currentPage, totalPages, startIndex + 1, endIndex, tableSize);
        }
        
        // ========== BOTTOM SECTION: NAVIGATION ==========
        int navStartY = lineY + 4;
        if (navStartY > height - 10) navStartY = height - 10;
        
        // Clear navigation area
        for (int i = navStartY; i < height - 1; i++)
        {
            gotoxy(0, i);
            printf("%-*s", width, "");
        }
        
        // Navigation box
        gotoxy(center_x - 30, navStartY);
        set_color(COLOR_BLUE);
        printf("+--------------------------------------------------------------+\n");
        
        gotoxy(center_x - 30, navStartY + 1);
        printf("|                    NAVIGATION CONTROLS                       |\n");
        
        gotoxy(center_x - 30, navStartY + 2);
        printf("+--------------------------------------------------------------+\n");
        
        // Navigation options
        char *navOptions[] = {
            " Next Page        ",
            " Previous Page    ",
            " Go to Page       ",
            " Search by ID     ",
            " Set Cursor       ",
            " Move Cursor Up   ",
            " Move Cursor Down ",
            " Quit             "
        };
        
        int navCursor = 0;
        int navSelected = -1;
        
        while (navSelected == -1)
        {
            for (int i = 0; i < 8; i++)
            {
                gotoxy(center_x - 30, navStartY + 3 + i);
                
                if (i == navCursor)
                {
                    set_color(COLOR_GREEN);
                    printf("| > %-56s |\n", navOptions[i]);
                }
                else
                {
                    set_color(COLOR_WHITE);
                    printf("|   %-56s |\n", navOptions[i]);
                }
            }
            
            gotoxy(center_x - 30, navStartY + 11);
            set_color(COLOR_BLUE);
            printf("+--------------------------------------------------------------+\n");
            
            gotoxy(center_x - 25, navStartY + 12);
            set_color(COLOR_YELLOW);
            printf("Use ARROW KEYS to navigate, ENTER to select");
            
            // Get navigation input
            show_cursor();
            #ifdef _WIN32
            int key = _getch();
            if (key == 224 || key == 0)
            {
                key = _getch();
                if (key == 72 && navCursor > 0) // Up
                {
                    navCursor--;
                }
                else if (key == 80 && navCursor < 7) // Down
                {
                    navCursor++;
                }
            }
            else if (key == 13) // Enter
            {
                navSelected = navCursor;
            }
            else if (key == 27) // ESC
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
                getchar(); // Skip [
                key = getchar();
                system("stty cooked");
                
                if (key == 65 && navCursor > 0) // Up
                {
                    navCursor--;
                }
                else if (key == 66 && navCursor < 7) // Down
                {
                    navCursor++;
                }
            }
            else if (key == 10) // Enter
            {
                navSelected = navCursor;
            }
            #endif
            hide_cursor();
        }
        
        // Process selected option
        if (navSelected != -1)
        {
            switch (navSelected)
            {
            case 0: // Next Page
                if (currentPage < totalPages)
                {
                    currentPage++;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                break;
                
            case 1: // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                break;
                
            case 2: // Go to Page
                {
                    gotoxy(center_x - 15, navStartY + 14);
                    set_color(COLOR_YELLOW);
                    printf("Enter page (1-%d): ", totalPages);
                    show_cursor();
                    int newPage;
                    scanf("%d", &newPage);
                    if (newPage >= 1 && newPage <= totalPages)
                    {
                        currentPage = newPage;
                        cursorIndex = (currentPage - 1) * recordsPerPage;
                    }
                }
                break;
                
            case 3: // Search by ID
                {
                    gotoxy(center_x - 20, navStartY + 14);
                    set_color(COLOR_YELLOW);
                    printf("Enter Student ID: ");
                    show_cursor();
                    int searchId;
                    scanf("%d", &searchId);
                    
                    bool found;
                    int position;
                    binary_search(indexTable, tableSize, searchId, &found, &position);
                    
                    if (found)
                    {
                        cursorIndex = position;
                        currentPage = (position / recordsPerPage) + 1;
                    }
                    else
                    {
                        gotoxy(center_x - 20, navStartY + 15);
                        set_color(COLOR_RED);
                        printf("ID %d not found!", searchId);
                        SLEEP_MS(1500);
                    }
                }
                break;
                
            case 4: // Set Cursor
                {
                    gotoxy(center_x - 20, navStartY + 14);
                    set_color(COLOR_YELLOW);
                    printf("Enter index (1-%d): ", tableSize);
                    show_cursor();
                    int newCursor;
                    scanf("%d", &newCursor);
                    if (newCursor >= 1 && newCursor <= tableSize)
                    {
                        cursorIndex = newCursor - 1;
                        currentPage = (cursorIndex / recordsPerPage) + 1;
                    }
                }
                break;
                
            case 5: // Move Cursor Up
                if (cursorIndex > 0)
                {
                    cursorIndex--;
                    if (cursorIndex < startIndex) currentPage--;
                }
                break;
                
            case 6: // Move Cursor Down
                if (cursorIndex < tableSize - 1)
                {
                    cursorIndex++;
                    if (cursorIndex >= endIndex) currentPage++;
                }
                break;
                
            case 7: // Quit
                continueDisplay = 0;
                break;
            }
        }
    }
    
    // Close the data file
    if (dataFile != NULL) Close(dataFile);
    
    // Clear and show exit message
    CLEAR_SCREEN();
    gotoxy(center_x - 20, height / 2);
    set_color(COLOR_GREEN);
    printf("Index Table Display Completed!\n");
    SLEEP_MS(1000);
}

/**
 * Display Pr_index table in a simple format with cursor control
 */
void display_pr_index(Pr_index *index, int index_size)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;
    
    int currentPage = 1;
    int cursorIndex = 0;
    int recordsPerPage = 20;
    int totalPages = (index_size + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0) totalPages = 1;
    
    int continueDisplay = 1;
    
    while (continueDisplay)
    {
        CLEAR_SCREEN();
        hide_cursor();
        
        // ========== HEADER ==========
        set_color(COLOR_CYAN);
        for (int i = 0; i < width; i++) printf("=");
        printf("\n");
        
        gotoxy(center_x - 15, 2);
        set_color(COLOR_MAGENTA);
        printf("PRIMARY INDEX TABLE DUMP");
        
        gotoxy(center_x - 25, 4);
        set_color(COLOR_YELLOW);
        printf("+----------------------------------------------------+\n");
        
        gotoxy(center_x - 25, 5);
        set_color(COLOR_CYAN);
        printf("|  Total Records: %-6d | Page: %-3d/%-3d              |\n",
               index_size, currentPage, totalPages);
        
        gotoxy(center_x - 25, 6);
        printf("|  Current Cursor: Index %-4d                        |\n", cursorIndex + 1);
        
        gotoxy(center_x - 25, 7);
        printf("+----------------------------------------------------+\n");
        
        // ========== TABLE ==========
        int startIndex = (currentPage - 1) * recordsPerPage;
        int endIndex = startIndex + recordsPerPage;
        if (endIndex > index_size) endIndex = index_size;
        
        int tableWidth = 52;
        int tableStartX = center_x - (tableWidth / 2);
        
        gotoxy(tableStartX, 9);
        set_color(COLOR_GREEN);
        printf("+------+------------+-----------------+-----------------+\n");
        
        gotoxy(tableStartX, 10);
        set_color(COLOR_YELLOW);
        printf("| Index | Student ID |   Block Number  |     Offset      |\n");
        
        gotoxy(tableStartX, 11);
        set_color(COLOR_GREEN);
        printf("+------+------------+-----------------+-----------------+\n");
        
        int lineY = 12;
        for (int i = startIndex; i < endIndex; i++)
        {
            gotoxy(tableStartX, lineY);
            
            if (i == cursorIndex)
            {
                set_color(COLOR_GREEN);
                printf("| >%-4d", i + 1);
                set_color(COLOR_WHITE);
                printf("| %-10d | %-15d | %-15d |\n",
                       index[i].Identifier,
                       index[i].crdt.block_number,
                       index[i].crdt.offset);
            }
            else
            {
                set_color(COLOR_WHITE);
                printf("|  %-4d| %-10d | %-15d | %-15d |\n",
                       i + 1,
                       index[i].Identifier,
                       index[i].crdt.block_number,
                       index[i].crdt.offset);
            }
            lineY++;
        }
        
        gotoxy(tableStartX, lineY);
        set_color(COLOR_GREEN);
        printf("+------+------------+-----------------+-----------------+\n");
        
        // ========== NAVIGATION ==========
        int navStartY = lineY + 2;
        
        // Clear navigation area
        for (int i = navStartY; i < height - 1; i++)
        {
            gotoxy(0, i);
            printf("%-*s", width, "");
        }
        
        // Navigation box
        gotoxy(center_x - 25, navStartY);
        set_color(COLOR_BLUE);
        printf("+----------------------------------------------------+\n");
        
        gotoxy(center_x - 25, navStartY + 1);
        printf("|                  NAVIGATION MENU                   |\n");
        
        gotoxy(center_x - 25, navStartY + 2);
        printf("+----------------------------------------------------+\n");
        
        // Simple navigation options
        char *navOptions[] = {
            " Next Page        ",
            " Previous Page    ",
            " Go to Page       ",
            " Set Cursor       ",
            " Move Cursor      ",
            " Quit             "
        };
        
        int navCursor = 0;
        int navSelected = -1;
        
        while (navSelected == -1)
        {
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
            set_color(COLOR_BLUE);
            printf("+----------------------------------------------------+\n");
            
            gotoxy(center_x - 20, navStartY + 10);
            set_color(COLOR_YELLOW);
            printf("Use ARROW KEYS and ENTER to select");
            
            // Get input
            show_cursor();
            #ifdef _WIN32
            int key = _getch();
            if (key == 224 || key == 0)
            {
                key = _getch();
                if (key == 72 && navCursor > 0) navCursor--;
                else if (key == 80 && navCursor < 5) navCursor++;
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
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                break;
                
            case 1: // Previous Page
                if (currentPage > 1)
                {
                    currentPage--;
                    cursorIndex = (currentPage - 1) * recordsPerPage;
                }
                break;
                
            case 2: // Go to Page
                {
                    gotoxy(center_x - 15, navStartY + 12);
                    set_color(COLOR_YELLOW);
                    printf("Enter page (1-%d): ", totalPages);
                    show_cursor();
                    int newPage;
                    scanf("%d", &newPage);
                    if (newPage >= 1 && newPage <= totalPages)
                    {
                        currentPage = newPage;
                        cursorIndex = (currentPage - 1) * recordsPerPage;
                    }
                }
                break;
                
            case 3: // Set Cursor
                {
                    gotoxy(center_x - 15, navStartY + 12);
                    set_color(COLOR_YELLOW);
                    printf("Enter index (1-%d): ", index_size);
                    show_cursor();
                    int newCursor;
                    scanf("%d", &newCursor);
                    if (newCursor >= 1 && newCursor <= index_size)
                    {
                        cursorIndex = newCursor - 1;
                        currentPage = (cursorIndex / recordsPerPage) + 1;
                    }
                }
                break;
                
            case 4: // Move Cursor
                {
                    gotoxy(center_x - 20, navStartY + 12);
                    set_color(COLOR_YELLOW);
                    printf("U: Up, D: Down: ");
                    show_cursor();
                    char move;
                    scanf(" %c", &move);
                    if (move == 'U' || move == 'u')
                    {
                        if (cursorIndex > 0) cursorIndex--;
                    }
                    else if (move == 'D' || move == 'd')
                    {
                        if (cursorIndex < index_size - 1) cursorIndex++;
                    }
                }
                break;
                
            case 5: // Quit
                continueDisplay = 0;
                break;
            }
        }
    }
    
    // Exit message
    CLEAR_SCREEN();
    gotoxy(center_x - 20, height / 2);
    set_color(COLOR_GREEN);
    printf("Display Completed!\n");
    SLEEP_MS(1000);
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
Pr_index index_Resident[5000];
int index_size = 1;
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

int current_main_menu_selection = 0;
int current_submenu_selection = 0;

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
        printf("the index size :%d, and the StudentId:%d",index_size,Student_ID);
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
    setHeader(F, "freepos", buf.Nb);
    setHeader(F, "tail", current_block);

    Close(F);

    printf("Successfully loaded %d students into database.\n", N);
    printf("Index table size: %d\n", index_size);
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
        Pr_index indextable[5000];
        int Size = 0;
        int C32 = 0;
        loadindextable(fname1, indextable, &Size, &C32);

        // Insert the new student
        int C34 = 0;
        insert_newStudent("STUDENTS_ESI.bin", indextable, Gender,
                          Family_Name, First_Name, Wilaya_Birth, Blood_Type,
                          Year_Study, Speciality_name, Date_Birth, Resident_UC, &C34);

        // Save updated index table
        int C31 = 0;
        save_indextable(indextable, Size, fname1, &C31);

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
            {
                CLEAR_SCREEN();
                printf("\n=== CREATE STUDENTS_ESI.BIN ===\n\n");

                int N;
                printf("Enter the number of initial records to load: ");
                scanf("%d", &N);
                getchar();

                initial_loading(N, &C2);
                printf("\nFile creation completed!\n");
                printf("Cost C2: %d\n", C2);
                displayPrIndexTableWithData(index_pr,index_size,fname,10);
                //--------------------------------
                printf("\n--------------------------\n");

                operation_completed_prompt(0);
                display_file_operations_menu();
                return;
            }
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
            {
                CLEAR_SCREEN();
                printf("\n=== SAVE INDEX TO FILE ===\n\n");
                save_indextable(index_pr, index_size, fname1, &C31);
                printf("Saving index to StudentID_INDEX.idx...\n");
                printf("Index saved successfully!\n");
                printf("Cost C31 would be displayed here.\n");
                printf("The cost here is %d", C31);
                CLEAR_SCREEN();
                displayTOFIndexFile(fname1, 10);
                operation_completed_prompt(0);
                display_file_operations_menu();
                return;
            }
            case 4:
            {
                CLEAR_SCREEN();
                printf("\n=== LOAD INDEX FROM FILE ===\n\n");
                loadindextable(fname1, index_pr, &index_size, &C32);
                printf("Loading index from StudentID_INDEX.idx...\n");
                printf("Index loaded successfully!\n");
                printf("Cost C32 would be displayed here.\n");

                operation_completed_prompt(0);
                display_file_operations_menu();
                return;
            }
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
        "display table ta3 index ",
        "4. Display students under 20 in year range",
        "5. Display students by study year",
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
                display_pr_index(index_pr, index_size);
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 4:
            {
                CLEAR_SCREEN();
                printf("\n=== DISPLAY STUDENTS UNDER 20 ===\n\n");

                printf("Displaying students under 20 years old...\n");
                printf("Cost C43 would be displayed here.\n");

                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
            case 5:
            {
                CLEAR_SCREEN();
                printf("\n=== DISPLAY BY STUDY YEAR ===\n\n");

                printf("Displaying students by study year...\n");
                printf("Cost C44 would be displayed here.\n");

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