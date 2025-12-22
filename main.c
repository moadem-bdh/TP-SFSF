#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h> // ADDED for toupper()
#include "mainLibrary.h"
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

// Color codes for different terminals
#ifdef _WIN32
#define COLOR_RESET 7
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_MAGENTA 13
#define COLOR_CYAN 11
#define COLOR_WHITE 15
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

// Cursor control functions
#ifdef _WIN32
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// Replace your select_from_list function with this:
int select_from_list(const char *title, const char *items[], int count, int start_y, int center_x) {
    int cursor = 0;
    int selected = -1;
    
    while(selected == -1) {
        // Clear the menu area
        for(int i = 0; i < count + 4; i++) {
            gotoxy(center_x - 25, start_y + i);
            printf("%-50s", ""); // Clear 50 spaces
        }
        
        // Draw simple ASCII box
        gotoxy(center_x - 25, start_y);
#ifdef _WIN32
        set_color(COLOR_CYAN);
#else
        set_color(COLOR_CYAN);
#endif
        printf("==================================================");
        
        gotoxy(center_x - 25, start_y + 1);
        printf("|");
        gotoxy(center_x + 21, start_y + 1);
        printf("|");
        
        // Title - centered
        int title_len = strlen(title);
        int title_start = center_x - (title_len / 2);
        gotoxy(title_start, start_y + 1);
#ifdef _WIN32
        set_color(COLOR_YELLOW);
#else
        set_color(COLOR_YELLOW);
#endif
        printf("%s", title);
        
        gotoxy(center_x - 25, start_y + 2);
#ifdef _WIN32
        set_color(COLOR_CYAN);
#else
        set_color(COLOR_CYAN);
#endif
        printf("==================================================");
        
        // Items
        for(int i = 0; i < count; i++) {
            gotoxy(center_x - 25, start_y + 3 + i);
#ifdef _WIN32
            set_color(COLOR_CYAN);
#else
            set_color(COLOR_CYAN);
#endif
            printf("|");
            
            gotoxy(center_x - 22, start_y + 3 + i);
            if(i == cursor) {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> ");
#ifdef _WIN32
                set_color(COLOR_WHITE);
#else
                set_color(COLOR_WHITE);
#endif
                printf("%-40s", items[i]);
            } else {
#ifdef _WIN32
                set_color(COLOR_GRAY);
#else
                set_color(COLOR_GRAY);
#endif
                printf("  %-40s", items[i]);
            }
            
            gotoxy(center_x + 21, start_y + 3 + i);
#ifdef _WIN32
            set_color(COLOR_CYAN);
#else
            set_color(COLOR_CYAN);
#endif
            printf("|");
        }
        
        gotoxy(center_x - 25, start_y + 3 + count);
#ifdef _WIN32
        set_color(COLOR_CYAN);
#else
        set_color(COLOR_CYAN);
#endif
        printf("==================================================");
        
        // Instructions
        gotoxy(center_x - 25, start_y + 5 + count);
#ifdef _WIN32
        set_color(COLOR_YELLOW);
#else
        set_color(COLOR_YELLOW);
#endif
        printf("UP/DOWN: Navigate   ENTER: Select   ESC: Cancel");
        
        // Get input
#ifdef _WIN32
        int key = _getch();
        if(key == 224 || key == 0) {
            key = _getch();
            if(key == 72 && cursor > 0) {
                cursor--;
            } else if(key == 80 && cursor < count - 1) {
                cursor++;
            }
        } else if(key == 13) {
            selected = cursor;
        } else if(key == 27) {
            return -1; // Cancel
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");
        
        if(key == 27) { // Escape sequence
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");
            
            if(key == 65 && cursor > 0) { // Up
                cursor--;
            } else if(key == 66 && cursor < count - 1) { // Down
                cursor++;
            }
        } else if(key == 10) { // Enter
            selected = cursor;
        } else if(key == 27) { // ESC
            return -1;
        }
#endif
    }
    
    return selected;
}

// Replace your select_from_paged_list function with this:
// Updated select_from_paged_list with proper clearing and organization
int select_from_paged_list(const char *title, const char *items[], int count, int items_per_page, int start_y, int center_x) {
    int cursor = 0;
    int page = 0;
    int selected = -1;
    int total_pages = (count + items_per_page - 1) / items_per_page;
    
    // Clear the entire selection area before starting
    int clear_height = items_per_page + 8;
    for(int i = 0; i < clear_height; i++) {
        gotoxy(center_x - 35, start_y + i);
        printf("%-70s", ""); // Clear 70 spaces
    }
    
    while(selected == -1) {
        // Clear only the dynamic content area (not the full box)
        for(int i = 3; i < items_per_page + 3; i++) {
            gotoxy(center_x - 35, start_y + i);
            printf("%-70s", ""); // Clear 70 spaces
        }
        
        // Draw the fixed box frame
        gotoxy(center_x - 35, start_y);
#ifdef _WIN32
        set_color(COLOR_BLUE);
#else
        set_color(COLOR_BLUE);
#endif
        // Top border
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
#ifdef _WIN32
        set_color(COLOR_YELLOW);
#else
        set_color(COLOR_YELLOW);
#endif
        printf("%s", title_with_page);
        
        // Separator line
        gotoxy(center_x - 35, start_y + 2);
#ifdef _WIN32
        set_color(COLOR_BLUE);
#else
        set_color(COLOR_BLUE);
#endif
        printf("+------------------------------------------------------------------+");
        
        // Display items for current page in an organized way
        int start = page * items_per_page;
        int end = start + items_per_page;
        if(end > count) end = count;
        
        // Calculate how many columns we can fit (2 columns for better organization)
        int cols = 2;
        int rows_per_col = (items_per_page + cols - 1) / cols;
        int col_width = 30;
        
        for(int i = start; i < end; i++) {
            int row = (i - start) % rows_per_col;
            int col = (i - start) / rows_per_col;
            
            int x_pos = center_x - 35 + 3 + (col * col_width);
            int y_pos = start_y + 3 + row;
            
            gotoxy(x_pos, y_pos);
            if(i == cursor) {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> ");
#ifdef _WIN32
                set_color(COLOR_WHITE);
#else
                set_color(COLOR_WHITE);
#endif
                // Truncate if too long for column
                char display_item[col_width - 2];
                strncpy(display_item, items[i], sizeof(display_item) - 1);
                display_item[sizeof(display_item) - 1] = '\0';
                if(strlen(items[i]) > sizeof(display_item) - 1) {
                    display_item[sizeof(display_item) - 4] = '.';
                    display_item[sizeof(display_item) - 3] = '.';
                    display_item[sizeof(display_item) - 2] = '.';
                }
                printf("%s", display_item);
            } else {
#ifdef _WIN32
                set_color(COLOR_GRAY);
#else
                set_color(COLOR_GRAY);
#endif
                printf("  ");
                // Truncate if too long for column
                char display_item[col_width - 2];
                strncpy(display_item, items[i], sizeof(display_item) - 1);
                display_item[sizeof(display_item) - 1] = '\0';
                if(strlen(items[i]) > sizeof(display_item) - 1) {
                    display_item[sizeof(display_item) - 4] = '.';
                    display_item[sizeof(display_item) - 3] = '.';
                    display_item[sizeof(display_item) - 2] = '.';
                }
                printf("%s", display_item);
            }
        }
        
        // Draw vertical column separators
        for(int row = 0; row < rows_per_col; row++) {
            gotoxy(center_x - 35 + col_width - 1, start_y + 3 + row);
            printf("|");
            gotoxy(center_x - 35 + (2 * col_width) - 1, start_y + 3 + row);
            if(col_width * 2 < 68) printf("|");
        }
        
        // Bottom border
        gotoxy(center_x - 35, start_y + 3 + rows_per_col);
#ifdef _WIN32
        set_color(COLOR_BLUE);
#else
        set_color(COLOR_BLUE);
#endif
        printf("+------------------------------------------------------------------+");
        
        // Instructions
        gotoxy(center_x - 35, start_y + 5 + rows_per_col);
#ifdef _WIN32
        set_color(COLOR_YELLOW);
#else
        set_color(COLOR_YELLOW);
#endif
        printf("ARROWS: Navigate  PAGE UP/DOWN: Change Page  ENTER: Select  ESC: Cancel");
        
        // Get input
#ifdef _WIN32
        int key = _getch();
        if(key == 224 || key == 0) {
            key = _getch();
            if(key == 72 && cursor > start) { // Up arrow
                cursor--;
            } else if(key == 80 && cursor < end - 1) { // Down arrow
                cursor++;
            } else if(key == 73 && page > 0) { // Page Up
                page--;
                cursor = page * items_per_page;
            } else if(key == 81 && page < total_pages - 1) { // Page Down
                page++;
                cursor = page * items_per_page;
            } else if(key == 75 && cursor > start) { // Left arrow - move to previous column
                int current_col = (cursor - start) / rows_per_col;
                if(current_col > 0) {
                    cursor -= rows_per_col;
                }
            } else if(key == 77 && cursor < end - 1) { // Right arrow - move to next column
                int current_col = (cursor - start) / rows_per_col;
                if(current_col < cols - 1 && cursor + rows_per_col < end) {
                    cursor += rows_per_col;
                }
            }
        } else if(key == 13) { // Enter
            selected = cursor;
        } else if(key == 27) { // ESC
            return -1;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");
        
        if(key == 27) {
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");
            
            if(key == 65 && cursor > start) { // Up
                cursor--;
            } else if(key == 66 && cursor < end - 1) { // Down
                cursor++;
            } else if(key == 53) { // Page Up (5)
                system("stty raw");
                getchar(); // Skip ~
                system("stty cooked");
                if(page > 0) {
                    page--;
                    cursor = page * items_per_page;
                }
            } else if(key == 54) { // Page Down (6)
                system("stty raw");
                getchar(); // Skip ~
                system("stty cooked");
                if(page < total_pages - 1) {
                    page++;
                    cursor = page * items_per_page;
                }
            } else if(key == 68 && cursor > start) { // Left
                int current_col = (cursor - start) / rows_per_col;
                if(current_col > 0) {
                    cursor -= rows_per_col;
                }
            } else if(key == 67 && cursor < end - 1) { // Right
                int current_col = (cursor - start) / rows_per_col;
                if(current_col < cols - 1 && cursor + rows_per_col < end) {
                    cursor += rows_per_col;
                }
            }
        } else if(key == 10) { // Enter
            selected = cursor;
        } else if(key == 27) { // ESC
            return -1;
        }
#endif
    }
    
    // Clear the entire selection area after selection
    for(int i = 0; i < clear_height; i++) {
        gotoxy(center_x - 35, start_y + i);
        printf("%-70s", ""); // Clear 70 spaces
    }
    
    return selected;
}
// Special function for selecting wilayas with better organization
int select_wilaya(int start_y, int center_x) {
    // Create display strings for wilayas
    char wilaya_display[58][35];
    const char *wilaya_options[58];
    
    for(int i = 0; i < 58; i++) {
        snprintf(wilaya_display[i], sizeof(wilaya_display[i]), "%2d. %s", 
                wilayas[i].number, wilayas[i].name);
        wilaya_options[i] = wilaya_display[i];
    }
    
    return select_from_paged_list("SELECT WILAYA OF BIRTH", 
                                wilaya_options, 58, 20, start_y, center_x);
}

// Special function for selecting blood type
int select_blood_type(int start_y, int center_x) {
    const char *blood_options[8];
    for(int i = 0; i < 8; i++) {
        blood_options[i] = Blood_Types[i].name;
    }
    
    // Use single column display for blood types
    int cursor = 0;
    int selected = -1;
    
    while(selected == -1) {
        // Clear area
        for(int i = 0; i < 12; i++) {
            gotoxy(center_x - 20, start_y + i);
            printf("%-40s", "");
        }
        
        // Draw box
        gotoxy(center_x - 20, start_y);
#ifdef _WIN32
        set_color(COLOR_MAGENTA);
#else
        set_color(COLOR_MAGENTA);
#endif
        printf("+--------------------------------------+");
        
        // Title
        gotoxy(center_x - 20, start_y + 1);
        printf("|");
        gotoxy(center_x + 18, start_y + 1);
        printf("|");
        
        gotoxy(center_x - 8, start_y + 1);
#ifdef _WIN32
        set_color(COLOR_YELLOW);
#else
        set_color(COLOR_YELLOW);
#endif
        printf("SELECT BLOOD TYPE");
        
        gotoxy(center_x - 20, start_y + 2);
#ifdef _WIN32
        set_color(COLOR_MAGENTA);
#else
        set_color(COLOR_MAGENTA);
#endif
        printf("+--------------------------------------+");
        
        // Display blood types in a single column
        for(int i = 0; i < 8; i++) {
            gotoxy(center_x - 20, start_y + 3 + i);
            printf("|");
            gotoxy(center_x + 18, start_y + 3 + i);
            printf("|");
            
            gotoxy(center_x - 17, start_y + 3 + i);
            if(i == cursor) {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> ");
#ifdef _WIN32
                set_color(COLOR_WHITE);
#else
                set_color(COLOR_WHITE);
#endif
                printf("%-30s", blood_options[i]);
            } else {
#ifdef _WIN32
                set_color(COLOR_GRAY);
#else
                set_color(COLOR_GRAY);
#endif
                printf("  %-30s", blood_options[i]);
            }
        }
        
        gotoxy(center_x - 20, start_y + 11);
#ifdef _WIN32
        set_color(COLOR_MAGENTA);
#else
        set_color(COLOR_MAGENTA);
#endif
        printf("+--------------------------------------+");
        
        // Instructions
        gotoxy(center_x - 20, start_y + 12);
#ifdef _WIN32
        set_color(COLOR_YELLOW);
#else
        set_color(COLOR_YELLOW);
#endif
        printf("UP/DOWN: Navigate  ENTER: Select  ESC: Cancel");
        
        // Get input
#ifdef _WIN32
        int key = _getch();
        if(key == 224 || key == 0) {
            key = _getch();
            if(key == 72 && cursor > 0) {
                cursor--;
            } else if(key == 80 && cursor < 7) {
                cursor++;
            }
        } else if(key == 13) {
            selected = cursor;
        } else if(key == 27) {
            return -1;
        }
#else
        system("stty raw");
        int key = getchar();
        system("stty cooked");
        
        if(key == 27) {
            system("stty raw");
            getchar(); // Skip [
            key = getchar();
            system("stty cooked");
            
            if(key == 65 && cursor > 0) {
                cursor--;
            } else if(key == 66 && cursor < 7) {
                cursor++;
            }
        } else if(key == 10) {
            selected = cursor;
        } else if(key == 27) {
            return -1;
        }
#endif
    }
    
    // Clear area after selection
    for(int i = 0; i < 13; i++) {
        gotoxy(center_x - 20, start_y + i);
        printf("%-40s", "");
    }
    
    return selected;
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
    printf("\033[%d;%dH", y, x);
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

// Global variables for menu state
int current_main_menu_selection = 0;
int current_submenu_selection = 0;

// Function prototypes
void display_welcome_screen();
void display_main_menu();
void display_file_operations_menu();
void display_search_operations_menu();
void display_optional_operations_menu();
void display_exit_confirmation();
void press_any_key_to_continue();
void operation_completed_prompt(int return_to_main);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>        // Added for srand()
#include <stdio.h>       // Input/output functions (printf, scanf, etc.)
#include <stdlib.h>      // Memory allocation, process control (malloc, free, exit)
#include <string.h>      // String manipulation functions (strcpy, strlen, strcmp)
#include "mainLibrary.h" // this is the library that i will use it for my functions
#include <stdbool.h>     // Boolean type (bool, true, false)

// Add these declarations if they're not in mainLibrary.h
block buf;
char filename[50];
int C2, C31, C32, C33, C34, C35, C36;

Pr_index index_pr[5000];
Pr_index index_Yearbirth[5000];
Pr_index index_Datebirth[5000];
Pr_index index_Monthbirth[5000];
Pr_index index_Wilayabirth[5000];
Pr_index index_Gender[5000];
Pr_index index_BooldType[5000];
Pr_index index_YearStudy[5000];
Pr_index index_Speciality[5000];
Pr_index index_Resident[5000];
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
void initial_loading(int N, int *C2)
{
    *C2 = 0;
    t_LnOVS *F;

    // Seed random number generator
    srand(time(NULL));

    Open(&F, "STUDENTS_ESI.bin", 'N');

    index_size = 0; // Track size of index table
    long current_block = AllocBlock(F);
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
        Insert_Pr_index(index_Datebirth, &index_size1, day, current_block, buf.Nb);
        Insert_Pr_index(index_BooldType, &index_size2, rand_blood, current_block, buf.Nb);
        Insert_Pr_index(index_Yearbirth, &index_size3, year, current_block, buf.Nb);
        Insert_Pr_index(index_Monthbirth, &index_size4, month, current_block, buf.Nb);
        Insert_Pr_index(index_Gender, &index_size5, Gender, current_block, buf.Nb);
        //  Insert_Pr_index(index_Resident, &index_size, Resident_UC, current_block, buf.Nb);
        Insert_Pr_index(index_YearStudy, &index_size6, rand_year, current_block, buf.Nb);
        Insert_Pr_index(index_Speciality, &index_size7, rand_spec, current_block, buf.Nb);
        Insert_Pr_index(index_Wilayabirth, &index_size8, wilya_num, current_block, buf.Nb);

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
int main()
{
    // Initialize
    srand(time(NULL));

    // Display welcome screen
    display_welcome_screen();

    // Main program loop
    while (1)
    {
        display_main_menu();
    }

    return 0;
}

void display_welcome_screen()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();

    // Calculate center positions
    int center_x = width / 2;

// Top border - Using simple ASCII characters
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // School name with animation
    gotoxy(center_x - 18, 5);
#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
    printf("ECOLE NATIONALE SUPERIEURE D'INFORMATIQUE");

    gotoxy(center_x - 2, 6);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("(ESI)");

    // Project title
    gotoxy(center_x - 30, 10);
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("Practical Work - FSDS: LOF Data File Management System");

    // Decorative line
    gotoxy(center_x - 20, 11);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    for (int i = 0; i < 40; i++)
        printf("~");
    printf("\n");

    // Teacher information
    gotoxy(center_x - 15, 15);
#ifdef _WIN32
    set_color(COLOR_BLUE);
#else
    set_color(COLOR_BLUE);
#endif
    printf("Supervised by: ");
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("Mr. KERMI Adel");

    // Students information
    gotoxy(center_x - 15, 17);
#ifdef _WIN32
    set_color(COLOR_BLUE);
#else
    set_color(COLOR_BLUE);
#endif
    printf("Developed by:  ");
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("SIFI Mohamed");

    gotoxy(center_x - 15, 18);
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("               BOUDEHANE Mohamed Adem");

    // Bottom border
    gotoxy(0, height - 5);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // Continue prompt - Using ASCII only
    gotoxy(center_x - 15, height - 3);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("Press ");
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("ENTER");
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf(" to continue to main menu...");

    // Wait for Enter key
    show_cursor();
#ifdef _WIN32
    while (getch() != 13)
        ; // Wait for Enter
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

// Header - Using ASCII characters only
#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
    for (int i = 0; i < width; i++)
        printf("*");
    printf("\n");

    gotoxy(center_x - 10, 2);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    printf("MAIN MENU");

    gotoxy(center_x - 20, 3);
#ifdef _WIN32
    set_color(COLOR_GRAY);
#else
    set_color(COLOR_GRAY);
#endif
    printf("LOF Data File Management System");

#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
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

    // Display all options first
    for (int i = 0; i < num_options; i++)
    {
        gotoxy(center_x - 20, start_y + i * 2);
        if (i == num_options - 1)
        { // Last option (Exit)
#ifdef _WIN32
            set_color(COLOR_RED);
#else
            set_color(COLOR_RED);
#endif
        }
        else if (i == 0)
        { // First option
#ifdef _WIN32
            set_color(COLOR_GREEN);
#else
            set_color(COLOR_GREEN);
#endif
        }
        else
        {
#ifdef _WIN32
            set_color(COLOR_YELLOW);
#else
            set_color(COLOR_YELLOW);
#endif
        }
        printf("%s", menu_options[i]);
    }

    // Footer
    gotoxy(0, height - 3);
#ifdef _WIN32
    set_color(COLOR_GRAY);
#else
    set_color(COLOR_GRAY);
#endif
    for (int i = 0; i < width; i++)
        printf("-");
    printf("\n");

    gotoxy(center_x - 15, height - 2);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("Use ");
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("ARROW KEYS");
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf(" to navigate, ");
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("ENTER");
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf(" to select");

    // Get user selection with cursor - Use simple ASCII indicator
    int selected = 0;
    while (!selected)
    {
        // Highlight current selection
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 23, start_y + i * 2);
            if (i == current_main_menu_selection)
            {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> "); // Simple ASCII arrow
            }
            else
            {
                printf("  "); // Just spaces
            }
        }

// Get key input
#ifdef _WIN32
        int key = getch();
        if (key == 224 || key == 0)
        {
            key = getch();
            if (key == 72 && current_main_menu_selection > 0)
            { // Up arrow
                current_main_menu_selection--;
            }
            else if (key == 80 && current_main_menu_selection < num_options - 1)
            { // Down arrow
                current_main_menu_selection++;
            }
        }
        else if (key == 13)
        { // Enter
            selected = 1;
            int selection = current_main_menu_selection + 1;

            // Handle selection
            switch (selection)
            {
            case 1:
                display_file_operations_menu();
                break;
            case 2:
                printf("\nIndex File Operations selected...\n");
                printf("\nThis would call your index operations functions.\n");
                operation_completed_prompt(1);
                break;
            case 3:
                printf("\nStudent Record Operations selected...\n");
                printf("\nThis would call your student operations functions.\n");
                operation_completed_prompt(1);
                break;
            case 4:
                display_search_operations_menu();
                break;
            case 5:
                printf("\nCreate LOF File (STUDENTS_CP.BIN) selected...\n");
                printf("\nThis would create STUDENTS_CP.BIN file.\n");
                operation_completed_prompt(1);
                break;
            case 6:
                display_optional_operations_menu();
                break;
            case 7:
                display_exit_confirmation();
                break;
            }
            current_main_menu_selection = 0;
            return;
        }
        else if (key == 27)
        {
            display_exit_confirmation();
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
            selected = 1;
            int selection = current_main_menu_selection + 1;

            switch (selection)
            {
            case 1:
                display_file_operations_menu();
                break;
            case 7:
                display_exit_confirmation();
                break;
            default:
                printf("\nOption %d selected...\n", selection);
                operation_completed_prompt(1);
                break;
            }
            current_main_menu_selection = 0;
            return;
        }
        else if (key == 27)
        {
            display_exit_confirmation();
            return;
        }
#endif
    }
}

// Ask user what to do after operation completes
void operation_completed_prompt(int return_to_main)
{
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

    printf("\n\n");
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    for (int i = 0; i < width; i++)
        printf("-");
    printf("\n");

    gotoxy(center_x - 15, height - 8);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("Operation completed successfully!");

    gotoxy(center_x - 20, height - 6);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("What would you like to do next?");

    gotoxy(center_x - 15, height - 4);
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("[R] Return");

    if (return_to_main)
    {
        gotoxy(center_x - 15, height - 3);
#ifdef _WIN32
        set_color(COLOR_GRAY);
#else
        set_color(COLOR_GRAY);
#endif
        printf("[M] Main Menu");
    }

    gotoxy(center_x - 15, height - 2);
#ifdef _WIN32
    set_color(COLOR_RED);
#else
    set_color(COLOR_RED);
#endif
    printf("[E] Exit Program");

#ifdef _WIN32
    int key;
    do
    {
        key = getch();
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

void display_file_operations_menu()
{
    CLEAR_SCREEN();
    hide_cursor();

    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;

// Header
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    gotoxy(center_x - 15, 2);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    printf("FILE OPERATIONS MENU");

#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
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

    // Display all options
    for (int i = 0; i < num_options; i++)
    {
        gotoxy(center_x - 25, start_y + i * 2);
        if (i == num_options - 1)
        {
#ifdef _WIN32
            set_color(COLOR_BLUE);
#else
            set_color(COLOR_BLUE);
#endif
        }
        else if (i == 0)
        {
#ifdef _WIN32
            set_color(COLOR_YELLOW);
#else
            set_color(COLOR_YELLOW);
#endif
        }
        else
        {
#ifdef _WIN32
            set_color(COLOR_WHITE);
#else
            set_color(COLOR_WHITE);
#endif
        }
        printf("%s", submenu_options[i]);
    }

    // Footer
    gotoxy(0, height - 3);
#ifdef _WIN32
    set_color(COLOR_GRAY);
#else
    set_color(COLOR_GRAY);
#endif
    for (int i = 0; i < width; i++)
        printf("-");
    printf("\n");

    gotoxy(center_x - 10, height - 2);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("Press ");
#ifdef _WIN32
    set_color(COLOR_RED);
#else
    set_color(COLOR_RED);
#endif
    printf("ESC");
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf(" to go back");

    // Selection logic
    int selected = 0;
    while (!selected)
    {
        // Highlight current selection
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 28, start_y + i * 2);
            if (i == current_submenu_selection)
            {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> ");
            }
            else
            {
                printf("  ");
            }
        }

// Get key input
#ifdef _WIN32
        int key = getch();
        if (key == 224 || key == 0)
        {
            key = getch();
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
            selected = 1;
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
                save_indextable(index_pr, index_size, fname, &C31);
                printf("Saving index to StudentID_INDEX.idx...\n");
                printf("Index saved successfully!\n");
                printf("Cost C31 would be displayed here.\n");
                printf("The cost here is ", C31);
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
        // Linux key handling
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
            selected = 1;
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
#ifdef _WIN32
    set_color(COLOR_BLUE);
#else
    set_color(COLOR_BLUE);
#endif
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    gotoxy(center_x - 15, 2);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    printf("SEARCH & ANALYSIS MENU");

#ifdef _WIN32
    set_color(COLOR_BLUE);
#else
    set_color(COLOR_BLUE);
#endif
    gotoxy(0, 3);
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    // Submenu options
    int start_y = 8;
    char *submenu_options[] = {
        "1. Search student by ID",
        "2. Display students by blood type & campus",
        "3. Display students by speciality",
        "4. Display students under 20 in year range",
        "5. Display students by study year",
        "6. Return to Main Menu"};

    int num_options = 6;
    current_submenu_selection = 0;

    for (int i = 0; i < num_options; i++)
    {
        gotoxy(center_x - 25, start_y + i * 2);
        if (i == num_options - 1)
        {
#ifdef _WIN32
            set_color(COLOR_BLUE);
#else
            set_color(COLOR_BLUE);
#endif
        }
        else if (i == 0)
        {
#ifdef _WIN32
            set_color(COLOR_YELLOW);
#else
            set_color(COLOR_YELLOW);
#endif
        }
        else
        {
#ifdef _WIN32
            set_color(COLOR_WHITE);
#else
            set_color(COLOR_WHITE);
#endif
        }
        printf("%s", submenu_options[i]);
    }

    // Footer
    gotoxy(0, height - 3);
#ifdef _WIN32
    set_color(COLOR_GRAY);
#else
    set_color(COLOR_GRAY);
#endif
    for (int i = 0; i < width; i++)
        printf("-");
    printf("\n");

    gotoxy(center_x - 10, height - 2);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("Costs: C33, C41, C42, C43, C44");

    // Selection logic
    int selected = 0;
    while (!selected)
    {
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 28, start_y + i * 2);
            if (i == current_submenu_selection)
            {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> ");
            }
            else
            {
                printf("  ");
            }
        }

#ifdef _WIN32
        int key = getch();
        if (key == 224 || key == 0)
        {
            key = getch();
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
            selected = 1;
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
                Search_StuDentID(student_id, index_pr, &found, &num_block, &offset,&C33, index_size);
                printf("Cost C33 would be displayed here.\n");
                printf("\nThe Cost of This Operation is : %d", C33);
                if (!found)
                {
                    printf("\n User not Found ");
                }
                else
                {
                    printf("\n The User Information found i block number %d , and offset number %d", num_block, offset);
                }
                operation_completed_prompt(0);
                display_search_operations_menu();
                return;
            }
           




case 2:
{
    CLEAR_SCREEN();
    
    int width = get_terminal_width();
    int height = get_terminal_height();
    int center_x = width / 2;
    
    // Show header
    gotoxy(0, 2);
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    for(int i = 0; i < width; i++) printf("=");
    
    gotoxy(center_x - 10, 3);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    printf("INSERT NEW USER");
    
    gotoxy(0, 4);
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    for(int i = 0; i < width; i++) printf("=");
    
    // Variables
    int C34;
    int Gender = 0;
    char Family_Name[30], First_Name[30], Wilaya_Birth[20] = "", 
         Blood_Type[50] = "", Year_Study[10] = "", Speciality_name[60] = "";
    DateS Date_Birth = {0, 0, 0};
    bool Resident_UC = false;
    
    // Get basic information
    gotoxy(center_x - 20, 7);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("BASIC INFORMATION");
    
    gotoxy(center_x - 20, 8);
    for(int i = 0; i < 40; i++) printf("-");
    
    // Family Name
    gotoxy(center_x - 20, 10);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("Family Name: ");
    
    // Clear input buffer
    fflush(stdin);
    
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    // Position cursor for input and get it
    gotoxy(center_x - 8, 10);
    fgets(Family_Name, sizeof(Family_Name), stdin);
    Family_Name[strcspn(Family_Name, "\n")] = 0;
    
    // First Name
    gotoxy(center_x - 20, 12);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("First Name: ");
    
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    gotoxy(center_x - 8, 12);
    fgets(First_Name, sizeof(First_Name), stdin);
    First_Name[strcspn(First_Name, "\n")] = 0;
    
    // Gender Selection
    {
        const char *gender_options[] = {"Male", "Female"};
        int choice = select_from_list("SELECT GENDER", gender_options, 2, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        Gender = choice + 1;
    }
    
    // Wilaya Birth Selection (using paged list with 2 columns)
    {
        // Create display strings for wilayas
        char wilaya_display[58][35];
        const char *wilaya_options[58];
        
        for(int i = 0; i < 58; i++) {
            snprintf(wilaya_display[i], sizeof(wilaya_display[i]), "%2d. %s", 
                    wilayas[i].number, wilayas[i].name);
            wilaya_options[i] = wilaya_display[i];
        }
        
        int choice = select_from_paged_list("SELECT WILAYA OF BIRTH", 
                                          wilaya_options, 58, 20, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        strcpy(Wilaya_Birth, wilayas[choice].name);
    }
    
    // Blood Type Selection
    {
        int choice = select_blood_type(15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        strcpy(Blood_Type, Blood_Types[choice].name);
    }
    
    // Year of Study Selection
    {
        const char *year_options[] = {"1CP", "2CP", "1CS", "2CS", "3CS"};
        int choice = select_from_list("SELECT YEAR OF STUDY", year_options, 5, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        strcpy(Year_Study, year_options[choice]);
    }
    
    // Speciality Selection
    {
        // Find the correct specialities based on year
        int year_index = -1;
        for(int i = 0; i < 5; i++) {
            if(strcmp(specs[i].years, Year_Study) == 0) {
                year_index = i;
                break;
            }
        }
        
        if(year_index != -1) {
            Speciality spec = specs[year_index];
            const char *spec_options[10];
            int spec_count = 0;
            
            // Count available specialities
            while(spec_count < 10 && strlen(spec.subspec[spec_count].name) > 0) {
                spec_options[spec_count] = spec.subspec[spec_count].name;
                spec_count++;
            }
            
            if(spec_count > 0) {
                int choice = select_from_list("SELECT SPECIALITY", 
                                            spec_options, spec_count, 15, center_x);
                if(choice == -1) {
                    operation_completed_prompt(0);
                    display_search_operations_menu();
                    return;
                }
                strcpy(Speciality_name, spec_options[choice]);
            }
        }
    }
    
    // Resident Status Selection
    {
        const char *resident_options[] = {"No - Not a resident", "Yes - University Campus Resident"};
        int choice = select_from_list("RESIDENT STATUS", resident_options, 2, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        Resident_UC = (choice == 1);
    }
    
    // Date of Birth - Year Selection
    {
        // Create year options (1950-2010)
        char year_strings[61][10];
        const char *year_options[61];
        
        for(int i = 0; i < 61; i++) {
            int year = 1950 + i;
            snprintf(year_strings[i], sizeof(year_strings[i]), "%d", year);
            year_options[i] = year_strings[i];
        }
        
        int choice = select_from_paged_list("SELECT BIRTH YEAR", 
                                          year_options, 61, 20, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        Date_Birth.year = 1950 + choice;
    }
    
    // Date of Birth - Month Selection
    {
        const char *month_options[] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        
        int choice = select_from_list("SELECT BIRTH MONTH", month_options, 12, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        Date_Birth.month = choice + 1;
    }
    
    // Date of Birth - Day Selection
    {
        int max_days = getDaysInMonth(Date_Birth.year, Date_Birth.month);
        
        // Create day options
        char day_strings[31][10];
        const char *day_options[31];
        
        for(int i = 0; i < max_days; i++) {
            snprintf(day_strings[i], sizeof(day_strings[i]), "%d", i + 1);
            day_options[i] = day_strings[i];
        }
        
        int choice = select_from_list("SELECT BIRTH DAY", day_options, max_days, 15, center_x);
        if(choice == -1) {
            operation_completed_prompt(0);
            display_search_operations_menu();
            return;
        }
        Date_Birth.day = choice + 1;
    }
    
    // Confirmation Screen
    CLEAR_SCREEN();
    
    // Header
    gotoxy(0, 2);
#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
    for(int i = 0; i < width; i++) printf("=");
    
    gotoxy(center_x - 15, 3);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    printf("CONFIRM USER INFORMATION");
    
    gotoxy(0, 4);
#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
    for(int i = 0; i < width; i++) printf("=");
    
    // Display all information with ASCII box
    int y = 7;
    
    // Top border
    gotoxy(center_x - 30, y);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("+------------------------------------------------------------+");
    y++;
    
    // Title
    gotoxy(center_x - 30, y);
    printf("|                    USER INFORMATION                        |");
    y++;
    
    // Separator
    gotoxy(center_x - 30, y);
    printf("+------------------------------------------------------------+");
    y++;
    
    // Family Name
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Family Name", Family_Name);
    y++;
    
    // First Name
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "First Name", First_Name);
    y++;
    
    // Gender
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Gender", (Gender == 1) ? "Male" : "Female");
    y++;
    
    // Wilaya of Birth
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Wilaya of Birth", Wilaya_Birth);
    y++;
    
    // Blood Type
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Blood Type", Blood_Type);
    y++;
    
    // Year of Study
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Year of Study", Year_Study);
    y++;
    
    // Speciality
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Speciality", Speciality_name);
    y++;
    
    // Date of Birth
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %02d/%02d/%04d%30s |", "Date of Birth", 
           Date_Birth.day, Date_Birth.month, Date_Birth.year, "");
    y++;
    
    // Campus Resident
    gotoxy(center_x - 30, y);
    printf("|  %-20s: %-35s |", "Campus Resident", Resident_UC ? "Yes" : "No");
    y++;
    
    // Bottom border
    gotoxy(center_x - 30, y);
    printf("+------------------------------------------------------------+");
    
    // Ask for confirmation
    gotoxy(center_x - 15, y + 3);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("Confirm insertion? (Y/N): ");
    
    show_cursor();
    char confirm;
    scanf(" %c", &confirm);
    getchar(); // Clear newline
    
    if(confirm == 'Y' || confirm == 'y') {
        // Load index table first
        Pr_index indextable[5000];
        int Size = 0;
        int C32 = 0;
        loadindextable("PRIMARY_INDEX.bin", indextable, &Size, &C32);
        
        // Insert the new student
        insert_newStudent("STUDENTS_ESI.bin", indextable, Gender, 
                         Family_Name, First_Name, Wilaya_Birth, Blood_Type, 
                         Year_Study, Speciality_name, Date_Birth, Resident_UC, &C34);
        
        // Save updated index table
        int C31 = 0;
        save_indextable(indextable, Size, "PRIMARY_INDEX.bin", &C31);
        
        // Success message
        gotoxy(center_x - 15, y + 5);
#ifdef _WIN32
        set_color(COLOR_GREEN);
#else
        set_color(COLOR_GREEN);
#endif
        printf("User inserted successfully!");
        
        // Show the generated ID
        gotoxy(center_x - 20, y + 6);
#ifdef _WIN32
        set_color(COLOR_CYAN);
#else
        set_color(COLOR_CYAN);
#endif
        printf("Student ID has been automatically generated.");
    } else {
        // Cancellation message
        gotoxy(center_x - 10, y + 5);
#ifdef _WIN32
        set_color(COLOR_RED);
#else
        set_color(COLOR_RED);
#endif
        printf("Insertion cancelled.");
    }
    
    // Wait for user to press Enter
    gotoxy(center_x - 15, y + 8);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("Press Enter to continue...");
    
    // Clear input buffer and wait for Enter
    while(getchar() != '\n');
    
    operation_completed_prompt(0);
    display_search_operations_menu();
    return;
}
            case 3:
            {
                CLEAR_SCREEN();
                printf("\n=== DISPLAY BY SPECIALITY ===\n\n");

                printf("Displaying students by speciality...\n");
                printf("Cost C42 would be displayed here.\n");

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
#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
    for (int i = 0; i < width; i++)
        printf("=");
    printf("\n");

    gotoxy(center_x - 12, 2);
#ifdef _WIN32
    set_color(COLOR_CYAN);
#else
    set_color(COLOR_CYAN);
#endif
    printf("OPTIONAL OPERATIONS");

#ifdef _WIN32
    set_color(COLOR_MAGENTA);
#else
    set_color(COLOR_MAGENTA);
#endif
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

    for (int i = 0; i < num_options; i++)
    {
        gotoxy(center_x - 20, start_y + i * 2);
        if (i == num_options - 1)
        {
#ifdef _WIN32
            set_color(COLOR_BLUE);
#else
            set_color(COLOR_BLUE);
#endif
        }
        else
        {
#ifdef _WIN32
            set_color(COLOR_YELLOW);
#else
            set_color(COLOR_YELLOW);
#endif
        }
        printf("%s", submenu_options[i]);
    }

    // Selection logic
    int selected = 0;
    while (!selected)
    {
        for (int i = 0; i < num_options; i++)
        {
            gotoxy(center_x - 23, start_y + i * 2);
            if (i == current_submenu_selection)
            {
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("> ");
            }
            else
            {
                printf("  ");
            }
        }

#ifdef _WIN32
        int key = getch();
        if (key == 224 || key == 0)
        {
            key = getch();
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
            selected = 1;
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

// Draw simple ASCII confirmation box
#ifdef _WIN32
    set_color(COLOR_RED);
#else
    set_color(COLOR_RED);
#endif

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
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("EXIT CONFIRMATION");

    // Message
    gotoxy(center_x - 16, center_y - 1);
#ifdef _WIN32
    set_color(COLOR_WHITE);
#else
    set_color(COLOR_WHITE);
#endif
    printf("Are you sure you want to exit?");

    // Options - Simple ASCII
    gotoxy(center_x - 8, center_y + 1);
#ifdef _WIN32
    set_color(COLOR_GREEN);
#else
    set_color(COLOR_GREEN);
#endif
    printf("[ YES ]");

    gotoxy(center_x + 4, center_y + 1);
#ifdef _WIN32
    set_color(COLOR_RED);
#else
    set_color(COLOR_RED);
#endif
    printf("[ NO ]");

    // Selection logic
    int selection = 0;
    int selected = 0;

    while (!selected)
    {
        if (selection == 0)
        {
            gotoxy(center_x - 8, center_y + 1);
#ifdef _WIN32
            set_color(COLOR_GREEN);
#else
            set_color(COLOR_GREEN);
#endif
            printf("[ YES ]");

            gotoxy(center_x + 4, center_y + 1);
#ifdef _WIN32
            set_color(COLOR_WHITE);
#else
            set_color(COLOR_WHITE);
#endif
            printf("[ NO ]");
        }
        else
        {
            gotoxy(center_x - 8, center_y + 1);
#ifdef _WIN32
            set_color(COLOR_WHITE);
#else
            set_color(COLOR_WHITE);
#endif
            printf("[ YES ]");

            gotoxy(center_x + 4, center_y + 1);
#ifdef _WIN32
            set_color(COLOR_RED);
#else
            set_color(COLOR_RED);
#endif
            printf("[ NO ]");
        }

#ifdef _WIN32
        int key = getch();
        if (key == 224 || key == 0)
        {
            key = getch();
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
#ifdef _WIN32
                set_color(COLOR_GREEN);
#else
                set_color(COLOR_GREEN);
#endif
                printf("Thank you for using ESI File Manager!");
#ifdef _WIN32
                set_color(COLOR_RESET);
#else
                set_color(COLOR_RESET);
#endif
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

void press_any_key_to_continue()
{
    int width = get_terminal_width();
    int height = get_terminal_height();

    gotoxy(width / 2 - 15, height - 1);
#ifdef _WIN32
    set_color(COLOR_YELLOW);
#else
    set_color(COLOR_YELLOW);
#endif
    printf("Press any key to continue...");

#ifdef _WIN32
    getch();
#else
    system("stty raw");
    getchar();
    system("stty cooked");
#endif
}