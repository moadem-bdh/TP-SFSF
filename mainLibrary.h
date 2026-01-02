#ifndef MAINLIBRARY_H
#define MAINLIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_INDEX 10000

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

struct block
{
   rec tab[40];
   int Nb;
   long next;
   long prev;
};

typedef struct block block;

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
// Core file operation functions

// the structures related to the primary index table

// this is for the coordinates(block number, offset ) of the key in the primary index
typedef struct
{
   int block_number, offset;

} Pr_cor;

typedef struct
{
   int Identifier;
   Pr_cor crdt; // this for the coordinates
} Pr_index;

typedef struct
{
   int number;
   char name[50];
} object1;

object1 wilayas[58] = {
    {1, "Adrar"}, {2, "Chlef"}, {3, "Laghouat"}, {4, "Oum El Bouaghi"}, {5, "Batna"}, {6, "Bejaia"}, {7, "Biskra"}, {8, "Bechar"}, {9, "Blida"}, {10, "Bouira"}, {11, "Tamanrasset"}, {12, "Tebessa"}, {13, "Tlemcen"}, {14, "Tiaret"}, {15, "Tizi Ouzou"}, {16, "Algiers"}, {17, "Djelfa"}, {18, "Jijel"}, {19, "Setif"}, {20, "Saida"}, {21, "Skikda"}, {22, "Sidi Bel Abbes"}, {23, "Annaba"}, {24, "Guelma"}, {25, "Constantine"}, {26, "Medea"}, {27, "Mostaganem"}, {28, "M'Sila"}, {29, "Mascara"}, {30, "Ouargla"}, {31, "Oran"}, {32, "El Bayadh"}, {33, "Illizi"}, {34, "Bordj Bou Arreridj"}, {35, "Boumerdes"}, {36, "El Tarf"}, {37, "Tindouf"}, {38, "Tissemsilt"}, {39, "El Oued"}, {40, "Khenchela"}, {41, "Souk Ahras"}, {42, "Tipaza"}, {43, "Mila"}, {44, "Ain Defla"}, {45, "Naama"}, {46, "Ain Temouchent"}, {47, "Ghardaia"}, {48, "Relizane"}, {49, "Timimoun"}, {50, "Bordj Badji Mokhtar"}, {51, "Ouled Djellal"}, {52, "Béni Abbès"}, {53, "In Salah"}, {54, "In Guezzam"}, {55, "Touggourt"}, {56, "Djanet"}, {57, "El Meghaier"}, {58, "El Menia"}};

object1 Blood_Types[8] = {{1, "O+"}, {2, "A+"}, {3, "B+"}, {4, "O-"}, {5, "A-"}, {6, "AB+"}, {7, "B-"}, {8, "AB-"}};

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
    {"1CP", {{"Integrated Preparatory Classes"}}},
    {"2CP", {{"Integrated Preparatory Classes"}}},
    {"1CS", {{"Common Core"}}},
    {"2CS", {{"Information Systems and Technologies (SIT)"}, {"Computer Systems (SIQ)"}, {"Software and Computer Systems (SIL)"}, {"Intelligent Systems and Data (SID)"}}},
    {"3CS", {{"Information Systems and Technologies (SIT)"}, {"Computer Systems (SIQ)"}, {"Software and Computer Systems (SIL)"}, {"Intelligent Systems and Data (SID)"}}}};

// strcutures of the index table TOF file
typedef struct
{
   int Identifier;
   Pr_cor crdt;
} rec_ip;

typedef struct
{
   rec_ip tab[40];
   int Nb;
} block_ip;

typedef struct
{
   int nblk;
} header_ip;

typedef struct
{
   FILE *f;
   header_ip head;
} TOF_ip;
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
      buf.prev = -1;
      buf.Nb = 0;
      fwrite(&buf, sizeof(block), 1, (*F)->f);
   }
} // LnOVS_open

void Open_TOF(TOF_ip **F, char *fname, char mode)
{
   block_ip buf;

   *F = malloc(sizeof(TOF_ip));

   if (mode == 'E' || mode == 'e')
   {
      // openning an existing LnOVS file ...
      (*F)->f = fopen(fname, "rb+");
      if ((*F)->f == NULL)
      {
         perror("TOF_open");
         exit(EXIT_FAILURE);
      }
      // loading header part in main memory (in (*F)->h)
      fread(&((*F)->head), sizeof(header_ip), 1, (*F)->f);
   }
   else
   {
      // creating a new LnOVS file ...
      (*F)->f = fopen(fname, "wb+");
      if ((*F)->f == NULL)
      {
         perror("TOF_open");
         exit(EXIT_FAILURE);
      }
      // initializing the header part in main memory (in (*F)->h)
      (*F)->head.nblk = 0; // Changed from 1 to 0 (no blocks initially)

      // writing the headers at offset 0 of stream (*F)->f
      fwrite(&((*F)->head), sizeof(header_ip), 1, (*F)->f);
      // writing the first allocated block
      buf.Nb = 0; // Initialize buffer
      fwrite(&buf, sizeof(block_ip), 1, (*F)->f);
   }
} // TOF open

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

void Close_TOF(TOF_ip *F)
{
   // saving header part in secondary memory (at the begining of the stream F->f)
   fseek(F->f, 0L, SEEK_SET);
   fwrite(&F->head, sizeof(header_ip), 1, F->f);
   fclose(F->f);
   free(F);
} // Close TOF file

// reading data block number i into variable buf
void ReadBlock(t_LnOVS *F, long i, block *buf)
{
   fseek(F->f, sizeof(header) + (i - 1) * sizeof(block), SEEK_SET);
   fread(buf, sizeof(block), 1, F->f);
}

void ReadBlock_ip(TOF_ip *F, int i, block_ip *buf)
{
   fseek(F->f, sizeof(header_ip) + (i - 1) * sizeof(block_ip), SEEK_SET);
   fread(buf, sizeof(block_ip), 1, F->f);
}

// writing the contents of the variable buf in data block number i
void WriteBlock(t_LnOVS *F, int i, block *buf)
{
   fseek(F->f, sizeof(header) + (i - 1) * sizeof(block), SEEK_SET);
   fwrite(buf, sizeof(block), 1, F->f);
}

void WriteBlock_ip(TOF_ip *F, int i, block_ip *buf)
{
   fseek(F->f, sizeof(header_ip) + (i - 1) * sizeof(block_ip), SEEK_SET);
   fwrite(buf, sizeof(block_ip), 1, F->f);
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

void setHeader_ip(TOF_ip *F, char *hname, int val)
{
   if (strcmp(hname, "nblk") == 0)
   {
      F->head.nblk = val;
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
   return -1;
}

int getHeader_ip(TOF_ip *F, char *hname)
{
   if (strcmp(hname, "nblk") == 0)
      return F->head.nblk;
   fprintf(stderr, "getHeader : Unknown headerName: \"%s\"\n", hname);
   return -1;
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
};

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
int binary_search(Pr_index index_table[], int size, int key, bool *found, int *i)
{

   int min = 0, max = size - 1;
   int middle;
   *found = false;
   while (min <= max && !(*found))
   {
      middle = min + (max - min) / 2;
      if (index_table[middle].Identifier == key)
      {
         *found = true;
         *i = middle;
         return 0;
      }
      else
      {
         if (index_table[middle].Identifier > key)
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

int Insert_Pr_index(Pr_index *index_table, int *Size,
                    int ID, int block_number, int offset)
{
   bool found;
   int i;

   binary_search(index_table, *Size, ID, &found, &i);

   if (found)
   {
      return 1;
   }

   // shift elements to the right
   for (int K = *Size; K > i; K--)
   {
      index_table[K] = index_table[K - 1];
   }

   // insert new entry
   index_table[i].Identifier = ID;
   index_table[i].crdt.block_number = block_number;
   index_table[i].crdt.offset = offset;

   (*Size)++;
   return 0;
}

int Insert_index(Pr_index index_table[], int *Size, int ID, int block_number, int offset)
{
   bool found;
   int i;
   int K = *Size;
   binary_search(index_table, *Size, ID, &found, &i);

   while (K >= i + 1)
   {
      index_table[K] = index_table[K - 1];
      K--;
   }
   index_table[i].Identifier = ID;
   index_table[i].crdt.block_number = block_number;
   index_table[i].crdt.offset = offset;

   (*Size)++;
   return 0;
}

int Insert_index_CP(Pr_index index_table[], int *Size, int ID, rec buff, int block_number, int offset)
{
   // Check if student is 1CP or 2CP
   // Also check that student is not deleted
   if (buff.Student_ID != -1 &&
       (strcmp(buff.Year_Study, "1CP") == 0 || strcmp(buff.Year_Study, "2CP") == 0))
   {
      bool found;
      int i;
      int K = *Size;

      binary_search(index_table, *Size, ID, &found, &i);

      // If student already exists in CP index, don't insert duplicate
      if (found)
      {
         return 0; // Already exists
      }

      // Shift elements to make room
      while (K >= i + 1)
      {
         index_table[K] = index_table[K - 1];
         K--;
      }

      // Insert the new entry
      index_table[i].Identifier = ID;
      index_table[i].crdt.block_number = block_number;
      index_table[i].crdt.offset = offset;

      (*Size)++;
      return 1; // Return 1 to indicate successful insertion
   }

   return 0; // Return 0 if not a CP student or if student is deleted
}

// additional functions
int rand_number(int max, int min)
{
   return min + rand() % (max - min + 1);
}

void random_name(char name[30])
{
   int i = rand_number(30, 4);

   for (int j = 0; j < i; j++)
   {
      int k = rand_number(26, 1);
      name[j] = k + 64;
   }
   name[i] = '\0';
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

int save_indextable(Pr_index index_table[], int Size, char *fname, int *C31)
{
   *C31 = 0;
   TOF_ip *F;

   // Open file for writing (create new)
   Open_TOF(&F, fname, 'N');

   block_ip buf;
   buf.Nb = 0;

   // Start from block 1
   int current_block = 1;

   for (int i = 0; i < Size; i++)
   {
      if (buf.Nb >= 40)
      {
         // Block is full, write it
         WriteBlock_ip(F, current_block, &buf);
         (*C31)++;

         // Reset buffer for next block
         buf.Nb = 0;
         current_block++;
      }

      // Add record to buffer
      buf.tab[buf.Nb].Identifier = index_table[i].Identifier;
      buf.tab[buf.Nb].crdt = index_table[i].crdt;
      buf.Nb++;
   }

   // Write the last (possibly partial) block
   if (buf.Nb > 0)
   {
      WriteBlock_ip(F, current_block, &buf);
      (*C31)++;
      // Don't increment current_block after writing last block
   }
   else
   {
      // If no data was written at all, we still need at least 1 block
      WriteBlock_ip(F, 1, &buf);
      (*C31)++;
   }

   // Update header with total blocks
   // If we wrote blocks 1 through current_block, then total blocks = current_block
   // But if we never incremented current_block beyond 1, total blocks = 1
   F->head.nblk = (*C31 > 0) ? (*C31) : 1;

   Close_TOF(F);

   return 0;
}

int loadindextable(char *filename, Pr_index indextable[], int *Size, int *C32)
{
   *C32 = 0;
   TOF_ip *F;
   *Size = 0; // Reset size to 0

   // Open existing file
   Open_TOF(&F, filename, 'E');

   block_ip buf;
   int Number_of_blocks;

   Number_of_blocks = getHeader_ip(F, "nblk");

   // Validate number of blocks
   if (Number_of_blocks <= 0)
   {
      printf("No blocks to read in index file %s\n", filename);
      Close_TOF(F);
      return 0; // Success - just empty file
   }

   for (int i = 1; i <= Number_of_blocks; i++)
   {
      ReadBlock_ip(F, i, &buf);
      (*C32)++;

      for (int j = 0; j < buf.Nb; j++)
      {
         // Check array bounds - CORRECTED
         if (*Size >= MAX_INDEX)
         {
            printf("Warning: Index table full at %d entries\n", MAX_INDEX);
            Close_TOF(F);
            return -1; // Error: array full
         }

         indextable[*Size].Identifier = buf.tab[j].Identifier;
         indextable[*Size].crdt = buf.tab[j].crdt;
         (*Size)++;
      }
   }

   Close_TOF(F);

   return 0; // Return success
}
int Search_StuDentID(int StudentID, Pr_index indextable[], bool *found, int *number_of_block, int *offset, int *C33, int Size)
{
   *C33 = 0;

   int position;

   binary_search(indextable, Size, StudentID, found, &position);

   if (*found)
   {
      *number_of_block = indextable[position].crdt.block_number;
      *offset = indextable[position].crdt.offset;
      return 1;
   }
   return 0;
}

int insert_newStudent(char *filename, Pr_index indextable[], int *Size, int Gender,
                      char Family_Name[], char First_Name[], char Wilaya_Birth[], char Blood_Type[], char Year_Study[], char Speciality[],
                      DateS Date_Birth,
                      bool Resident_UC, int *C34)
{
   *C34 = 0;

   bool found;
   int position;

   int Student_ID;

   do
   {
      Student_ID = rand_number(9000, 1000);
      binary_search(indextable, *Size, Student_ID, &found, &position);
   } while (found); // Continue until we find a unique ID

   t_LnOVS *F;
   long last_block;
   block buf;

   Open(&F, filename, 'E');
   last_block = getHeader(F, "tail");
   ReadBlock(F, last_block, &buf);
   (*C34)++;
   if (buf.Nb < 40)
   {
      buf.tab[buf.Nb].Student_ID = Student_ID;
      buf.tab[buf.Nb].Date_Birth = Date_Birth;

      strcpy(buf.tab[buf.Nb].Family_Name, Family_Name);
      strcpy(buf.tab[buf.Nb].First_Name, First_Name);
      strcpy(buf.tab[buf.Nb].Wilaya_Birth, Wilaya_Birth);

      buf.tab[buf.Nb].Gender = Gender;
      strcpy(buf.tab[buf.Nb].Blood_Type, Blood_Type);
      strcpy(buf.tab[buf.Nb].Year_Study, Year_Study);

      // Set speciality
      strcpy(buf.tab[buf.Nb].Speciality, Speciality);

      buf.tab[buf.Nb].Resident_UC = Resident_UC;
      buf.Nb++;
      setHeader(F, "freepos", buf.Nb);
      WriteBlock(F, last_block, &buf);
      (*C34)++;
   }
   else
   {
      long j = last_block;
      last_block = AllocBlock(F);
      buf.next = last_block;
      WriteBlock(F, j, &buf);
      (*C34)++;
      ReadBlock(F, last_block, &buf);
      (*C34)++;
      buf.prev = j;
      buf.next = -1;
      buf.Nb = 0;
      buf.tab[buf.Nb].Student_ID = Student_ID;
      buf.tab[buf.Nb].Date_Birth = Date_Birth;

      strcpy(buf.tab[buf.Nb].Family_Name, Family_Name);
      strcpy(buf.tab[buf.Nb].First_Name, First_Name);
      strcpy(buf.tab[buf.Nb].Wilaya_Birth, Wilaya_Birth);

      buf.tab[buf.Nb].Gender = Gender;
      strcpy(buf.tab[buf.Nb].Blood_Type, Blood_Type);
      strcpy(buf.tab[buf.Nb].Year_Study, Year_Study);

      // Set speciality
      strcpy(buf.tab[buf.Nb].Speciality, Speciality);

      buf.tab[buf.Nb].Resident_UC = Resident_UC;
      buf.next = -1;
      buf.Nb++;
      WriteBlock(F, last_block, &buf);
      (*C34)++;
   }

   setHeader(F, "freepos", buf.Nb);
   setHeader(F, "tail", last_block);
   Insert_Pr_index(indextable, Size, Student_ID, last_block, buf.Nb - 1);

   Close(F);

   return 0;
}

int Delete_from_index(Pr_index indextable[], int *Size, int StudentID)
{
   bool found;
   int position;

   binary_search(indextable, *Size, StudentID, &found, &position);

   if (!found)
   {
      return 0;
   }
   else
   {
      for (int k = position; k < *Size - 1; k++)
      {
         indextable[k] = indextable[k + 1];
      }
      (*Size)--;
      return 1;
   }
}
// FIXED: simple_search with bounds checking and validation
int simple_search(Pr_index indextable[], int Size, int block_number, int offset)
{
   // Validate input parameters
   if (Size <= 0 || block_number <= 0 || offset < 0)
   {
      return -1; // Invalid parameters
   }
   
   for (int i = 0; i < Size; i++)
   {
      if (indextable[i].crdt.block_number == block_number && 
          indextable[i].crdt.offset == offset)
      {
         return i; // Found at position i
      }
   }
   return -1; // Not found
}

// FIXED: Delete_from_index1 with proper error handling
int Delete_from_index1(Pr_index indextable[], int *Size, int block_number, int offset)
{
   // Validate parameters first
   if (*Size <= 0 || block_number <= 0 || offset < 0 || indextable == NULL)
   {
      return 0; // Invalid parameters or empty table
   }
   
   int pos = simple_search(indextable, *Size, block_number, offset);
   
   if (pos == -1)
   {
      // Element not found - nothing to delete
      return 0;
   }
   
   // Validate that pos is within bounds (should be, but double-check)
   if (pos < 0 || pos >= *Size)
   {
      return 0; // Invalid position
   }
   
   // Shift elements to the left to remove the element at pos
   for (int k = pos; k < *Size - 1; k++)
   {
      indextable[k] = indextable[k + 1];
   }
   
   // Decrement size
   (*Size)--;
   
   return 1; // Success
}

int Delete_by_student_ID(Pr_index indextable[], int *Size, int StudentID, char *fname, int *C35)
{
   *C35 = 0;
   bool found;
   int position;

   binary_search(indextable, *Size, StudentID, &found, &position);

   if (!found)
   {
      return 0;
   }
   else
   {
      t_LnOVS *F;
      Open(&F, fname, 'E');
      block buf, bufl;
      long last_block = getHeader(F, "tail");
      long first_block = getHeader(F, "head");
      ReadBlock(F, indextable[position].crdt.block_number, &buf);
      (*C35)++;
      if (first_block == last_block && buf.Nb == 1)
      {
         setHeader(F, "tail", -1);
         setHeader(F, "head", -1);
         Close(F);
         return 1;
      }
      else
      {

         if (indextable[position].crdt.block_number == last_block)
         {
            buf.tab[indextable[position].crdt.offset] = buf.tab[buf.Nb - 1];
            buf.Nb--;
            if (buf.Nb == 0)
            {
               setHeader(F, "tail", buf.prev);
               setHeader(F, "freepos", 0);
            }
            else
            {
               setHeader(F, "freepos", buf.Nb);
               WriteBlock(F, last_block, &buf);
               (*C35)++;
            }
         }
         else
         {
            ReadBlock(F, last_block, &bufl);
            (*C35)++;
            buf.tab[indextable[position].crdt.offset] = bufl.tab[bufl.Nb - 1];
            WriteBlock(F, indextable[position].crdt.block_number, &buf);
            (*C35)++;
            bufl.Nb--;

            if (bufl.Nb == 0)
            {
               setHeader(F, "tail", bufl.prev);
            }
            else
            {
               WriteBlock(F, last_block, &bufl);
               (*C35)++;
            }
         }
      }

      Close(F);
      return 1;
   }
}

int binary_searchm_ulitple(Pr_index index_table[], int size, int key, bool *found, int *i)
{
   if (size == 0)
   {
      *found = false;
      *i = 0;
      return 1;
   }
   int min = 0, max = size - 1;
   int middle;
   *found = false;

   while (min <= max)
   {
      middle = min + (max - min) / 2;
      if (index_table[middle].Identifier == key)
      {
         *found = true;
         *i = middle;
         max = middle - 1;
         return 0;
      }
      else
      {
         if (index_table[middle].Identifier > key)
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

int Insert_Pr_index_multiples(Pr_index index_table[], int *Size, int ID, int block_number, int offset)
{
   bool found;
   int i;
   int K = *Size;

   binary_search(index_table, *Size, ID, &found, &i);

   while (K >= i + 1)
   {
      index_table[K] = index_table[K - 1];
      K--;
   }

   index_table[i].Identifier = ID;
   index_table[i].crdt.block_number = block_number;
   index_table[i].crdt.offset = offset;

   (*Size)++;
   return 0;
}

int modification_first_name(int StudentID, char *firstname, Pr_index indextable[], int Size, char *fname, int *C36)
{
   *C36 = 0;
   bool found;
   int position;

   binary_search(indextable, Size, StudentID, &found, &position);

   if (!found)
   {
      return 0; // Student not found
   }
   else
   {
      t_LnOVS *F;
      Open(&F, fname, 'E'); // 'E' for existing file
      block buf;

      ReadBlock(F, indextable[position].crdt.block_number, &buf);
      (*C36)++;
      // Update the family name
      strcpy(buf.tab[indextable[position].crdt.offset].First_Name, firstname);

      // Write back the updated block
      WriteBlock(F, indextable[position].crdt.block_number, &buf);
      (*C36)++;

      // Close the file
      Close(F);

      return 1; // Success
   }
}

//=========================================================================================
// Display functions
//=========================================================================================
int find_value_Upper_bound_index(Pr_index index_table[], int size, int value)
{

   bool found;
   int i;

   binary_search(index_table, size, value, &found, &i);

   if (found)
   {

      while (i > 0 && index_table[i - 1].Identifier == value)
      {
         i--;
      }

      return i;
   }
   else
   {

      printf("None in the list has that type of blood\n");
      return -1;
   }
};

// 4.1. Question  Display students by thier blood type

void display_students_by_Blood(Pr_index index_table[], int size, int value, char *fname, int *C41)
{
   int start = find_value_Upper_bound_index(index_table, size, value);
   if (start < 0)
   {
      return;
   }

   t_LnOVS *F;
   block buffer;
   rec record;
   Open(&F, fname, 'E');

   // Print table header
   printf("\n==============================================================================================================================================\n");
   printf("  ID   | Name                     | Gender | Birth Date  | Wilaya          | Blood  | Year  | Speciality                | Resident | Block,Offset\n");
   printf("-------+--------------------------+--------+-------------+-----------------+--------+-------+---------------------------+----------+-------------\n");

   int k = start;
   while ((k < size) && (index_table[k].Identifier == value))
   {
      if (index_table[k].Identifier == value)
      {

         Pr_cor coordinats = index_table[k].crdt;
         ReadBlock(F, coordinats.block_number, &buffer);
         if (C41 != NULL)
         {
            (*C41)++;
         }
         record = buffer.tab[coordinats.offset];

         // Format full name
         char fullName[26];
         snprintf(fullName, sizeof(fullName), "%s %s", record.Family_Name, record.First_Name);
         if (strlen(fullName) > 24)
         {
            fullName[21] = '.';
            fullName[22] = '.';
            fullName[23] = '.';
            fullName[24] = '\0';
         }

         // Format wilaya
         char wilaya[17];
         strncpy(wilaya, record.Wilaya_Birth, 16);
         wilaya[16] = '\0';
         if (strlen(record.Wilaya_Birth) > 16)
         {
            wilaya[13] = '.';
            wilaya[14] = '.';
            wilaya[15] = '.';
         }

         // Format speciality
         char speciality[27];
         strncpy(speciality, record.Speciality, 26);
         speciality[26] = '\0';
         if (strlen(record.Speciality) > 26)
         {
            speciality[23] = '.';
            speciality[24] = '.';
            speciality[25] = '.';
         }

         // Print student row
         printf(" %-5d | %-24s | %-6s | %02d/%02d/%04d | %-15s | %-6s | %-5s | %-25s | %-8s | (%d,%d)\n",
                record.Student_ID,
                fullName,
                record.Gender == 1 ? "Male" : "Female",
                record.Date_Birth.day,
                record.Date_Birth.month,
                record.Date_Birth.year,
                wilaya,
                record.Blood_Type,
                record.Year_Study,
                speciality,
                record.Resident_UC ? "Yes" : "No",
                coordinats.block_number,
                coordinats.offset);
      }
      k++;
   }

   printf("==============================================================================================================================================\n\n");

   if (C41 != NULL)
   {
      printf("The complexity in this operation is: %d\n", *C41);
   }

   Close(F);
}

// 4.2. Question  Display students by thier speciality
void display_students_by_Speciality(Pr_index index_table[], int size, int value, char *fname, int *C42)
{
   int start = find_value_Upper_bound_index(index_table, size, value);
   if (start < 0)
   {
      return;
   }

   t_LnOVS *F;
   block buffer;
   rec record;
   Open(&F, fname, 'E');

   // Print table header
   printf("\n==============================================================================================================================================\n");
   printf("  ID   | Name                     | Gender | Birth Date  | Wilaya          | Blood  | Year  | Speciality                | Resident | Block,Offset\n");
   printf("-------+--------------------------+--------+-------------+-----------------+--------+-------+---------------------------+----------+-------------\n");

   int k = start;
   while ((k < size) && (index_table[k].Identifier == value))
   {
      if (index_table[k].Identifier == value)
      {

         Pr_cor coordinats = index_table[k].crdt;
         ReadBlock(F, coordinats.block_number, &buffer);
         if (C42 != NULL)
         {
            (*C42)++;
         }

         record = buffer.tab[coordinats.offset];

         // Format full name
         char fullName[26];
         snprintf(fullName, sizeof(fullName), "%s %s", record.Family_Name, record.First_Name);
         if (strlen(fullName) > 24)
         {
            fullName[21] = '.';
            fullName[22] = '.';
            fullName[23] = '.';
            fullName[24] = '\0';
         }

         // Format wilaya
         char wilaya[17];
         strncpy(wilaya, record.Wilaya_Birth, 16);
         wilaya[16] = '\0';
         if (strlen(record.Wilaya_Birth) > 16)
         {
            wilaya[13] = '.';
            wilaya[14] = '.';
            wilaya[15] = '.';
         }

         // Format speciality
         char speciality[27];
         strncpy(speciality, record.Speciality, 26);
         speciality[26] = '\0';
         if (strlen(record.Speciality) > 26)
         {
            speciality[23] = '.';
            speciality[24] = '.';
            speciality[25] = '.';
         }

         // Print student row
         printf(" %-5d | %-24s | %-6s | %02d/%02d/%04d | %-15s | %-6s | %-5s | %-25s | %-8s | (%d,%d)\n",
                record.Student_ID,
                fullName,
                record.Gender == 1 ? "Male" : "Female",
                record.Date_Birth.day,
                record.Date_Birth.month,
                record.Date_Birth.year,
                wilaya,
                record.Blood_Type,
                record.Year_Study,
                speciality,
                record.Resident_UC ? "Yes" : "No",
                coordinats.block_number,
                coordinats.offset);
      }
      k++;
   }

   printf("==============================================================================================================================================\n\n");

   if (C42 != NULL)
   {
      printf("The complexity in this operation is: %d\n", *C42);
   }

   Close(F);
}

// 4.3. Question  Display students interval of birthday in intrval Y1 Y2
void display_students_by_birth_interval(Pr_index index_table[], int size, int Y1, int Y2, char *fname, int *C43)
{
   // Validate input years
   if (Y1 < 2003 || Y1 > 2008 || Y2 < 2003 || Y2 > 2008)
   {
      printf("\nError: Years must be between 2003 and 2008.\n");
      return;
   }

   if (Y1 > Y2)
   {
      printf("\nError: Start year (Y1) must be less than or equal to end year (Y2).\n");
      return;
   }

   int start = find_value_Upper_bound_index(index_table, size, Y1);
   bool found;
   int var;
   if (start < 0)
   {
      return;
   }

   t_LnOVS *F;
   block buffer;
   rec record;
   Open(&F, fname, 'E');

   // Print table header
   printf("\n==============================================================================================================================================\n");
   printf("  ID   | Name                     | Gender | Birth Date  | Wilaya          | Blood  | Year  | Speciality                | Resident | Block,Offset\n");
   printf("-------+--------------------------+--------+-------------+-----------------+--------+-------+---------------------------+----------+-------------\n");

   int k = start;
   while ((k < size) && (index_table[k].Identifier <= Y2))
   {
      if ((index_table[k].Identifier <= Y2) && (index_table[k].Identifier >= Y1))
      {

         Pr_cor coordinats = index_table[k].crdt;
         ReadBlock(F, coordinats.block_number, &buffer);
         if (C43 != NULL)
         {
            (*C43)++;
         }

         record = buffer.tab[coordinats.offset];
         if ((2025 - record.Date_Birth.year) <= 20)
         {
            // Format full name
            char fullName[26];
            snprintf(fullName, sizeof(fullName), "%s %s", record.Family_Name, record.First_Name);
            if (strlen(fullName) > 24)
            {
               fullName[21] = '.';
               fullName[22] = '.';
               fullName[23] = '.';
               fullName[24] = '\0';
            }

            // Format wilaya
            char wilaya[17];
            strncpy(wilaya, record.Wilaya_Birth, 16);
            wilaya[16] = '\0';
            if (strlen(record.Wilaya_Birth) > 16)
            {
               wilaya[13] = '.';
               wilaya[14] = '.';
               wilaya[15] = '.';
            }

            // Format speciality
            char speciality[27];
            strncpy(speciality, record.Speciality, 26);
            speciality[26] = '\0';
            if (strlen(record.Speciality) > 26)
            {
               speciality[23] = '.';
               speciality[24] = '.';
               speciality[25] = '.';
            }

            // Print student row
            printf(" %-5d | %-24s | %-6s | %02d/%02d/%04d | %-15s | %-6s | %-5s | %-25s | %-8s | (%d,%d)\n",
                   record.Student_ID,
                   fullName,
                   record.Gender == 1 ? "Male" : "Female",
                   record.Date_Birth.day,
                   record.Date_Birth.month,
                   record.Date_Birth.year,
                   wilaya,
                   record.Blood_Type,
                   record.Year_Study,
                   speciality,
                   record.Resident_UC ? "Yes" : "No",
                   coordinats.block_number,
                   coordinats.offset);
         }
      }
      k++;
   }

   printf("==============================================================================================================================================\n\n");

   if (C43 != NULL)
   {
      printf("The complexity in this operation is: %d\n", *C43);
   }

   Close(F);
}

// 4.4. Question  Display students by thier year of study
void display_students_by_year_study(Pr_index index_table[], int size, int value, char *fname, int *C44)
{
   int start = find_value_Upper_bound_index(index_table, size, value);
   if (start < 0)
   {
      return;
   }

   t_LnOVS *F;
   block buffer;
   rec record;
   int k = start;

   Open(&F, fname, 'E');

   // Print table header
   printf("\n==============================================================================================================================================\n");
   printf("  ID   | Name                     | Gender | Birth Date  | Wilaya          | Blood  | Year  | Speciality                | Resident | Block,Offset\n");
   printf("-------+--------------------------+--------+-------------+-----------------+--------+-------+---------------------------+----------+-------------\n");

   while ((k < size) && (index_table[k].Identifier == value))
   {
      if (index_table[k].Identifier == value)
      {

         Pr_cor coordinats = index_table[k].crdt;
         ReadBlock(F, coordinats.block_number, &buffer);
         if (C44 != NULL)
         {
            (*C44)++;
         }

         record = buffer.tab[coordinats.offset];

         // Format full name
         char fullName[26];
         snprintf(fullName, sizeof(fullName), "%s %s", record.Family_Name, record.First_Name);
         if (strlen(fullName) > 24)
         {
            fullName[21] = '.';
            fullName[22] = '.';
            fullName[23] = '.';
            fullName[24] = '\0';
         }

         // Format wilaya
         char wilaya[17];
         strncpy(wilaya, record.Wilaya_Birth, 16);
         wilaya[16] = '\0';
         if (strlen(record.Wilaya_Birth) > 16)
         {
            wilaya[13] = '.';
            wilaya[14] = '.';
            wilaya[15] = '.';
         }

         // Format speciality
         char speciality[27];
         strncpy(speciality, record.Speciality, 26);
         speciality[26] = '\0';
         if (strlen(record.Speciality) > 26)
         {
            speciality[23] = '.';
            speciality[24] = '.';
            speciality[25] = '.';
         }

         // Print student row
         printf(" %-5d | %-24s | %-6s | %02d/%02d/%04d | %-15s | %-6s | %-5s | %-25s | %-8s | (%d,%d)\n",
                record.Student_ID,
                fullName,
                record.Gender == 1 ? "Male" : "Female",
                record.Date_Birth.day,
                record.Date_Birth.month,
                record.Date_Birth.year,
                wilaya,
                record.Blood_Type,
                record.Year_Study,
                speciality,
                record.Resident_UC ? "Yes" : "No",
                coordinats.block_number,
                coordinats.offset);
      }
      k++;
   }

   printf("==============================================================================================================================================\n\n");

   if (C44 != NULL)
   {
      printf("The complexity in this operation is: %d\n", *C44);
   }

   Close(F);
}

void display_header(t_LnOVS *F, char *Fname)
{

   Open(&F, Fname, 'E');

   long head = F->h.head; // the first block in the file (i.e. the head block of the list of blocks)
   long tail = F->h.tail;
   long freepos = F->h.freepos; // the first free position in the last block (i.e. in the tail block of the list)

   long lostspace = F->h.lostspace; // the total space lost due to logical deletions
   long newblck = F->h.newblck;     // the file border (the first free block available to expand the file size)
   long freeblck = F->h.freeblck;

   printf("\n========================================================\n");
   printf("                  FILE HEADER INFORMATION\n");
   printf("========================================================\n");
   printf("  File Name            : %s\n", Fname);
   printf("  Head Block           : %ld (First block in the list)\n", head);
   printf("  Tail Block           : %ld (Last block in the list)\n", tail);
   printf("  Free Position        : %ld (First free position in tail block)\n", freepos);
   printf("  Lost Space           : %ld (Space lost due to logical deletions)\n", lostspace);
   printf("  New Block Border     : %ld (Next new block to allocate)\n", newblck);
   printf("  Free Block List Head : %ld (Head of freed blocks list)\n", freeblck);
   printf("========================================================\n\n");

   Close(F);
}

void display_block(t_LnOVS *F, int blkN, char *Fname)
{
   Open(&F, Fname, 'E');
   block buff;
   ReadBlock(F, blkN, &buff);

   printf("\n========================================\n");
   printf("         BLOCK %d DETAILS\n", blkN);
   printf("========================================\n");
   printf("Number of records: %d\n", buff.Nb);
   printf("Next block: %ld\n", buff.next);
   printf("Previous block: %ld\n", buff.prev);
   printf("========================================\n\n");

   if (buff.Nb == 0)
   {
      printf("Block is empty.\n\n");
   }
   else
   {
      printf("==============================================================================================================================================\n");
      printf("  ID   | Name                     | Gender | Birth Date  | Wilaya          | Blood  | Year  | Speciality                | Resident\n");
      printf("-------+--------------------------+--------+-------------+-----------------+--------+-------+---------------------------+----------\n");

      for (int n = 0; n < buff.Nb; n++)
      {
         rec student = buff.tab[n];

         // Format full name
         char fullName[26];
         snprintf(fullName, sizeof(fullName), "%s %s", student.Family_Name, student.First_Name);
         if (strlen(fullName) > 24)
         {
            fullName[21] = '.';
            fullName[22] = '.';
            fullName[23] = '.';
            fullName[24] = '\0';
         }

         // Format wilaya
         char wilaya[17];
         strncpy(wilaya, student.Wilaya_Birth, 16);
         wilaya[16] = '\0';
         if (strlen(student.Wilaya_Birth) > 16)
         {
            wilaya[13] = '.';
            wilaya[14] = '.';
            wilaya[15] = '.';
         }

         // Format speciality
         char speciality[27];
         strncpy(speciality, student.Speciality, 26);
         speciality[26] = '\0';
         if (strlen(student.Speciality) > 26)
         {
            speciality[23] = '.';
            speciality[24] = '.';
            speciality[25] = '.';
         }

         // Print student row
         printf(" %-5d | %-24s | %-6s | %02d/%02d/%04d | %-15s | %-6s | %-5s | %-25s | %-8s\n",
                student.Student_ID,
                fullName,
                student.Gender == 1 ? "Male" : "Female",
                student.Date_Birth.day,
                student.Date_Birth.month,
                student.Date_Birth.year,
                wilaya,
                student.Blood_Type,
                student.Year_Study,
                speciality,
                student.Resident_UC ? "Yes" : "No");
      }

      printf("==============================================================================================================================================\n\n");
   }

   Close(F);
}

// dipaly all the content of the file

void DisplayAllContentsTableFileName(t_LnOVS *F, char *Fname)
{
   block currentBlock;
   rec studentRecord;
   Open(&F, Fname, 'E');
   printf("\n=========================================================================================================\n");
   printf("                                     STUDENT DATABASE - TABLE VIEW\n");
   printf("=========================================================================================================\n");
   printf(" ID   | Name                     | Gender | Birth Date  | Wilaya        | Year  | Speciality             \n");
   printf("------+--------------------------+--------+-------------+---------------+-------+------------------------\n");

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
   Close(F);
}

void create_CP_STUDENT(Pr_index index_table[], int max_size, t_LnOVS *student_file)
{
   t_LnOVS *F;

   Open(&F, "STUDENTS_CP.bin", 'N');
   block buff, buff_student;

   long current_block = 1;
   long first_block = current_block; // Save first block
   long previous_block = current_block;

   // Initialize buffer
   buff.Nb = 0;
   buff.prev = -1;
   buff.next = -1;
   setHeader(F, "head", first_block);
   setHeader(F, "tail", first_block);

   int load_factor = (40 * 75) / 100; // 30 records per block (75% of 40)

   for (long i = 0; i < max_size; i++)
   {
      // Read the student from the original file
      ReadBlock(student_file, index_table[i].crdt.block_number, &buff_student);

      if (buff.Nb < load_factor)
      {
         // Add student to current block
         buff.tab[buff.Nb] = buff_student.tab[index_table[i].crdt.offset];
         buff.Nb++;
      }
      else
      {
         // Current block is full, write it
         buff.next = current_block + 1;
         WriteBlock(F, current_block, &buff);

         // Prepare next block
         previous_block = current_block;
         current_block = AllocBlock(F);

         // Reset buffer for new block
         buff.Nb = 0;
         buff.prev = previous_block;
         buff.next = -1;

         // Add the current student to the new block
         buff.tab[buff.Nb] = buff_student.tab[index_table[i].crdt.offset];
         buff.Nb++;

         // Update tail in header
         setHeader(F, "tail", current_block);
      }
   }

   // Write the last block
   if (buff.Nb > 0)
   {
      buff.next = -1;
      WriteBlock(F, current_block, &buff);
      setHeader(F, "freepos", buff.Nb);
      setHeader(F, "tail", current_block);
   }
   else
   {
      // No students were added, still update headers
      setHeader(F, "freepos", 0);
   }

   Close(F);
}
#endif // MAINLIBRARY_H
