#include <stdio.h>       // Input/output functions (printf, scanf, etc.)
#include <stdlib.h>      // Memory allocation, process control (malloc, free, exit)
#include <string.h>      // String manipulation functions (strcpy, strlen, strcmp)
#include "mainLibrary.h" // this is the library that i will use it for my functions
#include <stdbool.h>     // Boolean type (bool, true, false)

block buf;
char filename[50] = "STUDENTS_ESI.bin";
Pr_index index_pr[5000];
void initial_loading(int N)
{
    t_LnOVS *F;
    Open(&F, "STUDENTS_ESI.bin", 'N');
    setHeader(F, "head", -1);
    setHeader(F, "freepos", 0);
    setHeader(F, "lostspace", -1);
    setHeader(F, "newblck", 0);
    setHeader(F, "freeblck", -1);
    long i = AllocBlock(F);
    long j = i;
    int wilya_num;
    int Student_ID, Gender, year, month, day, pos;
    char Family_Name[30], First_Name[30], Wilaya_Birth[20], Blood_Type, Year_Study, Speciality;
    int rand_blood;
    int rand_year; 
    int resident; 
    bool Resident_UC, found;

    buf.Nb = 0;
    setHeader(F, "head", i);
    for (int k = 1; k < N; k++)
    {
        if (buf.Nb < 40)
        {
            Student_ID = rand_number(9000, 1000);
            while (binray_search(index_pr, k - 1, Student_ID, found, &pos))
            {
                Student_ID = rand_number(9000, 1000);
            }
            random_name(Family_Name);
            random_name(First_Name);
            year = rand_number(2008, 2003);
            month = rand_number(12, 1);
            day = randomDay(year, month);
            buf.tab[buf.Nb].Student_ID = Student_ID;
            buf.tab[buf.Nb].Date_Birth.year = year;
            buf.tab[buf.Nb].Date_Birth.month = month;
            buf.tab[buf.Nb].Date_Birth.day = day;
            strcpy(buf.tab[buf.Nb].Family_Name, Family_Name);
            strcpy(buf.tab[buf.Nb].First_Name, First_Name);
            wilya_num = rand_number(58, 1);
            strcpy(buf.tab[buf.Nb].Wilaya_Birth, wilayas[wilya_num - 1].name);
         
            Gender=rand_number(2,1);
            buf.tab[buf.Nb].Gender=Gender;
             rand_blood=rand_number(8,1);
             
            strcpy(buf.tab[buf.Nb].Blood_Type,Blood_Types[rand_blood-1].name);
            rand_year=rand_number(5,1);
            strcpy(buf.tab[buf.Nb].Year_Study,specs[rand_year-1].years);
            if (rand_year<=3)
            {
               strcpy(buf.tab[buf.Nb].Speciality,specs[rand_year-1].subspec[0].name);
            }
            else
            {
               int rand_spec= rand_number(4,1);
                strcpy(buf.tab[buf.Nb].Speciality,specs[rand_year-1].subspec[rand_spec].name);

            }
           
            Resident_UC=rand_number(1,0);
            buf.tab[buf.Nb].Resident_UC=Resident_UC;
            
            buf.Nb++;
        }
        else
        {
            i = AllocBlock(F);
            buf.next = i;
            WriteBlock(F, j, &buf);
            j = i;
            buf.Nb = 1;
            // assigniw hna record ta3na
            // ta3 hadi l iteration
        }
    }
    buf.next = -1;
    WriteBlock(F, i, &buf);
    setHeader(F, "freepos", buf.Nb);

    Close(F);
}
int main()
{
    char name[30];
    printf("start\n");
    random_name(name);
    printf("this :%s", name);
    printf("this is name%s", name);
    printf("\nend");
    return 0;
}