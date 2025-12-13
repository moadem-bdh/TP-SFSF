#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>  // Added for srand()
#include <stdio.h>       // Input/output functions (printf, scanf, etc.)
#include <stdlib.h>      // Memory allocation, process control (malloc, free, exit)
#include <string.h>      // String manipulation functions (strcpy, strlen, strcmp)
#include "mainLibrary.h" // this is the library that i will use it for my functions
#include <stdbool.h>     // Boolean type (bool, true, false)


// Add these declarations if they're not in mainLibrary.h
 block buf;
 char filename[50];
 Pr_index index_pr[5000];
int index_size=0 ;

void initial_loading(int N)
{
    t_LnOVS *F;
    
    // Seed random number generator
    srand(time(NULL));
    
    Open(&F, "STUDENTS_ESI.bin", 'N');
    
    index_size = 0;  // Track size of index table
    long current_block = AllocBlock(F);
    long first_block = current_block;  // Save first block
    long previous_block = current_block;
    
    // Initialize buffer
    buf.Nb = 0;
    buf.next = -1;
    
    // Set header - tail should be same as head initially
    setHeader(F, "head", first_block);
    setHeader(F, "tail", first_block);
    
    // Generate N records
    for (int k = 0; k < N; k++)
    {
        // Check if current block is full
        if (buf.Nb >= 40)
        {
            // Write current block to file
            WriteBlock(F, current_block, &buf);
            
            // Allocate new block
            previous_block = current_block;
            current_block = AllocBlock(F);
            
            // Link previous block to new block
            buf.next = current_block;
            WriteBlock(F, previous_block, &buf);
            
            // Reset buffer for new block
            buf.Nb = 0;
            buf.next = -1;
            
            // Update tail in header
            setHeader(F, "tail", current_block);
        }
        
        // Generate unique student ID
        int Student_ID;
        bool found;
        int pos;
        
        do {
            Student_ID = rand_number(9000, 1000);
            binary_search(index_pr, index_size, Student_ID, &found, &pos);
        } while (found);  // Continue until we find a unique ID
        
        // Generate random data
        char Family_Name[30], First_Name[30];
        random_name(Family_Name);
        random_name(First_Name);
        
        int year = rand_number(2008, 2003);
        int month = rand_number(12, 1);
        int day = randomDay(year, month);
        
        int wilya_num = rand_number(58, 1);
        int Gender = rand_number(2, 1);
        int rand_blood = rand_number(8, 1);
        int rand_year = rand_number(5, 1);
        bool Resident_UC = rand_number(1, 0);
        
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
            strcpy(buf.tab[buf.Nb].Speciality, specs[rand_year - 1].subspec[0].name);
        }
        else
        {
            int rand_spec = rand_number(4, 1);
            // Fixed: array index should be 0-3, not 1-4
            strcpy(buf.tab[buf.Nb].Speciality, specs[rand_year - 1].subspec[rand_spec - 1].name);
        }
        
        buf.tab[buf.Nb].Resident_UC = Resident_UC;
        
        // Insert into primary index
        Insert_Pr_index(index_pr, &index_size, Student_ID, current_block, buf.Nb);
        
        buf.Nb++;
    }
    
    // Write the last block
    buf.next = -1;
    WriteBlock(F, current_block, &buf);
    
    // Update headers
    setHeader(F, "freepos", buf.Nb);
    setHeader(F, "tail", current_block);
    
    Close(F);
    
    printf("Successfully loaded %d students into database.\n", N);
    printf("Index table size: %d\n", index_size);
}

int main()
{
    printf("Enter the number of students you want to create: ");
    int N;
    scanf("%d", &N);
    
    if (N <= 0)
    {
        printf("Number must be positive.\n");
        return 1;
    }
    
    initial_loading(N);
    
    // Optional: Verify the data was written correctly
    printf("\nWould you like to display the created database? (1 for yes, 0 for no): ");
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        t_LnOVS *F;
        Open(&F, "STUDENTS_ESI.bin", 'E');
        DisplayAllContentsTable(F);
        Close(F);
    }
    
    return 0;
}