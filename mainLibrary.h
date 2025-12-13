#include <stdio.h>   // Input/output functions (printf, scanf, etc.)
#include <stdlib.h>  // Memory allocation, process control (malloc, free, exit)
#include <string.h>  // String manipulation functions (strcpy, strlen, strcmp)
#include <stdbool.h> // Boolean type (bool, true, false)

//  Definition of the structures

typedef struct
{
   int year, month, day;

} DateS;

typedef struct
{
   int Student_ID, Gender;
   char Family_Name[30], First_Name[30], Wilaya_Birth[20], Blood_Type[50], Year_Study[10], Speciality[60];
   DateS Date_Birth;
   bool Resident_UC;

} rec;

typedef struct block
{
   rec tab[40];
   int Nb;
   long next;
} block;

typedef struct
{
   long head; // the first block in the file (i.e. the head block of the list of blocks)
   long tail;
   long freepos; // the first free position in the last block (i.e. in the tail block of the list)

   long lostspace; // the total space lost due to logical deletions
   long newblck;   // the file border (the first free block available to expand the file size)
   long freeblck;  // head of the list of freed blocks (list of the unused blocks)

} header;

typedef struct LnOVS_file
{
   FILE *f;  // C stream implementing the file
   header h; // the header in main memory
} t_LnOVS;
// The fundalental functions

// the structures related to the primary index table

// this is for the coordinates(block number, offset ) of the key in the primary index
typedef struct
{
   int block_number, offset;

} Pr_cor;

typedef struct
{
   int Student_ID;
   Pr_cor crdt; // this for the coordinates
} Pr_index;

typedef struct
{
   int number;
   char name[50];
} object1;

object1 wilayas[58] = {
    {1, "Adrar"}, {2, "Chlef"}, {3, "Laghouat"}, {4, "Oum El Bouaghi"}, {5, "Batna"}, {6, "Bejaia"}, {7, "Biskra"}, {8, "Bechar"}, {9, "Blida"}, {10, "Bouira"}, {11, "Tamanrasset"}, {12, "Tebessa"}, {13, "Tlemcen"}, {14, "Tiaret"}, {15, "Tizi Ouzou"}, {16, "Algiers"}, {17, "Djelfa"}, {18, "Jijel"}, {19, "Setif"}, {20, "Saida"}, {21, "Skikda"}, {22, "Sidi Bel Abbes"}, {23, "Annaba"}, {24, "Guelma"}, {25, "Constantine"}, {26, "Medea"}, {27, "Mostaganem"}, {28, "M'Sila"}, {29, "Mascara"}, {30, "Ouargla"}, {31, "Oran"}, {32, "El Bayadh"}, {33, "Illizi"}, {34, "Bordj Bou Arreridj"}, {35, "Boumerdes"}, {36, "El Tarf"}, {37, "Tindouf"}, {38, "Tissemsilt"}, {39, "El Oued"}, {40, "Khenchela"}, {41, "Souk Ahras"}, {42, "Tipaza"}, {43, "Mila"}, {44, "Ain Defla"}, {45, "Naama"}, {46, "Ain Temouchent"}, {47, "Ghardaia"}, {48, "Relizane"}, {49, "Timimoun"}, {50, "Bordj Badji Mokhtar"}, {51, "Ouled Djellal"}, {52, "Béni Abbès"}, {53, "In Salah"}, {54, "In Guezzam"}, {55, "Touggourt"}, {56, "Djanet"}, {57, "El Meghaier"}, {58, "El Menia"}};

object1 Blood_Types[8] = {{1, " O+"}, {2, " A+"}, {3, "B+"}, {4, " O-"}, {5, "A-"}, {6, " AB+"}, {7, "B-"}, {8, "AB-"}};

typedef struct
{
   char name[60];

} object2;
typedef struct
{
   char years[10];
   object2 subspec[10];

} Speciality;

Speciality specs[5] = {
    {"1CP", {"Integrated Preparatory Classes"}}, {"2CP", {"Integrated Preparatory Classes"}}, {"1CS", {"Common Core"}}, {"2CS", {"Information Systems and Technologies (SIT)", "Computer Systems (SIQ)", "Software and Computer Systems (SIL)", "Intelligent Systems and Data (SID)"}}, {"3CS", {"Information Systems and Technologies (SIT)", "Computer Systems (SIQ)", "Software and Computer Systems (SIL)", "Intelligent Systems and Data (SID)"}}

};

// open a LnOVS file / mode ='N' for a New file and mode ='E' for an Existing file
// returns in *F a pointer to a newly allocated variable of type 't_LnOVS'
void Open(t_LnOVS **F, char *fname, char mode)
{
   block buf;

   *F = malloc(sizeof(t_LnOVS));

   if (mode == 'E' || mode == 'e')
   {
      // openning an existing LnOVS file ...
      (*F)->f = fopen(fname, "rb+");
      if ((*F)->f == NULL)
      {
         perror("LnOVS_open");
         exit(EXIT_FAILURE);
      }
      // loading header part in main memory (in (*F)->h)
      fread(&((*F)->h), sizeof(header), 1, (*F)->f);
   }
   else
   {
      // creating a new LnOVS file ...
      (*F)->f = fopen(fname, "wb+");
      if ((*F)->f == NULL)
      {
         perror("LnOVS_open");
         exit(EXIT_FAILURE);
      }
      // initializing the header part in main memory (in (*F)->h)
      (*F)->h.head = 1; // the first allocated empty block
      (*F)->h.tail = 1;
      (*F)->h.freepos = 0;   // first free position in the last block
      (*F)->h.lostspace = 0; // initially there are 0 bytes occupied by the logically deleted records
      (*F)->h.newblck = 2;   // the next allocated new block (in case the list of freed blocks is empty)
      (*F)->h.freeblck = -1; // initially the list of freed blocks is empty
      // writing the headers at offset 0 of stream (*F)->f
      fwrite(&((*F)->h), sizeof(header), 1, (*F)->f);
      // writing the first allocated block
      buf.next = -1;
      fwrite(&buf, sizeof(block), 1, (*F)->f);
   }
} // LnOVS_open

// close a LnOVS file :
// the header is first saved at the beginning of the file (offset 0) and the t_LnOVS variable is freed
void Close(t_LnOVS *F)
{
   // saving header part in secondary memory (at the begining of the stream F->f)
   fseek(F->f, 0L, SEEK_SET);
   fwrite(&F->h, sizeof(header), 1, F->f);
   fclose(F->f);
   free(F);
}

// reading data block number i into variable buf
void ReadBlock(t_LnOVS *F, long i, block *buf)
{
   fseek(F->f, sizeof(header) + (i - 1) * sizeof(block), SEEK_SET);
   fread(buf, sizeof(block), 1, F->f);
}

// writing the contents of the variable buf in data block number i
void WriteBlock(t_LnOVS *F, long i, block *buf)
{
   fseek(F->f, sizeof(header) + (i - 1) * sizeof(block), SEEK_SET);
   fwrite(buf, sizeof(block), 1, F->f);
}

// header updates in main memory
void setHeader(t_LnOVS *F, char *hname, long val)
{
   if (strcmp(hname, "head") == 0)
   {
      F->h.head = val;
      return;
   }
   if (strcmp(hname, "tail") == 0)
   {
      F->h.tail = val;
      return;
   }
   if (strcmp(hname, "freepos") == 0)
   {
      F->h.freepos = val;
      return;
   }
   if (strcmp(hname, "lostspace") == 0)
   {
      F->h.lostspace = val;
      return;
   }
   if (strcmp(hname, "newblck") == 0)
   {
      F->h.newblck = val;
      return;
   }
   if (strcmp(hname, "freeblck") == 0)
   {
      F->h.freeblck = val;
      return;
   }
   fprintf(stderr, "setHeader : Unknown headerName: \"%s\"\n", hname);
}

// header values (from main memory)
long getHeader(t_LnOVS *F, char *hname)
{
   if (strcmp(hname, "head") == 0)
      return F->h.head;
   if (strcmp(hname, "tail") == 0)
      return F->h.tail;
   if (strcmp(hname, "freepos") == 0)
      return F->h.freepos;
   if (strcmp(hname, "lostspace") == 0)
      return F->h.lostspace;
   if (strcmp(hname, "newblck") == 0)
      return F->h.newblck;
   if (strcmp(hname, "freeblck") == 0)
      return F->h.freeblck;
   fprintf(stderr, "getHeader : Unknown headerName: \"%s\"\n", hname);
}

// allocate a new block to the file
long AllocBlock(t_LnOVS *F)
{
   long i;
   block buf;

   if (F->h.freeblck == -1)
   {
      // if there is no freed block, extend the file size by one additional block
      i = F->h.newblck;
      F->h.newblck++;
   }
   else
   {
      // else, reuse the first block from the list of freed blocks...
      i = F->h.freeblck;
      // and update the head of the list of freed blocks to point to the next block
      // = deletion at the beginning of the list
      ReadBlock(F, i, &buf);
      F->h.freeblck = buf.next;
   }

   return i;
}

// mark block i as unused
void freeBlock(t_LnOVS *F, long i)
{
   block buf;

   // add i to the list of freed blocks (insertion at the beginning of the ist) ...
   buf.next = F->h.freeblck;
   WriteBlock(F, i, &buf);
   F->h.freeblck = i; // the new head of the free-blocks list is now i
}

// function related to the primary index table

int binary_search(Pr_index index_table[5000], int size, int key, bool *found, int *i)
{
   if (size == 0)
   {
      *found = false,
      *i = 0;
      return 1;
   }
   int min = 0, max = size - 1;
   int middle;
   *found = false;
   while (min <= max)
   {
      middle = min + (max - min) / 2;
      if (index_table[middle].Student_ID == key)
      {
         *found = true;
         *i = middle;
         return 0;
      }
      else
      {
         if (index_table[middle].Student_ID > key)
         {
            max = middle - 1;
         }
         else
         {
            min = middle + 1;
         }
      }
   }

   *i = min;
   return 1;
}

int Insert_Pr_index(Pr_index index_table[5000], int *Size, int ID, int block_number, int offset)
{

   bool found;
   int i;
   int K = *Size;
   binary_search(index_table, *Size, ID, &found, &i);
   if (!found)
   {
      while (K >= i + 1)
      {
         index_table[K] = index_table[K - 1];
         K--;
      }
      index_table[i].Student_ID = ID;
      index_table[i].crdt.block_number = block_number;
      index_table[i].crdt.offset = offset;
   }
   (*Size)++;
}

// additional functions
int rand_number(int max, int min)
{
   return min + rand() % (max - min + 1);
}

void random_name(char name[30])
{

   int i = rand_number(30, 4);

   int k;
   for (int j = 1; j < i; j++)
   {
      k = rand_number(26, 1);

      name[j] = k + 64;
   }
}

int isLeapYear(int year)
{
   return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int getDaysInMonth(int year, int month)
{
   switch (month)
   {
   case 1:
   case 3:
   case 5:
   case 7:
   case 8:
   case 10:
   case 12:
      return 31;
   case 4:
   case 6:
   case 9:
   case 11:
      return 30;
   case 2:
      return isLeapYear(year) ? 29 : 28;
   default:
      return -1; // invalid month
   }
}

// Generate random day based on year+month
int randomDay(int year, int month)
{
   int days = getDaysInMonth(year, month);
   if (days == -1)
      return -1; // invalid month
   return (rand() % days) + 1;
}

void DisplayAllContentsTable(t_LnOVS *F)
{
    block currentBlock;
    rec studentRecord;
    
    printf("\n=========================================================================================================\n");
    printf("                                     STUDENT DATABASE - TABLE VIEW\n");
    printf("=========================================================================================================\n");
    printf(" ID   | Name                     | Gender | Birth Date  | Wilaya        | Year  | Speciality\n");
    printf("------+--------------------------+--------+-------------+---------------+-------+------------\n");
    
    long currentBlockNum = getHeader(F, "head");
    long tailBlock = getHeader(F, "tail");
    long freePos = getHeader(F, "freepos");
    int recordCount = 0;
    
    while (currentBlockNum != -1)
    {
        ReadBlock(F, currentBlockNum, &currentBlock);
        
        int maxRecords = currentBlock.Nb;
        if (currentBlockNum == tailBlock)
        {
            maxRecords = freePos;
        }
        
        for (int i = 0; i < maxRecords; i++)
        {
            studentRecord = currentBlock.tab[i];
            
            // Skip deleted records (if Student_ID = -1)
            if (studentRecord.Student_ID == -1)
                continue;
            
            recordCount++;
            
            // Format name
            char fullName[35];
            snprintf(fullName, sizeof(fullName), "%s %s", 
                     studentRecord.Family_Name, studentRecord.First_Name);
            
            // Truncate if too long
            if (strlen(fullName) > 22)
            {
                fullName[19] = '.';
                fullName[20] = '.';
                fullName[21] = '.';
                fullName[22] = '\0';
            }
            
            // Format speciality
            char speciality[20];
            strncpy(speciality, studentRecord.Speciality, 19);
            speciality[19] = '\0';
            if (strlen(studentRecord.Speciality) > 19)
            {
                speciality[16] = '.';
                speciality[17] = '.';
                speciality[18] = '.';
            }
            
            // Format wilaya
            char wilaya[20];
            strncpy(wilaya, studentRecord.Wilaya_Birth, 19);
            wilaya[19] = '\0';
            if (strlen(studentRecord.Wilaya_Birth) > 19)
            {
                wilaya[16] = '.';
                wilaya[17] = '.';
                wilaya[18] = '.';
            }
            
            // Format year
            char year[10];
            strncpy(year, studentRecord.Year_Study, 9);
            year[9] = '\0';
            
            printf(" %-5d| %-24s| %-7s| %02d/%02d/%04d | %-15s | %-6s| %s\n",
                   studentRecord.Student_ID,
                   fullName,
                   studentRecord.Gender == 1 ? "Male" : "Female",
                   studentRecord.Date_Birth.day,
                   studentRecord.Date_Birth.month,
                   studentRecord.Date_Birth.year,
                   wilaya,
                   year,
                   speciality);
        }
        
        currentBlockNum = currentBlock.next;
    }
    
    printf("=========================================================================================================\n");
    printf("Total Active Records: %d\n\n", recordCount);
}

// Function to display a single record in detail
void DisplayRecordDetail(rec student)
{
   printf("\n════════════════════════════════════════════════════════════\n");
   printf("                 STUDENT DETAILED VIEW\n");
   printf("════════════════════════════════════════════════════════════\n");

   // Personal Information
   printf("┌─ PERSONAL INFORMATION\n");
   printf("│  Student ID: %d\n", student.Student_ID);
   printf("│  Full Name: %s %s\n", student.Family_Name, student.First_Name);
   printf("│  Gender: %s\n", student.Gender == 1 ? "Male" : student.Gender == 2 ? "Female"
                                                                                : "Not specified");

   // Date of Birth with validation
   if (student.Date_Birth.year > 1900 && student.Date_Birth.year < 2100 &&
       student.Date_Birth.month >= 1 && student.Date_Birth.month <= 12 &&
       student.Date_Birth.day >= 1 && student.Date_Birth.day <= 31)
   {
      printf("│  Date of Birth: %02d/%02d/%04d\n",
             student.Date_Birth.day,
             student.Date_Birth.month,
             student.Date_Birth.year);
      printf("│  Age: %d years\n", 2024 - student.Date_Birth.year);
   }
   else
   {
      printf("│  Date of Birth: Invalid date\n");
   }

   printf("│  Birth Wilaya: %s\n", student.Wilaya_Birth);

   // Academic Information
   printf("├─ ACADEMIC INFORMATION\n");
   printf("│  Year of Study: %s\n", student.Year_Study);
   printf("│  Speciality: %s\n", student.Speciality);

   // Medical Information
   printf("├─ MEDICAL INFORMATION\n");
   printf("│  Blood Type: %s\n", student.Blood_Type);

   // Residential Status
   printf("├─ RESIDENTIAL STATUS\n");
   printf("│  University Campus Resident: %s\n",
          student.Resident_UC ? "Yes" : "No");

   printf("└───────────────────────────────────────────────────────────\n");
}